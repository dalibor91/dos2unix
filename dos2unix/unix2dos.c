 *  Copyright (C) 2009-2011 Erwin Waterlander
#include "querycp.h"
#define WRONG_CODEPAGE  0x4

#define CONVMODE_ASCII  0
#define CONVMODE_7BIT   1
#define CONVMODE_437    437
#define CONVMODE_850    850
#define CONVMODE_860    860
#define CONVMODE_863    863
#define CONVMODE_865    865
#define CONVMODE_1252   1252

#define FROMTO_UNIX2DOS 0
#define FROMTO_UNIX2MAC 1
  int ConvMode;                         /* 0: ascii, 1: 7bit, 2: iso */  
  int FromToMode;                       /* 0: unix2dos, 1: unix2mac */  
Usage: unix2dos [options] [file ...] [-n infile outfile ...]\n\
 -ascii           convert only line breaks (default)\n\
 -iso             conversion between DOS and ISO-8859-1 character set\n\
   -1252          Use Windows code page 1252 (Western European)\n\
   -437           Use DOS code page 437 (US) (default)\n\
   -850           Use DOS code page 850 (Western European)\n\
   -860           Use DOS code page 860 (Portuguese)\n\
   -863           Use DOS code page 863 (French Canadian)\n\
   -865           Use DOS code page 865 (Nordic)\n\
 -7               Convert 8 bit characters to 7 bit space\n\
Copyright (C) 2009-2011 Erwin Waterlander\n\
    int *ConvTable;
    switch (ipFlag->ConvMode)
    {
      case CONVMODE_ASCII: /* ascii */
        ConvTable = U2DAsciiTable;
        break;
      case CONVMODE_7BIT: /* 7bit */
        ConvTable = U2D7BitTable;
        break;
      case CONVMODE_437: /* iso */
        ConvTable = U2DIso437Table;
        break;
      case CONVMODE_850: /* iso */
        ConvTable = U2DIso850Table;
        break;
      case CONVMODE_860: /* iso */
        ConvTable = U2DIso860Table;
        break;
      case CONVMODE_863: /* iso */
        ConvTable = U2DIso863Table;
        break;
      case CONVMODE_865: /* iso */
        ConvTable = U2DIso865Table;
        break;
      case CONVMODE_1252: /* iso */
        ConvTable = U2DIso1252Table;
        break;
      default: /* unknown convmode */
        ipFlag->status |= WRONG_CODEPAGE ;
        return(-1);
    }
    if (ipFlag->ConvMode > 1) /* not ascii or 7bit */
       fprintf(stderr, _("unix2dos: using code page %d.\n"), ipFlag->ConvMode);

    switch (ipFlag->FromToMode)
      case FROMTO_UNIX2DOS: /* unix2dos */
             {
               else
               {
               }
             }
          }
          if (putc(ConvTable[TempChar], ipOutF) == EOF)
      case FROMTO_UNIX2MAC: /* unix2mac */
              if(putc(ConvTable[TempChar], ipOutF) == EOF){
      default: /* unknown FromToMode */
         fprintf(stderr, "%s", _("unix2dos: error: Value of environment variable DOS2UNIX_LOCALEDIR is too long.\n"));
  pFlag->ConvMode = CONVMODE_ASCII;  /* default ascii */
  pFlag->FromToMode = FROMTO_UNIX2DOS;  /* default unix2dos */
    pFlag->FromToMode = FROMTO_UNIX2MAC;
      else if (strcmp(argv[ArgIdx],"-ascii") == 0)  /* SunOS compatible options */
        pFlag->ConvMode = CONVMODE_ASCII;
      else if (strcmp(argv[ArgIdx],"-7") == 0)
        pFlag->ConvMode = CONVMODE_7BIT;
      else if (strcmp(argv[ArgIdx],"-iso") == 0)
      {
        pFlag->ConvMode = (int)query_con_codepage();
        fprintf(stderr,_("unix2dos: active code page: %d\n"), pFlag->ConvMode);
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
          if (strcmpi(argv[ArgIdx],"ascii") == 0)  /* Benjamin Lin's legacy options */
            pFlag->ConvMode = CONVMODE_ASCII;
            pFlag->ConvMode = CONVMODE_7BIT;
          {
            pFlag->ConvMode = (int)query_con_codepage();
            fprintf(stderr,_("unix2dos: active code page: %d\n"), pFlag->ConvMode);
            if (pFlag->ConvMode < 2)
               pFlag->ConvMode = CONVMODE_437;
          }
            pFlag->FromToMode = FROMTO_UNIX2MAC;
          } else if (pFlag->status & WRONG_CODEPAGE)
          {
            if (!pFlag->Quiet)
              fprintf(stderr, _("unix2dos: code page %d is not supported.\n"), pFlag->ConvMode);
              if (pFlag->FromToMode == FROMTO_UNIX2MAC)
                fprintf(stderr, _("unix2dos: converting file %s to file %s in Mac format ...\n"), argv[ArgIdx-1], argv[ArgIdx]);
        } else if (pFlag->status & WRONG_CODEPAGE)
        {
          if (!pFlag->Quiet)
            fprintf(stderr, _("unix2dos: code page %d is not supported.\n"), pFlag->ConvMode);
            if (pFlag->FromToMode == FROMTO_UNIX2MAC)
              fprintf(stderr, _("unix2dos: converting file %s to Mac format ...\n"), argv[ArgIdx]);