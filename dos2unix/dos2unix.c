 *  Copyright (c) 1994-1995 Benjamin Lin.
 *  Copyright (c) 1998 Bernd Johannes Wuebben
 *  Copyright (c) 1998 Christian Wurll
 *  Copyright (c) 2009-2010 Erwin Waterlander
 * Added extra newline if ^M occurs
 * Christian Wurll, wurll@ira.uka.de
 * Thu Nov 19 1998 
 * 
#ifdef MSDOS /* DJGPP */
#include <unistd.h>
#if defined(WIN32) /* MINGW32 */
/* Some compilers have no fchmod().
 * DJGPP, MINGW32 */
#define NO_FCHMOD 1
#endif

#if defined(MSDOS) || defined(__OS2__)
/* Systems without soft links use 'stat' instead of 'lstat'. */
#define STAT stat
#else
#define STAT lstat
#define NO_REGFILE  0x2
/******************************************************************
 *
 * int regfile(char *path)
 *
 * test if *path points to a regular file.
 *
 * returns 0 on success, -1 when it fails.
 *
 ******************************************************************/
int regfile(char *path)
{
   struct stat buf;

   if ((STAT(path, &buf) == 0) && S_ISREG(buf.st_mode))
      return(0);
   else
      return(-1);
}
Copyright (c) 2009-2010 Erwin Waterlander\n\
#ifndef NO_FCHMOD
  mode_t mask;
#endif
  if ((ipFlag->Force == 0) && regfile(ipInFN))
  {
    ipFlag->status |= NO_REGFILE ;
    return -1;
  }
  else
    ipFlag->status = 0 ;

  if (stat(ipInFN, &StatBuf))
#ifndef NO_FCHMOD
  /* preserve original mode as modified by umask */
  mask = umask(0);
  umask(mask);
  if (!RetVal && fchmod(fd, StatBuf.st_mode & ~mask))
    RetVal = -1;
#endif

  if ((ipFlag->Force == 0) && regfile(ipInFN))
  {
    ipFlag->status |= NO_REGFILE ;
    return -1;
  }
  else
    ipFlag->status = 0 ;

#ifndef NO_FCHMOD
          if (pFlag->status & NO_REGFILE)
          {
            if (!pFlag->Quiet)
              fprintf(stderr, _("dos2unix: Skipping %s, not a regular file.\n"), argv[ArgIdx-1]);
          } else if (pFlag->status & BINARY_FILE)
        if (pFlag->status & NO_REGFILE)
        {
          if (!pFlag->Quiet)
            fprintf(stderr, _("dos2unix: Skipping %s, not a regular file.\n"), argv[ArgIdx]);
        } else if (pFlag->status & BINARY_FILE)