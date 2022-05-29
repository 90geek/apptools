#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "platform/app_debug.h"
#include "platform/app_os.h"

#define MDEPKG_NDEBUG 1

#define DebugPrint(n, format, args...)	printf(format, ##args)

#define _DEBUG_PRINT(PrintLevel, ...)              \
do {                                             \
	DebugPrint (PrintLevel, ##__VA_ARGS__);      \
} while (FALSE)
#define _DEBUG(Expression)   _DEBUG_PRINT Expression

/**  
  Macro that calls DebugAssert() if an expression evaluates to FALSE.

  If MDEPKG_NDEBUG is not defined and the DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED 
  bit of PcdDebugProperyMask is set, then this macro evaluates the Boolean 
  expression specified by Expression.  If Expression evaluates to FALSE, then 
  DebugAssert() is called passing in the source filename, source line number, 
  and Expression.

  @param  Expression  Boolean expression.

**/
#if !defined(MDEPKG_NDEBUG)
  #define ASSERT(Expression)        \
    do {                            \
        if (!(Expression)) {        \
          _DEBUG (Expression);     \
		while(1);
        }                           \
    } while (FALSE)
#else
  #define ASSERT(Expression)
#endif

/**  
  Macro that calls DebugPrint().

  If MDEPKG_NDEBUG is not defined and the DEBUG_PROPERTY_DEBUG_PRINT_ENABLED 
  bit of PcdDebugProperyMask is set, then this macro passes Expression to 
  DebugPrint().

  @param  Expression  Expression containing an error level, a format string, 
                      and a variable argument list based on the format string.
  

**/
#if !defined(MDEPKG_NDEBUG)      
  #define DEBUG(Expression)        \
    do {                           \
        _DEBUG (Expression);       \
    } while (FALSE)
#else
  #define DEBUG(Expression)
#endif


/*=============================================================*/

//
// define debug function for loongson
//
#define DEBUG_ENABLE_LOONGSON

#ifdef DEBUG_ENABLE_LOONGSON
#define DbgPrint(n, format, args...) DebugPrint(n, format, ##args)
#else
#define DbgPrint(n, format, args...)
#endif



#ifdef DEBUG_ENABLE_LOONGSON
#define DBGPRINT(Expression) _DEBUG (Expression)
#else
#define DBGPRINT(Expression)
#endif

#endif
