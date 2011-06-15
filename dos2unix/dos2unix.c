/*
 *  Name: dos2unix
 *  Documentation:
 *    Remove cr ('\x0d') characters from a file.
 *
 *  The dos2unix package is distributed under FreeBSD style license.
 *  See also http://www.freebsd.org/copyright/freebsd-license.html
 *  --------
 * 
 *  Copyright (C) 2009-2011 Erwin Waterlander
 *  Copyright (C) 1998 Christian Wurll
 *  Copyright (C) 1998 Bernd Johannes Wuebben
 *  Copyright (C) 1994-1995 Benjamin Lin.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice in the documentation and/or other materials provided with
 *     the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 *  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  == 1.0 == 1989.10.04 == John Birchfield (jb@koko.csustan.edu)
 *  == 1.1 == 1994.12.20 == Benjamin Lin (blin@socs.uts.edu.au)
 *     Cleaned up for Borland C/C++ 4.02
 *  == 1.2 == 1995.03.16 == Benjamin Lin (blin@socs.uts.edu.au)
 *     Modified to more conform to UNIX style.
 *  == 2.0 == 1995.03.19 == Benjamin Lin (blin@socs.uts.edu.au)
 *     Rewritten from scratch.
 *  == 2.1 == 1995.03.29 == Benjamin Lin (blin@socs.uts.edu.au)
 *     Conversion to SunOS charset implemented.
 *  == 2.2 == 1995.03.30 == Benjamin Lin (blin@socs.uts.edu.au)
 *     Fixed a bug in 2.1 where in new file mode, if outfile already exists
 *     conversion can not be completed properly.
 *
 * Added Mac text file translation, i.e. \r to \n conversion
 * Bernd Johannes Wuebben, wuebben@kde.org
 * Wed Feb  4 19:12:58 EST 1998      
 *
 * Added extra newline if ^M occurs
 * Christian Wurll, wurll@ira.uka.de
 * Thu Nov 19 1998 
 * 
 *  See ChangeLog.txt for complete version history.
 *
 */


#define VER_AUTHOR   "Erwin Waterlander"

/* #define DEBUG 1 */

/* define feature test macros for realpath() -- needed on    */
/* systems that have S_ISLNK, but chicken/egg means we must  */
/* define early, before including stdlib.h (or sys/stat.h)   */
#define _XOPEN_SOURCE 500

/* similarly, instead of realpath we like to use, if         */
/* available, the canonicalize_file_name() function, which   */
/* is a GNU extension. We only ACTUALLY use the function if  */
/* USE_CANONICALIZE_FILE_NAME is defined, but we don't define*/
/* that until later. So...define the feature test macro now. */
#define _GNU_SOURCE

#if defined(DJGPP) || defined(__TURBOC__) /* DJGPP */
#  include <dir.h>
#else
#  include <libgen.h>
#endif
#ifndef __TURBOC__
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>
#include <limits.h>
#ifdef __TURBOC__
#define __FLAT__
#endif
#include <sys/stat.h>
#ifdef ENABLE_NLS
#include <locale.h>
#endif
#include <errno.h>
#include "dos2unix.h"
#include "querycp.h"

#if defined(WIN32) && !defined(__CYGWIN__) /* MINGW32 */
#define MSDOS
#endif

#if  defined(__TURBOC__) || defined(DJGPP) || defined(__MINGW32__)
/* Some compilers have no mkstemp().
 * Use mktemp() instead.
 * BORLANDC, DJGPP, MINGW32 */
#define NO_MKSTEMP 1
#endif

#ifdef MSDOS
/* Some compilers have no fchmod().
 * BORLANDC, DJGPP, MINGW32, OPENWATCOM */
#define NO_FCHMOD 1
#endif

/* Watcom C defines S_ISLNK */
#ifdef __WATCOMC__
#undef S_ISLNK
#endif

#if defined(MSDOS) || defined(__OS2__)
/* Systems without soft links use 'stat' instead of 'lstat'. */
#define STAT stat
#else
#define STAT lstat
#endif

#if defined(MSDOS) || defined(__OS2__)
/* On some systems rename() will always fail if target file already exists. */
#define NEED_REMOVE 1
#endif

#if defined(MSDOS) || defined(__CYGWIN__) || defined(__OS2__) /* DJGPP, MINGW32 and OS/2 */
/* required for setmode() and O_BINARY */
#include <fcntl.h>
#include <io.h>
#endif

#if defined(__GLIBC__)
/* on glibc, canonicalize_file_name() broken prior to 2.4 (06-Mar-2006) */
# if __GNUC_PREREQ (2,4)
#  define USE_CANONICALIZE_FILE_NAME 1
# endif
#elif defined(__CYGWIN__)
/* on cygwin, canonicalize_file_name() available since api 0/213 */
/* (1.7.0beta61, 25-Sep-09) */
# include <cygwin/version.h>
# if (CYGWIN_VERSION_DLL_COMBINED >= 213)
#  define USE_CANONICALIZE_FILE_NAME 1
# endif
#endif

#if defined(MSDOS) || defined(__CYGWIN__) || defined(__OS2__)
  #define R_CNTRL   "rb"
  #define W_CNTRL   "wb"
#else
  #define R_CNTRL   "r"
  #define W_CNTRL   "w"
#endif

#define BINARY_FILE 0x1
#define NO_REGFILE  0x2
#define WRONG_CODEPAGE  0x4
#define OUTPUTFILE_SYMLINK 0x8
#define INPUT_TARGET_NO_REGFILE 0x10
#define OUTPUT_TARGET_NO_REGFILE 0x20

#define CONVMODE_ASCII  0
#define CONVMODE_7BIT   1
#define CONVMODE_437    437
#define CONVMODE_850    850
#define CONVMODE_860    860
#define CONVMODE_863    863
#define CONVMODE_865    865
#define CONVMODE_1252   1252

#define FROMTO_DOS2UNIX 0
#define FROMTO_MAC2UNIX 1

typedef struct
{
  int NewFile;                          /* is in new file mode? */
  int Quiet;                            /* is in quiet mode? */
  int KeepDate;                         /* should keep date stamp? */
  int ConvMode;                         /* 0: ascii, 1: 7bit, 2: iso */  
  int FromToMode;                       /* 0: dos2unix, 1: mac2unix */  
  int NewLine;                          /* if TRUE, then additional newline */
  int Force;                            /* if TRUE, force conversion of all files. */
  int Follow;                           /* 0: skip symlink, 1: follow symbolic link, 2: replace symlink. */
  int status;
  int stdio_mode;                       /* if TRUE, stdio mode */
  int error;                            /* an error occurred */
} CFlag;

/******************************************************************
 *
 * int symbolic_link(char *path)
 *
 * test if *path points to a that exists and is a symbolic link
 *
 * returns 1 on success, 0 when it fails.
 *
 ******************************************************************/
int symbolic_link(char *path)
{
#ifdef S_ISLNK
   struct stat buf;

   if (STAT(path, &buf) == 0)
   {
      if (S_ISLNK(buf.st_mode))
         return(1);
   }
#endif
   return(0);
}

/******************************************************************
 *
 * int regfile(char *path, int allowSymlinks)
 *
 * test if *path points to a regular file (or is a symbolic link,
 * if allowSymlinks != 0).
 *
 * returns 0 on success, -1 when it fails.
 *
 ******************************************************************/
int regfile(char *path, int allowSymlinks, CFlag *ipFlag, char *progname)
{
   struct stat buf;
   char *errstr;

   if (STAT(path, &buf) == 0)
   {
#if DEBUG
      fprintf(stderr, "%s: %s MODE 0%o ", progname, path, buf.st_mode);
#ifdef S_ISSOCK
      if (S_ISSOCK(buf.st_mode))
         fprintf(stderr, " (socket)");
#endif
#ifdef S_ISLNK
      if (S_ISLNK(buf.st_mode))
         fprintf(stderr, " (symbolic link)");
#endif
      if (S_ISREG(buf.st_mode))
         fprintf(stderr, " (regular file)");
      if (S_ISBLK(buf.st_mode))
         fprintf(stderr, " (block device)");
      if (S_ISDIR(buf.st_mode))
         fprintf(stderr, " (directory)");
      if (S_ISCHR(buf.st_mode))
         fprintf(stderr, " (character device)");
      if (S_ISFIFO(buf.st_mode))
         fprintf(stderr, " (FIFO)");
      fprintf(stderr, "\n");
#endif
      if ((S_ISREG(buf.st_mode))
#ifdef S_ISLNK
          || (S_ISLNK(buf.st_mode) && allowSymlinks)
#endif
         )
         return(0);
      else
         return(-1);
   }
   else
   {
     if (!ipFlag->Quiet)
     {
       ipFlag->error = errno;
       errstr = strerror(errno);
       fprintf(stderr, "%s: %s: %s\n", progname, path, errstr);
     }
     return(-1);
   }
}

/******************************************************************
 *
 * int regfile_target(char *path)
 *
 * test if *path points to a regular file (follow symbolic link)
 *
 * returns 0 on success, -1 when it fails.
 *
 ******************************************************************/
int regfile_target(char *path, CFlag *ipFlag, char *progname)
{
   struct stat buf;
   char *errstr;

   if (stat(path, &buf) == 0)
   {
      if (S_ISREG(buf.st_mode))
         return(0);
      else
         return(-1);
   }
   else
   {
     if (!ipFlag->Quiet)
     {
       ipFlag->error = errno;
       errstr = strerror(errno);
       fprintf(stderr, "%s: %s: %s\n", progname, path, errstr);
     }
     return(-1);
   }
}

void PrintUsage(char *progname)
{
  fprintf(stderr, _("\
%s %s (%s)\n\
Usage: %s [options] [file ...] [-n infile outfile ...]\n\
 -ascii                convert only line breaks (default)\n\
 -iso                  conversion between DOS and ISO-8859-1 character set\n\
   -1252               Use Windows code page 1252 (Western European)\n\
   -437                Use DOS code page 437 (US) (default)\n\
   -850                Use DOS code page 850 (Western European)\n\
   -860                Use DOS code page 860 (Portuguese)\n\
   -863                Use DOS code page 863 (French Canadian)\n\
   -865                Use DOS code page 865 (Nordic)\n\
 -7                    Convert 8 bit characters to 7 bit space\n\
 -c, --convmode        conversion mode\n\
   convmode            ascii, 7bit, iso, mac, default to ascii\n\
 -f, --force           force conversion of binary files\n\
 -h, --help            give this help\n\
 -k, --keepdate        keep output file date\n\
 -L, --license         display software license\n\
 -l, --newline         add additional newline\n\
 -n, --newfile         write to new file\n\
   infile              original file in new file mode\n\
   outfile             output file in new file mode\n\
 -o, --oldfile         write to old file\n\
   file ...            files to convert in old file mode\n\
 -q, --quiet           quiet mode, suppress all warnings\n\
                       always on in stdio mode\n\
 -s, --safe            skip binary files (default)\n"),
 progname, VER_REVISION, VER_DATE, progname);
#ifdef S_ISLNK
  fprintf(stderr, _("\
 -F, --follow-symlink  follow symbolic links and convert the targets\n\
 -R, --replace-symlink replace symbolic links with converted files\n\
                       (original target files remain unchanged)\n\
 -S, --skip-symlink    keep symbolic links and targets unchanged (default)\n"));
#endif
  fprintf(stderr, _("\
 -V, --version         display version number\n"));
}

void PrintLicense(void)
{
  fprintf(stderr, "%s", _("\
Copyright (C) 2009-2011 Erwin Waterlander\n\
Copyright (C) 1998      Christian Wurll (Version 3.1)\n\
Copyright (C) 1998      Bernd Johannes Wuebben (Version 3.0)\n\
Copyright (C) 1994-1995 Benjamin Lin\n\
All rights reserved.\n\n\
\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions\n\
are met:\n\
1. Redistributions of source code must retain the above copyright\n\
   notice, this list of conditions and the following disclaimer.\n\
2. Redistributions in binary form must reproduce the above copyright\n\
   notice in the documentation and/or other materials provided with\n\
   the distribution.\n\n\
\
THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY\n\
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n\
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n\
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE\n\
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n\
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT\n\
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR\n\
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,\n\
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE\n\
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN\n\
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
"));
}

void PrintVersion(char *progname)
{
  fprintf(stderr, "%s %s (%s)\n", progname, VER_REVISION, VER_DATE);
#ifdef ENABLE_NLS
  fprintf(stderr, "%s", _("With native language support.\n"));
#else
  fprintf(stderr, "%s", "Without native language support.\n");
#endif
#if DEBUG
  fprintf(stderr, "VER_AUTHOR: %s\n", VER_AUTHOR);
#endif
}

#ifdef ENABLE_NLS
void PrintLocaledir(char *localedir)
{
  fprintf(stderr, "LOCALEDIR: %s\n", localedir);
}
#endif

/* opens file of name ipFN in read only mode
 * RetVal: NULL if failure
 *         file stream otherwise
 */
FILE* OpenInFile(char *ipFN)
{
  return (fopen(ipFN, R_CNTRL));
}


/* opens file of name ipFN in write only mode
 * RetVal: NULL if failure
 *         file stream otherwise
 */
FILE* OpenOutFile(int fd)
{
  return (fdopen(fd, W_CNTRL));
}


void StripDelimiter(FILE* ipInF, FILE* ipOutF, CFlag *ipFlag, int CurChar)
{
  int TempNextChar;
  /* CurChar is always CR (x0d) */
  /* In normal dos2unix mode put nothing (skip CR). */
  /* Don't modify Mac files when in dos2unix mode. */
  if ( (TempNextChar = getc(ipInF)) != EOF) {
    ungetc( TempNextChar, ipInF );  /* put back peek char */
    if ( TempNextChar != '\x0a' ) {
      putc( CurChar, ipOutF );  /* Mac line, put back CR */
    }
  }
  else if ( CurChar == '\x0d' ) {  /* EOF: last Mac line delimiter (CR)? */
    putc( CurChar, ipOutF );
  }
  if (ipFlag->NewLine) {  /* add additional LF? */
    putc('\x0a', ipOutF);
  }
}

/* converts stream ipInF to UNIX format text and write to stream ipOutF
 * RetVal: 0  if success
 *         -1  otherwise
 */
int ConvertDosToUnix(FILE* ipInF, FILE* ipOutF, CFlag *ipFlag, char *progname)
{
    int RetVal = 0;
    int TempChar;
    int TempNextChar;
    int *ConvTable;

    ipFlag->status = 0;

    switch (ipFlag->ConvMode)
    {
      case CONVMODE_ASCII: /* ascii */
        ConvTable = D2UAsciiTable;
        break;
      case CONVMODE_7BIT: /* 7bit */
        ConvTable = D2U7BitTable;
        break;
      case CONVMODE_437: /* iso */
        ConvTable = D2UIso437Table;
        break;
      case CONVMODE_850: /* iso */
        ConvTable = D2UIso850Table;
        break;
      case CONVMODE_860: /* iso */
        ConvTable = D2UIso860Table;
        break;
      case CONVMODE_863: /* iso */
        ConvTable = D2UIso863Table;
        break;
      case CONVMODE_865: /* iso */
        ConvTable = D2UIso865Table;
        break;
      case CONVMODE_1252: /* iso */
        ConvTable = D2UIso1252Table;
        break;
      default: /* unknown convmode */
        ipFlag->status |= WRONG_CODEPAGE ;
        return(-1);
    }
    if ((ipFlag->ConvMode > 1) && (!ipFlag->Quiet)) /* not ascii or 7bit */
    {
       fprintf(stderr, "%s: ", progname);
       fprintf(stderr, _("using code page %d.\n"), ipFlag->ConvMode);
    }

    /* CR-LF -> LF */
    /* LF    -> LF, in case the input file is a Unix text file */
    /* CR    -> CR, in dos2unix mode (don't modify Mac file) */
    /* CR    -> LF, in Mac mode */
    /* \x0a = Newline/Line Feed (LF) */
    /* \x0d = Carriage Return (CR) */

    switch (ipFlag->FromToMode)
    {
      case FROMTO_DOS2UNIX: /* dos2unix */
        while ((TempChar = getc(ipInF)) != EOF) {  /* get character */
          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09') &&  /* Not a TAB */
              (TempChar != '\x0c')) {  /* Not a form feed */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
          if (TempChar != '\x0d') {
            if (putc(ConvTable[TempChar], ipOutF) == EOF) {
              RetVal = -1;
              if (!ipFlag->Quiet)
              {
                fprintf(stderr, "%s: ", progname);
                fprintf(stderr, "%s", _("can not write to output file\n"));
              }
              break;
            } 
          } else {
            StripDelimiter( ipInF, ipOutF, ipFlag, TempChar );
          }
        }
        break;
      case FROMTO_MAC2UNIX: /* mac2unix */
        while ((TempChar = getc(ipInF)) != EOF) {
          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09') &&  /* Not a TAB */
              (TempChar != '\x0c')) {  /* Not a form feed */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
          if ((TempChar != '\x0d'))
            {
              if(putc(ConvTable[TempChar], ipOutF) == EOF){
                RetVal = -1;
                if (!ipFlag->Quiet)
                {
                  fprintf(stderr, "%s: ", progname);
                  fprintf(stderr, "%s", _("can not write to output file\n"));
                }
                break;
              }
            }
          else{
            /* TempChar is a CR */
            if ( (TempNextChar = getc(ipInF)) != EOF) {
              ungetc( TempNextChar, ipInF );  /* put back peek char */
              /* Don't touch this delimiter if it's a CR,LF pair. */
              if ( TempNextChar == '\x0a' ) {
                putc('\x0d', ipOutF); /* put CR, part of DOS CR-LF */
                continue;
              }
            }
            if (putc('\x0a', ipOutF) == EOF) /* MAC line end (CR). Put LF */
              {
                RetVal = -1;
                if (!ipFlag->Quiet)
                {
                  fprintf(stderr, "%s: ", progname);
                  fprintf(stderr, "%s", _("can not write to output file\n"));
                }
                break;
              }
            if (ipFlag->NewLine) {  /* add additional LF? */
              putc('\x0a', ipOutF);
            }
          }
        }
        break;
      default: /* unknown FromToMode */
      ;
#if DEBUG
      fprintf(stderr, "%s: ", progname);
      fprintf(stderr, _("program error, invalid conversion mode %d\n"),ipFlag->FromToMode);
      exit(1);
#endif
    }
    return RetVal;
}

#ifdef __TURBOC__
char *dirname(char *path)
{
  char *ptr;

  if (( path == NULL) || (((ptr=strrchr(path,'/')) == NULL) && ((ptr=strrchr(path,'\\')) == NULL)) )
    return ".";
  else
  {
    *ptr = '\0';
    return(path);
  }
}
#endif

#ifdef NO_MKSTEMP
FILE* MakeTempFileFrom(const char *OutFN, char **fname_ret)
#else
static int MakeTempFileFrom(const char *OutFN, char **fname_ret)
#endif
{
  char *cpy = strdup(OutFN);
  char *dir = NULL;
  size_t fname_len = 0;
  char  *fname_str = NULL;
#ifdef NO_MKSTEMP
  char *name;
  FILE *fd = NULL;
#else
  int fd = -1;
#endif
  
  *fname_ret = NULL;

  if (!cpy)
    goto make_failed;
  
  dir = dirname(cpy);
  
  fname_len = strlen(dir) + strlen("/d2utmpXXXXXX") + sizeof (char);
  if (!(fname_str = malloc(fname_len)))
    goto make_failed;
  sprintf(fname_str, "%s%s", dir, "/d2utmpXXXXXX");
  *fname_ret = fname_str;

  free(cpy);

#ifdef NO_MKSTEMP
  name = mktemp(fname_str);
  *fname_ret = name;
  if ((fd = fopen(fname_str, W_CNTRL)) == NULL)
    goto make_failed;
#else
  if ((fd = mkstemp(fname_str)) == -1)
    goto make_failed;
#endif
  
  return (fd);
  
 make_failed:
  free(*fname_ret);
  *fname_ret = NULL;
#ifdef NO_MKSTEMP
  return (NULL);
#else
  return (-1);
#endif
}

/* Test if *lFN is the name of a symbolic link.  If not, set *rFN equal
 * to lFN, and return 0.  If so, then use canonicalize_file_name or
 * realpath to determine the pointed-to file; the resulting name is
 * stored in newly allocated memory, *rFN is set to point to that value,
 * and 1 is returned. On error, -1 is returned and errno is set as
 * appropriate.
 *
 * Note that if symbolic links are not supported, then 0 is always returned
 * and *rFN = lFN.
 *
 * RetVal: 0 if success, and *lFN is not a symlink
 *         1 if success, and *lFN is a symlink
 *         -1 otherwise
 */
int ResolveSymbolicLink(char *lFN, char **rFN, CFlag *ipFlag, char *progname)
{
  int RetVal = 0;
#ifdef S_ISLNK
  struct stat StatBuf;
  char *errstr;
  char *targetFN = NULL;

  if (STAT(lFN, &StatBuf))
  {
    if (!ipFlag->Quiet)
    {
      ipFlag->error = errno;
      errstr = strerror(errno);
      fprintf(stderr, "%s: %s: %s\n", progname, lFN, errstr);
    }
    RetVal = -1;
  }
  else if (S_ISLNK(StatBuf.st_mode))
  {
#if USE_CANONICALIZE_FILE_NAME
    targetFN = canonicalize_file_name(lFN);
    if (!targetFN)
    {
      if (!ipFlag->Quiet)
      {
        errstr = strerror(errno);
        fprintf(stderr, "%s: %s: %s\n", progname, lFN, errstr);
        ipFlag->error = 1;
      }
      RetVal = -1;
    }
    else
    {
      *rFN = targetFN;
      RetVal = 1;
    }
#else
    /* Sigh. Use realpath, but realize that it has a fatal
     * flaw: PATH_MAX isn't necessarily the maximum path
     * length -- so realpath() might fail. */
    targetFN = (char *) malloc(PATH_MAX * sizeof(char));
    if (!targetFN)
    {
      if (!ipFlag->Quiet)
      {
        errstr = strerror(errno);
        fprintf(stderr, "%s: %s: %s\n", progname, lFN, errstr);
        ipFlag->error = 1;
      }
      RetVal = -1;
    }
    else
    {
      /* is there any platform with S_ISLNK that does not have realpath? */
      char *rVal = realpath(lFN, targetFN);
      if (!rVal)
      {
        if (!ipFlag->Quiet)
        {
          errstr = strerror(errno);
          fprintf(stderr, "%s: %s: %s\n", progname, lFN, errstr);
          ipFlag->error = 1;
        }
        free(targetFN);
        RetVal = -1;
      }
      else
      {
        *rFN = rVal;
        RetVal = 1;
      }
    }
#endif /* !USE_CANONICALIZE_FILE_NAME */
  }
  else
    *rFN = lFN;
#else  /* !S_ISLNK */
  *rFN = lFN;
#endif /* !S_ISLNK */
  return RetVal;
}

/* convert file ipInFN to UNIX format text and write to file ipOutFN
 * RetVal: 0 if success
 *         -1 otherwise
 */
int ConvertDosToUnixNewFile(char *ipInFN, char *ipOutFN, CFlag *ipFlag, char *progname)
{
  int RetVal = 0;
  FILE *InF = NULL;
  FILE *TempF = NULL;
  char *TempPath;
  char *errstr;
  struct stat StatBuf;
  struct utimbuf UTimeBuf;
#ifndef NO_FCHMOD
  mode_t mask;
#endif
#ifdef NO_MKSTEMP
  FILE* fd;
#else
  int fd;
#endif
  char *TargetFN = NULL;
  int ResolveSymlinkResult = 0;

  ipFlag->status = 0 ;

  /* Test if output file is a symbolic link */
  if (symbolic_link(ipOutFN) && !ipFlag->Follow)
  {
    ipFlag->status |= OUTPUTFILE_SYMLINK ;
    /* Not a failure, skipping input file according spec. (keep symbolic link unchanged) */
    return -1;
  }

  /* Test if input file is a regular file or symbolic link */
  if (regfile(ipInFN, 1, ipFlag, progname))
  {
    ipFlag->status |= NO_REGFILE ;
    /* Not a failure, skipping non-regular input file according spec. */
    return -1;
  }

  /* Test if input file target is a regular file */
  if (symbolic_link(ipInFN) && regfile_target(ipInFN, ipFlag,progname))
  {
    ipFlag->status |= INPUT_TARGET_NO_REGFILE ;
    /* Not a failure, skipping non-regular input file according spec. */
    return -1;
  }

  /* Test if output file target is a regular file */
  if (symbolic_link(ipOutFN) && (ipFlag->Follow == 1) && regfile_target(ipOutFN, ipFlag,progname))
  {
    ipFlag->status |= OUTPUT_TARGET_NO_REGFILE ;
    /* Failure, input is regular, cannot produce output. */
    if (!ipFlag->error) ipFlag->error = 1;
    return -1;
  }

  /* retrieve ipInFN file date stamp */
  if (stat(ipInFN, &StatBuf))
  {
    if (!ipFlag->Quiet)
    {
      ipFlag->error = errno;
      errstr = strerror(errno);
      fprintf(stderr, "%s: %s: %s\n", progname, ipInFN, errstr);
    }
    RetVal = -1;
  }

#ifdef NO_MKSTEMP
  if((fd = MakeTempFileFrom(ipOutFN, &TempPath))==NULL) {
#else
  if((fd = MakeTempFileFrom (ipOutFN, &TempPath)) < 0) {
#endif
    if (!ipFlag->Quiet)
    {
      ipFlag->error = errno;
      fprintf(stderr, "%s: ", progname);
      perror(_("Failed to open temporary output file"));
    }
    RetVal = -1;
  }

#if DEBUG
  fprintf(stderr, "%s: ", progname);
  fprintf(stderr, _("using %s as temporary file\n"), TempPath);
#endif

  /* can open in file? */
  if (!RetVal)
  {
    InF=OpenInFile(ipInFN);
    if (InF == NULL)
    {
      ipFlag->error = errno;
      errstr = strerror(errno);
      fprintf(stderr, "%s: %s: %s\n", progname, ipInFN, errstr);
      RetVal = -1;
    }
  }

  /* can open output file? */
  if ((!RetVal) && (InF))
  {
#ifdef NO_MKSTEMP
    if ((TempF=fd) == NULL)
    {
#else
    if ((TempF=OpenOutFile(fd)) == NULL)
    {
      ipFlag->error = errno;
      errstr = strerror(errno);
      fprintf(stderr, "%s: %s\n", progname, errstr);
#endif
      fclose (InF);
      InF = NULL;
      RetVal = -1;
    }
  }

#ifndef NO_FCHMOD
  /* preserve original mode as modified by umask */
  mask = umask(0);
  umask(mask);
  if (!RetVal && fchmod(fd, StatBuf.st_mode & ~mask))
    RetVal = -1;
#endif

  /* conversion sucessful? */
  if ((!RetVal) && (ConvertDosToUnix(InF, TempF, ipFlag, progname)))
    RetVal = -1;

   /* can close in file? */
  if ((InF) && (fclose(InF) == EOF))
    RetVal = -1;

  /* can close output file? */
  if ((TempF) && (fclose(TempF) == EOF))
    RetVal = -1;

#ifdef NO_MKSTEMP
  if(fd!=NULL)
    fclose(fd);
#else
  if(fd>=0)
    close(fd);
#endif

  if ((!RetVal) && (ipFlag->KeepDate))
  {
    UTimeBuf.actime = StatBuf.st_atime;
    UTimeBuf.modtime = StatBuf.st_mtime;
    /* can change output file time to in file time? */
    if (utime(TempPath, &UTimeBuf) == -1)
    {
      if (!ipFlag->Quiet)
      {
        ipFlag->error = errno;
        errstr = strerror(errno);
        fprintf(stderr, "%s: %s: %s\n", progname, TempPath, errstr);
      }
      RetVal = -1;
    }
  }

  /* any error? cleanup the temp file */
  if (RetVal && (TempPath != NULL))
  {
    if (unlink(TempPath) && (errno != ENOENT))
    {
      if (!ipFlag->Quiet)
      {
        ipFlag->error = errno;
        errstr = strerror(errno);
        fprintf(stderr, "%s: %s: %s\n", progname, TempPath, errstr);
      }
      RetVal = -1;
    }
  }

  /* If output file is a symbolic link, optional resolve the link and modify  */
  /* the target, instead of removing the link and creating a new regular file */
  TargetFN = ipOutFN;
  if (symbolic_link(ipOutFN) && !RetVal)
  {
    ResolveSymlinkResult = 0; /* indicates that TargetFN need not be freed */
    if (ipFlag->Follow == 1)
    {
      ResolveSymlinkResult = ResolveSymbolicLink(ipOutFN, &TargetFN, ipFlag, progname);
      if (ResolveSymlinkResult < 0)
      {
        if (!ipFlag->Quiet)
        {
          fprintf(stderr, "%s: ", progname);
          fprintf(stderr, _("problems resolving symbolic link '%s'\n"), ipOutFN);
          fprintf(stderr, _("          output file remains in '%s'\n"), TempPath);
        }
        RetVal = -1;
      }
    }
  }

  /* can rename temporary file to output file? */
  if (!RetVal)
  {
#ifdef NEED_REMOVE
    if (unlink(TargetFN) && (errno != ENOENT))
    {
      if (!ipFlag->Quiet)
      {
        ipFlag->error = errno;
        errstr = strerror(errno);
        fprintf(stderr, "%s: %s: %s\n", progname, TargetFN, errstr);
      }
      RetVal = -1;
    }
#endif
    if (rename(TempPath, TargetFN) == -1)
    {
      if (!ipFlag->Quiet)
      {
        ipFlag->error = errno;
        errstr = strerror(errno);
        fprintf(stderr, "%s: ", progname);
        fprintf(stderr, _("problems renaming '%s' to '%s': %s\n"), TempPath, TargetFN, errstr);
#ifdef S_ISLNK
        if (ResolveSymlinkResult > 0)
          fprintf(stderr, _("          which is the target of symbolic link '%s'\n"), ipOutFN);
#endif
        fprintf(stderr, _("          output file remains in '%s'\n"), TempPath);
      }
      RetVal = -1;
    }

    if (ResolveSymlinkResult > 0)
      free(TargetFN);
  }
  free(TempPath);
  return RetVal;
}

/* convert stdin to UNIX format text and write to stdout
 * RetVal: 0 if success
 *         -1 otherwise
 */
int ConvertDosToUnixStdio(CFlag *ipFlag, char *progname)
{
    ipFlag->NewFile = 1;
    ipFlag->Quiet = 1;
    ipFlag->KeepDate = 0;
    ipFlag->Force = 1;

#if defined(WIN32) && !defined(__CYGWIN__)

    /* stdin and stdout are by default text streams. We need
     * to set them to binary mode. Otherwise an LF will
     * automatically be converted to CR-LF on DOS/Windows.
     * Erwin */

    /* 'setmode' was deprecated by MicroSoft
     * since Visual C++ 2005. Use '_setmode' instead. */

    _setmode(fileno(stdout), O_BINARY);
    _setmode(fileno(stdin), O_BINARY);
    return (ConvertDosToUnix(stdin, stdout, ipFlag, progname));
#elif defined(MSDOS) || defined(__CYGWIN__) || defined(__OS2__)
    setmode(fileno(stdout), O_BINARY);
    setmode(fileno(stdin), O_BINARY);
    return (ConvertDosToUnix(stdin, stdout, ipFlag, progname));
#else
    return (ConvertDosToUnix(stdin, stdout, ipFlag, progname));
#endif
}


int main (int argc, char *argv[])
{
  /* variable declarations */
  char progname[9];
  int ArgIdx;
  int CanSwitchFileMode;
  int ShouldExit;
  int RetVal = 0;
  int process_options = 1;
  CFlag *pFlag;
  char *ptr;
#ifdef ENABLE_NLS
  char localedir[1024];
#endif

#ifdef ENABLE_NLS
   ptr = getenv("DOS2UNIX_LOCALEDIR");
   if (ptr == NULL)
      strcpy(localedir,LOCALEDIR);
   else
   {
      if (strlen(ptr) < sizeof(localedir))
         strcpy(localedir,ptr);
      else
      {
         fprintf(stderr, "%s", _("dos2unix: error: Value of environment variable DOS2UNIX_LOCALEDIR is too long.\n"));
         strcpy(localedir,LOCALEDIR);
      }
   }

   setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, localedir);
   textdomain (PACKAGE);
#endif


  /* variable initialisations */
  progname[8] = '\0';
  ArgIdx = 0;
  CanSwitchFileMode = 1;
  ShouldExit = 0;
  pFlag = (CFlag*)malloc(sizeof(CFlag));  
  pFlag->NewFile = 0;
  pFlag->Quiet = 0;
  pFlag->KeepDate = 0;
  pFlag->ConvMode = CONVMODE_ASCII;  /* default ascii */
  pFlag->FromToMode = FROMTO_DOS2UNIX;  /* default dos2unix */
  pFlag->NewLine = 0;
  pFlag->Force = 0;
  pFlag->Follow = 0;
  pFlag->status = 0;
  pFlag->stdio_mode = 1;
  pFlag->error = 0;

  if ( ((ptr=strrchr(argv[0],'/')) == NULL) && ((ptr=strrchr(argv[0],'\\')) == NULL) )
    ptr = argv[0];
  else
    ptr++;

  if ((strcmpi("mac2unix", ptr) == 0) || (strcmpi("mac2unix.exe", ptr) == 0))
  {
    pFlag->FromToMode = FROMTO_MAC2UNIX;
    strcpy(progname,"mac2unix");
  }
  else
  {
    strcpy(progname,"dos2unix");
  }

  while ((++ArgIdx < argc) && (!ShouldExit))
  {
    /* is it an option? */
    if ((argv[ArgIdx][0] == '-') && process_options)
    {
      /* an option */
      if (strcmp(argv[ArgIdx],"--") == 0)
        process_options = 0;
      else if ((strcmp(argv[ArgIdx],"-h") == 0) || (strcmp(argv[ArgIdx],"--help") == 0))
      {
        PrintUsage(progname);
        return(pFlag->error);
      }
      else if ((strcmp(argv[ArgIdx],"-k") == 0) || (strcmp(argv[ArgIdx],"--keepdate") == 0))
        pFlag->KeepDate = 1;
      else if ((strcmp(argv[ArgIdx],"-f") == 0) || (strcmp(argv[ArgIdx],"--force") == 0))
        pFlag->Force = 1;
      else if ((strcmp(argv[ArgIdx],"-s") == 0) || (strcmp(argv[ArgIdx],"--safe") == 0))
        pFlag->Force = 0;
      else if ((strcmp(argv[ArgIdx],"-q") == 0) || (strcmp(argv[ArgIdx],"--quiet") == 0))
        pFlag->Quiet = 1;
      else if ((strcmp(argv[ArgIdx],"-l") == 0) || (strcmp(argv[ArgIdx],"--newline") == 0))
        pFlag->NewLine = 1;
      else if ((strcmp(argv[ArgIdx],"-S") == 0) || (strcmp(argv[ArgIdx],"--skip-symlink") == 0))
        pFlag->Follow = 0;
      else if ((strcmp(argv[ArgIdx],"-F") == 0) || (strcmp(argv[ArgIdx],"--follow-symlink") == 0))
        pFlag->Follow = 1;
      else if ((strcmp(argv[ArgIdx],"-R") == 0) || (strcmp(argv[ArgIdx],"--replace-symlink") == 0))
        pFlag->Follow = 2;
      else if ((strcmp(argv[ArgIdx],"-V") == 0) || (strcmp(argv[ArgIdx],"--version") == 0))
      {
        PrintVersion(progname);
#ifdef ENABLE_NLS
        PrintLocaledir(localedir);
#endif
        return(pFlag->error);
      }
      else if ((strcmp(argv[ArgIdx],"-L") == 0) || (strcmp(argv[ArgIdx],"--license") == 0))
      {
        PrintLicense();
        return(pFlag->error);
      }
      else if (strcmp(argv[ArgIdx],"-ascii") == 0)  /* SunOS compatible options */
        pFlag->ConvMode = CONVMODE_ASCII;
      else if (strcmp(argv[ArgIdx],"-7") == 0)
        pFlag->ConvMode = CONVMODE_7BIT;
      else if (strcmp(argv[ArgIdx],"-iso") == 0)
      {
        pFlag->ConvMode = (int)query_con_codepage();
        if (!pFlag->Quiet)
        {
           fprintf(stderr,"%s: ",progname);
           fprintf(stderr,_("active code page: %d\n"), pFlag->ConvMode);
        }
        if (pFlag->ConvMode < 2)
           pFlag->ConvMode = CONVMODE_437;
      }
      else if (strcmp(argv[ArgIdx],"-437") == 0)
        pFlag->ConvMode = CONVMODE_437;
      else if (strcmp(argv[ArgIdx],"-850") == 0)
        pFlag->ConvMode = CONVMODE_850;
      else if (strcmp(argv[ArgIdx],"-860") == 0)
        pFlag->ConvMode = CONVMODE_860;
      else if (strcmp(argv[ArgIdx],"-863") == 0)
        pFlag->ConvMode = CONVMODE_863;
      else if (strcmp(argv[ArgIdx],"-865") == 0)
        pFlag->ConvMode = CONVMODE_865;
      else if (strcmp(argv[ArgIdx],"-1252") == 0)
        pFlag->ConvMode = CONVMODE_1252;
      else if ((strcmp(argv[ArgIdx],"-c") == 0) || (strcmp(argv[ArgIdx],"--convmode") == 0))
      {
        if (++ArgIdx < argc)
        {
          if (strcmpi(argv[ArgIdx],"ascii") == 0)  /* Benjamin Lin's legacy options */
            pFlag->ConvMode = CONVMODE_ASCII;
          else if (strcmpi(argv[ArgIdx], "7bit") == 0)
            pFlag->ConvMode = CONVMODE_7BIT;
          else if (strcmpi(argv[ArgIdx], "iso") == 0)
          {
            pFlag->ConvMode = (int)query_con_codepage();
            if (!pFlag->Quiet)
            {
               fprintf(stderr,"%s: ",progname);
               fprintf(stderr,_("active code page: %d\n"), pFlag->ConvMode);
            }
            if (pFlag->ConvMode < 2)
               pFlag->ConvMode = CONVMODE_437;
          }
          else if (strcmpi(argv[ArgIdx], "mac") == 0)
            pFlag->FromToMode = FROMTO_MAC2UNIX;
          else
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("invalid %s conversion mode specified\n"),argv[ArgIdx]);
            pFlag->error = 1;
            ShouldExit = 1;
            pFlag->stdio_mode = 0;
          }
        }
        else
        {
          ArgIdx--;
          fprintf(stderr,"%s: ",progname);
          fprintf(stderr,_("option '%s' requires an argument\n"),argv[ArgIdx]);
          pFlag->error = 1;
          ShouldExit = 1;
          pFlag->stdio_mode = 0;
        }
      }

      else if ((strcmp(argv[ArgIdx],"-o") == 0) || (strcmp(argv[ArgIdx],"--oldfile") == 0))
      {
        /* last convert not paired */
        if (!CanSwitchFileMode)
        {
          fprintf(stderr,"%s: ",progname);
          fprintf(stderr, _("target of file %s not specified in new file mode\n"), argv[ArgIdx-1]);
          pFlag->error = 1;
          ShouldExit = 1;
          pFlag->stdio_mode = 0;
        }
        pFlag->NewFile = 0;
      }

      else if ((strcmp(argv[ArgIdx],"-n") == 0) || (strcmp(argv[ArgIdx],"--newfile") == 0))
      {
        /* last convert not paired */
        if (!CanSwitchFileMode)
        {
          fprintf(stderr,"%s: ",progname);
          fprintf(stderr, _("target of file %s not specified in new file mode\n"), argv[ArgIdx-1]);
          pFlag->error = 1;
          ShouldExit = 1;
          pFlag->stdio_mode = 0;
        }
        pFlag->NewFile = 1;
      }
      else { /* wrong option */
        PrintUsage(progname);
        ShouldExit = 1;
        pFlag->error = 1;
        pFlag->stdio_mode = 0;
      }
    }
    else
    {
      pFlag->stdio_mode = 0;
      /* not an option */
      if (pFlag->NewFile)
      {
        if (CanSwitchFileMode)
          CanSwitchFileMode = 0;
        else
        {
          RetVal = ConvertDosToUnixNewFile(argv[ArgIdx-1], argv[ArgIdx], pFlag, progname);
          if (pFlag->status & NO_REGFILE)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("Skipping %s, not a regular file.\n"), argv[ArgIdx-1]);
            }
          } else if (pFlag->status & OUTPUTFILE_SYMLINK)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("Skipping %s, output file %s is a symbolic link.\n"), argv[ArgIdx-1], argv[ArgIdx]);
            }
          } else if (pFlag->status & INPUT_TARGET_NO_REGFILE)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("Skipping symbolic link %s, target is not a regular file.\n"), argv[ArgIdx-1]);
            }
          } else if (pFlag->status & OUTPUT_TARGET_NO_REGFILE)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("Skipping %s, target of symbolic link %s is not a regular file.\n"), argv[ArgIdx-1], argv[ArgIdx]);
            }
          } else if (pFlag->status & BINARY_FILE)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("Skipping binary file %s\n"), argv[ArgIdx-1]);
            }
          } else if (pFlag->status & WRONG_CODEPAGE)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("code page %d is not supported.\n"), pFlag->ConvMode);
            }
          } else {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("converting file %s to file %s in Unix format ...\n"), argv[ArgIdx-1], argv[ArgIdx]);
            }
            if (RetVal)
            {
              if (!pFlag->Quiet)
              {
                fprintf(stderr,"%s: ",progname);
                fprintf(stderr, _("problems converting file %s to file %s\n"), argv[ArgIdx-1], argv[ArgIdx]);
              }
            }
          }
          CanSwitchFileMode = 1;
        }
      }
      else
      {
        RetVal = ConvertDosToUnixNewFile(argv[ArgIdx], argv[ArgIdx], pFlag, progname);
        if (pFlag->status & NO_REGFILE)
        {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("Skipping %s, not a regular file.\n"), argv[ArgIdx]);
          }
        } else if (pFlag->status & OUTPUTFILE_SYMLINK)
        {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("Skipping symbolic link %s.\n"), argv[ArgIdx]);
          }
        } else if (pFlag->status & INPUT_TARGET_NO_REGFILE)
        {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("Skipping symbolic link %s, target is not a regular file.\n"), argv[ArgIdx]);
          }
        } else if (pFlag->status & BINARY_FILE)
        {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("Skipping binary file %s\n"), argv[ArgIdx]);
          }
        } else if (pFlag->status & WRONG_CODEPAGE)
        {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("code page %d is not supported.\n"), pFlag->ConvMode);
          }
        } else {
          if (!pFlag->Quiet)
          {
            fprintf(stderr,"%s: ",progname);
            fprintf(stderr, _("converting file %s to Unix format ...\n"), argv[ArgIdx]);
          }
          if (RetVal)
          {
            if (!pFlag->Quiet)
            {
              fprintf(stderr,"%s: ",progname);
              fprintf(stderr, _("problems converting file %s\n"), argv[ArgIdx]);
            }
          }
        }
      }
    }
  }

  /* no file argument, use stdin and stdout */
  if (pFlag->stdio_mode)
  {
    exit(ConvertDosToUnixStdio(pFlag, progname));
  }


  if (!CanSwitchFileMode)
  {
    fprintf(stderr,"%s: ",progname);
    fprintf(stderr, _("target of file %s not specified in new file mode\n"), argv[ArgIdx-1]);
    pFlag->error = 1;
  }
  return (pFlag->error);
}

