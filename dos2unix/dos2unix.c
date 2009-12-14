 *  Copyright (c) 1998, Bernd Johannes Wuebben
 *  Copyright (c) 1998, Christian Wurll
 *  Copyright (c) 2009  Erwin Waterlander
 * Added extra newline if ^M occurs
 * Christian Wurll, wurll@ira.uka.de
 * Thu Nov 19 1998 
 * 
 * Added Mac text file translation, i.e. \r to \n conversion
 * Bernd Johannes Wuebben, wuebben@kde.org
 * Wed Feb  4 19:12:58 EST 1998      
 *
 *  See ChangeLog.txt for complete version history.
 *
#ifdef DJGPP
#  include <unistd.h>
#else
#  include <libgen.h>
#  include <sys/unistd.h>
#if defined(WIN32)
#define MSDOS
#endif

#ifdef MSDOS /* DJGPP and MINGW32 */
#include <fcntl.h>
#include <io.h>
#endif

#ifdef MSDOS
#ifdef ENABLE_NLS
  fprintf(stderr, _("With native language support.\n"));
#else
  fprintf(stderr, "Without native language support.\n");
#endif
  /* CurChar is always CR (x0d) */
  /* In normal dos2unix mode put nothing (skip CR). */
    putc('\x0a', ipOutF);
    /* CR-LF -> LF */
    /* LF    -> LF, in case the input file is a Unix text file */
    /* CR    -> CR, in dos2unix mode (don't modify Mac file) */
    /* CR    -> LF, in Mac mode */
    /* \x0a = Newline/Line Feed (LF) */
    /* \x0d = Carriage Return (CR) */

      case 0: /* ASCII */
        while ((TempChar = getc(ipInF)) != EOF) {
          if (TempChar != '\x0d') {
            if (putc(D2UAsciiTable[TempChar], ipOutF) == EOF) {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("dos2unix: can not write to out file\n"));
              break;
            } 
          } else {
            StripDelimiter( ipInF, ipOutF, ipFlag, TempChar );
          }
        }
        break;
      case 1: /* 7Bit */
        while ((TempChar = getc(ipInF)) != EOF) {
          if (TempChar != '\x0d') {
            if (putc(D2U7BitTable[TempChar], ipOutF) == EOF) {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("dos2unix: can not write to out file\n"));
              break;
            }
          } else {
            StripDelimiter( ipInF, ipOutF, ipFlag, TempChar );
          }
        }
        break;
      case 2: /* ISO */
        while ((TempChar = getc(ipInF)) != EOF) {
          if (TempChar != '\x0d') {
            if (putc(D2UIsoTable[TempChar], ipOutF) == EOF) {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("dos2unix: can not write to out file\n"));
              break;
            }
          } else {
            StripDelimiter( ipInF, ipOutF, ipFlag, TempChar );
          }
        }
        break;
      case 3: /* Mac */
        while ((TempChar = getc(ipInF)) != EOF)
          if ((TempChar != '\x0d'))
            {
              if(putc(D2UAsciiTable[TempChar], ipOutF) == EOF){
                RetVal = -1;
                if (!ipFlag->Quiet)
                  fprintf(stderr, _("dos2unix: can not write to out file\n"));
                break;
              }
            }
          else{
            if ( (TempNextChar = getc(ipInF)) != EOF) {
              ungetc( TempNextChar, ipInF );  /* put back peek char */
              /* Don't touch this delimiter if it's a CR,LF pair. */
              if ( TempNextChar == '\x0a' ) {
                continue;
              }
            }
            if (putc('\x0a', ipOutF) == EOF)
              {
                RetVal = -1;
                if (!ipFlag->Quiet)
                  fprintf(stderr, _("dos2unix: can not write to out file\n"));
                break;
              }
          }
        break;
      default: /* unknown convmode */
#ifdef MSDOS
FILE* MakeTempFileFrom(const char *OutFN, char **fname_ret)
#else
#endif
#ifdef MSDOS
  char *name;
  FILE *fd = NULL;
#else
#endif
#ifdef MSDOS
  name = mktemp(fname_str);
  *fname_ret = name;
  if ((fd = fopen(fname_str, W_CNTRL)) == NULL)
    goto make_failed;
#else
#endif
#ifdef MSDOS
  return (NULL);
#else
#endif
#ifdef MSDOS
  FILE* fd;
#else
#endif
#ifdef MSDOS
  if((fd = MakeTempFileFrom(ipOutFN, &TempPath))==NULL) {
#else
#endif
          perror("Failed to open output temp file");
          RetVal = -1;
#ifdef MSDOS
  if ((!RetVal) && (InF) && ((TempF=fd) == NULL))
#else
#endif
#ifdef MSDOS
  if(fd!=NULL)
    fclose(fd);
#else
    close(fd);
#endif
  if ((RetVal) && (remove(TempPath)))
#ifdef MSDOS
    remove(ipOutFN);
#endif
#ifdef MSDOS
  FILE* fd;
#else
#endif
#ifdef MSDOS
  if((fd = MakeTempFileFrom(ipInFN, &TempPath))==NULL) {
#else
#endif
    perror("Failed to open output temp file");
    RetVal = -1;
#ifndef MSDOS
#endif
#ifdef MSDOS
  if ((!RetVal) && (InF) && ((TempF=fd) == NULL))
#else
#endif
#ifdef MSDOS
  if(fd!=NULL)
    fclose(fd);
#else
    close(fd);
#endif
  if ((RetVal) && (remove(TempPath)))
#ifdef MSDOS
  if (!RetVal)
    remove(ipInFN);
#endif

    /* stdin and stdout are by default text streams. We need
     * to reopen them in binary mode. Otherwise a LF will
     * automatically be converted to CR-LF on DOS/Windows.
     * Erwin */

#ifdef WIN32
    /* 'setmode' was deprecated by MicroSoft, starting
     * since Visual C++ 2005. Use '_setmode' instead. */
    _setmode(fileno(stdout), O_BINARY);
    _setmode(fileno(stdin), O_BINARY);
#elif defined(MSDOS)
    setmode(fileno(stdout), O_BINARY);
    setmode(fileno(stdin), O_BINARY);
    return (ConvertDosToUnix(stdin, stdout, ipFlag));
#else
    return (ConvertDosToUnix(stdin, stdout, ipFlag));
#endif