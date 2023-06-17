#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "platform/app_debug.h"
#include "platform/app_os.h"

// #define MDEPKG_NDEBUG
#define DebugCodeEnabled() 0

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
          printf("%s-%d-%d\n",__FUNCTION__,__LINE__,Expression);  \
          while(1){}              \
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

/**
  Macro that marks the beginning of debug source code.

  If the DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is set,
  then this macro marks the beginning of source code that is included in a module.
  Otherwise, the source lines between DEBUG_CODE_BEGIN() and DEBUG_CODE_END()
  are not included in a module.

**/
#define DEBUG_CODE_BEGIN()  do { if (DebugCodeEnabled ()) { UINT8  __DebugCodeLocal


/**
  The macro that marks the end of debug source code.

  If the DEBUG_PROPERTY_DEBUG_CODE_ENABLED bit of PcdDebugProperyMask is set,
  then this macro marks the end of source code that is included in a module.
  Otherwise, the source lines between DEBUG_CODE_BEGIN() and DEBUG_CODE_END()
  are not included in a module.

**/
#define DEBUG_CODE_END()    __DebugCodeLocal = 0; __DebugCodeLocal++; } } while (FALSE)

#endif
