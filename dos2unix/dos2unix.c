#ifdef MSDOS
/* Some compilers have no mkstemp().
 * Use mktemp() instead.
 * DJGPP, MINGW32 */
#define NO_MKSTEMP 1
#endif

#if defined(MSDOS) || defined(__OS2__)
/* On some systems rename() will always fail if target file already exists. */
#define NEED_REMOVE 1
#endif

#if defined(MSDOS) || defined(__OS2__) /* DJGPP, MINGW32 and OS/2 */
/* required for setmode() and O_BINARY */
#if defined(MSDOS) || defined(__OS2__)
#define BINARY_FILE 0x1
  int Force;                            /* if TRUE, force conversion of all files. */
  int status;
Usage: dos2unix [-fhkLlqV] [-c convmode] [-o file ...] [-n infile outfile ...]\n\
 -c --convmode    conversion mode\n\
   convmode       ASCII, 7bit, ISO, Mac, default to ASCII\n\
 -f --force       force conversion of all files\n\
   infile         original file in new file mode\n\
   outfile        output file in new file mode\n\
 -o --oldfile     write to old file\n\
   file ...       files to convert in old file mode\n\
 -q --quiet       quiet mode, suppress all warnings\n\
                  always on in stdio mode\n\
 -V --version     display version number\n"), VER_REVISION, VER_DATE);
    ipFlag->status = 0;

          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09')) {  /* Not a TAB */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09')) {  /* Not a TAB */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09')) {  /* Not a TAB */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
        while ((TempChar = getc(ipInF)) != EOF) {
          if ((ipFlag->Force == 0) &&
              (TempChar < 32) &&
              (TempChar != '\x0a') &&  /* Not an LF */
              (TempChar != '\x0d') &&  /* Not a CR */
              (TempChar != '\x09')) {  /* Not a TAB */
            RetVal = -1;
            ipFlag->status |= BINARY_FILE ;
            break;
          }
        }
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NEED_REMOVE
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NO_MKSTEMP
#ifdef NEED_REMOVE
    ipFlag->Force = 1;
#elif defined(MSDOS) || defined(__OS2__)
  int RetVal = 0;
  pFlag->Force = 0;
  pFlag->status = 0;
      if ((strcmp(argv[ArgIdx],"-f") == 0) || (strcmp(argv[ArgIdx],"--force") == 0))
        pFlag->Force = 1;
          RetVal = ConvertDosToUnixNewFile(argv[ArgIdx-1], argv[ArgIdx], pFlag);
          if (pFlag->status & BINARY_FILE)
              fprintf(stderr, _("dos2unix: Skipping binary file %s\n"), argv[ArgIdx-1]);
          } else {
            if (!pFlag->Quiet)
              fprintf(stderr, _("dos2unix: converting file %s to file %s in UNIX format ...\n"), argv[ArgIdx-1], argv[ArgIdx]);
            if (RetVal)
            {
              if (!pFlag->Quiet)
                fprintf(stderr, _("dos2unix: problems converting file %s to file %s\n"), argv[ArgIdx-1], argv[ArgIdx]);
              ShouldExit = 1;
            }
        RetVal = ConvertDosToUnixOldFile(argv[ArgIdx], pFlag);
        if (pFlag->status & BINARY_FILE)
            fprintf(stderr, _("dos2unix: Skipping binary file %s\n"), argv[ArgIdx]);
        } else {
          if (!pFlag->Quiet)
            fprintf(stderr, _("dos2unix: converting file %s to UNIX format ...\n"), argv[ArgIdx]);
          if (RetVal)
          {
            if (!pFlag->Quiet)
              fprintf(stderr, _("dos2unix: problems converting file %s\n"), argv[ArgIdx]);
            ShouldExit = 1;
          }