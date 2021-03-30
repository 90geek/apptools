#ifndef __APP_TYPE_H__
#define __APP_TYPE_H__

/* Common unsigned types */
#ifndef DEFINED_U8
#define DEFINED_U8
typedef unsigned char  U8;
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
typedef unsigned short U16;
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
typedef unsigned int   U32;
#endif

/* Common signed types */
#ifndef DEFINED_S8
#define DEFINED_S8
typedef signed char  S8;
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
typedef signed short S16;
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
typedef signed int   S32;
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
typedef signed int   S32;
#endif

#ifndef NULL
#define NULL (void *)0
#endif

/* BOOL type constant values */
#ifndef TRUE
    #define TRUE (1 == 1)
#endif

#ifndef FALSE
    #define FALSE (!TRUE)
#endif

#endif
