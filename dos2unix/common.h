#  ifndef __MSYS__
#    include <libgen.h>
#  endif
#if (defined(__WATCOMC__) && defined(__NT__))  /* Watcom */
#  define WIN32
#endif

#if defined(WIN32) && !defined(__CYGWIN__) /* Windows */
#if defined(__TURBOC__) || defined(__MSYS__)