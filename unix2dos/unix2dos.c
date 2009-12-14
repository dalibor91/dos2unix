 *  Copyright (c) 2009 Erwin Waterlander
 *  See ChangeLog.txt for complete version history.
#define VER_AUTHOR   "Erwin Waterlander"
#ifdef DJGPP
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
    /* LF    -> CR-LF */
    /* CR-LF -> CR-LF, in case the input file is a DOS text file */
      case 0: /* ASCII */
        while ((TempChar = getc(ipInF)) != EOF)  /* get character */
          if (
              /* read a Unix line end */
              ((TempChar == '\x0a') && (putc('\x0d', ipOutF) == EOF)) ||  /* got LF, put CR */
              /* read a DOS line end */
              ((TempChar == '\x0d') && (((TempChar = getc(ipInF)) == EOF) || (putc('\x0d', ipOutF) == EOF))) || /* got CR, put CR and get next char (LF) */
              (putc(U2DAsciiTable[TempChar], ipOutF) == EOF)) /* put char (LF or other char) */
          {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("unix2dos: can not write to output file\n"));
              break;
          }
        break;
        while ((TempChar = getc(ipInF)) != EOF)
          if (((TempChar == '\x0a') && (putc('\x0d', ipOutF) == EOF)) ||
              ((TempChar == '\x0d') && (((TempChar = getc(ipInF)) == EOF) || (putc('\x0d', ipOutF) == EOF))) ||
              (putc(U2D7BitTable[TempChar], ipOutF) == EOF))
          {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("unix2dos: can not write to output file\n"));
              break;
          }
        break;
        while ((TempChar = getc(ipInF)) != EOF)
          if (((TempChar == '\x0a') && (putc('\x0d', ipOutF) == EOF)) ||
              ((TempChar == '\x0d') && (((TempChar = getc(ipInF)) == EOF) || (putc('\x0d', ipOutF) == EOF))) ||
              (putc(U2DIsoTable[TempChar], ipOutF) == EOF))
          {
              RetVal = -1;
              if (!ipFlag->Quiet)
                fprintf(stderr, _("unix2dos: can not write to output file\n"));
              break;
          }
        break;
      ;
      fprintf(stderr, _("unix2dos: program error, invalid conversion mode %d\n"),ipFlag->ConvMode);
      exit(1);
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
    perror("Can't open output temp file");
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
    return (ConvertUnixToDos(stdin, stdout, ipFlag));
#elif defined(MSDOS)
    setmode(fileno(stdout), O_BINARY);
    setmode(fileno(stdin), O_BINARY);
#else
    return (ConvertUnixToDos(stdin, stdout, ipFlag));
#endif