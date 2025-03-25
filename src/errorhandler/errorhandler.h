#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__
#include <assert.h>

void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function);

#define __ASSERT_FUNCTION	__func__

//"The "impossible" has happened - notify the user, then lie down and die"
#ifdef NDEBUG
   #undef assert
   #define assert(expr) \
  ((void) sizeof((expr) ? 1 : 0), __extension__ ({			\
      if (expr)								\
        ; /* empty */							\
      else								\
        __assert_fail (#expr, __FILE_NAME__, __LINE__, __ASSERT_FUNCTION);	\
    }))
#endif

#define try(expr) \
({puts("try Not implemented yet");})

#define catch(expr) \
({puts("catch Not implemented yet");})

#endif //__ERRORHANDLER_H__
