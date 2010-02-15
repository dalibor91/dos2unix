#include <fnmatch.h>
  int ConvMode;                         /* 0: ascii, 1: 7bit, 2: iso, 3: mac */  
   convmode       ascii, 7bit, iso, mac, default to ascii\n\
void AddDOSNewLine(FILE* ipInF, FILE* ipOutF, CFlag *ipFlag, int CurChar, int PrevChar)
  if (ipFlag->NewLine) {  /* add additional CR-LF? */
    /* Don't add line ending if it is a DOS line ending. Only in case of Unix line ending. */
    if ((CurChar == '\x0a') && (PrevChar != '\x0d')) {
    int PreviousChar = 0;
      case 0: /* ascii */
          if (TempChar == '\x0a')
	  {
            putc('\x0d', ipOutF); /* got LF, put CR */
	  } else {
             if (TempChar == '\x0d') /* got CR */
	     {
               if ((TempChar = getc(ipInF)) == EOF) /* get next char */
                 TempChar = '\x0d';  /* Read error, or end of file. */
	       else
	       {
                 putc('\x0d', ipOutF); /* put CR */
                 PreviousChar = '\x0d';
	       }
	     }
	  }
          if (putc(U2DAsciiTable[TempChar], ipOutF) == EOF)
            AddDOSNewLine( ipInF, ipOutF, ipFlag, TempChar, PreviousChar);
          PreviousChar = TempChar;
      case 1: /* 7bit */
          if (TempChar == '\x0a')
	  {
            putc('\x0d', ipOutF); /* got LF, put CR */
	  } else {
             if (TempChar == '\x0d') /* got CR */
	     {
               if ((TempChar = getc(ipInF)) == EOF) /* get next char */
                 TempChar = '\x0d';  /* Read error, or end of file. */
	       else
	       {
                 putc('\x0d', ipOutF); /* put CR */
                 PreviousChar = '\x0d';
	       }
	     }
	  }
          if (putc(U2D7BitTable[TempChar], ipOutF) == EOF)
            AddDOSNewLine( ipInF, ipOutF, ipFlag, TempChar, PreviousChar);
          PreviousChar = TempChar;
      case 2: /* iso */
          if (TempChar == '\x0a')
	  {
            putc('\x0d', ipOutF); /* got LF, put CR */
	  } else {
             if (TempChar == '\x0d') /* got CR */
	     {
               if ((TempChar = getc(ipInF)) == EOF) /* get next char */
                 TempChar = '\x0d';  /* Read error, or end of file. */
	       else
	       {
                 putc('\x0d', ipOutF); /* put CR */
                 PreviousChar = '\x0d';
	       }
	     }
	  }
          if (putc(U2DIsoTable[TempChar], ipOutF) == EOF)
            AddDOSNewLine( ipInF, ipOutF, ipFlag, TempChar, PreviousChar);
          }
          PreviousChar = TempChar;
        }
        break;
      case 3: /* Mac */
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
          if ((TempChar != '\x0a')) /* Not an LF */
            {
              if(putc(U2DAsciiTable[TempChar], ipOutF) == EOF){
                RetVal = -1;
                if (!ipFlag->Quiet)
                  fprintf(stderr, _("unix2dos: can not write to output file\n"));
                break;
              }
              PreviousChar = TempChar;
            }
          else{
            /* TempChar is an LF */
            /* Don't touch this delimiter if it's a CR,LF pair. */
            if ( PreviousChar == '\x0d' ) {
              if (putc('\x0a', ipOutF) == EOF)  /* CR,LF pair. Put LF */
                {
                  RetVal = -1;
                  if (!ipFlag->Quiet)
                    fprintf(stderr, _("unix2dos: can not write to output file\n"));
                  break;
                }
              PreviousChar = TempChar;
              continue;
            }
            PreviousChar = TempChar;
            if (putc('\x0d', ipOutF) == EOF) /* Unix line end (LF). Put CR */
              {
                RetVal = -1;
                if (!ipFlag->Quiet)
                  fprintf(stderr, _("unix2dos: can not write to output file\n"));
                break;
              }
            if (ipFlag->NewLine) {  /* add additional CR? */
              putc('\x0d', ipOutF);
            }
   ptr = getenv("DOS2UNIX_LOCALEDIR");
  if (fnmatch("*unix2mac", argv[0], FNM_CASEFOLD) == 0)
    pFlag->ConvMode = 3;

          if (strcmpi(argv[ArgIdx],"ascii") == 0)
          else if (strcmpi(argv[ArgIdx], "7bit") == 0)
          else if (strcmpi(argv[ArgIdx], "iso") == 0)
          else if (strcmpi(argv[ArgIdx], "mac") == 0)
            pFlag->ConvMode = 3;
            {
              if (pFlag->ConvMode == 3)
                fprintf(stderr, _("unix2dos: converting file %s to file %s in MAC format ...\n"), argv[ArgIdx-1], argv[ArgIdx]);
              else
                fprintf(stderr, _("unix2dos: converting file %s to file %s in DOS format ...\n"), argv[ArgIdx-1], argv[ArgIdx]);
            }
          {
            if (pFlag->ConvMode == 3)
              fprintf(stderr, _("unix2dos: converting file %s to MAC format ...\n"), argv[ArgIdx]);
            else
              fprintf(stderr, _("unix2dos: converting file %s to DOS format ...\n"), argv[ArgIdx]);
          }