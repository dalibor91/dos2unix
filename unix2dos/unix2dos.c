#if defined(MSDOS) || defined(OS2) /* DJGPP, MINGW32 and OS/2 */
#if defined(MSDOS) || defined(OS2)
#ifdef ENABLE_NLS
void PrintLocaledir(char *localedir)
{
  fprintf(stderr, "LOCALEDIR: %s\n", localedir);
}
#endif
#elif defined(MSDOS) || defined(OS2)
  char localedir[1024];
  char *ptr;

   ptr = getenv("UNIX2DOS_LOCALEDIR");
   if (ptr == NULL)
      strcpy(localedir,LOCALEDIR);
   else
   {
      if (strlen(ptr) < sizeof(localedir))
         strcpy(localedir,ptr);
      else
      {
         fprintf(stderr,_("unix2dos: error: Value of environment variable UNIX2DOS_LOCALEDIR is too long.\n"));
         strcpy(localedir,LOCALEDIR);
      }
   }

   bindtextdomain (PACKAGE, localedir);
      {
#ifdef ENABLE_NLS
        PrintLocaledir(localedir);
#endif
      }