/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All UINT32ellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other UINT32ellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any UINT32ellectual Property Assertion or UINT32ellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such UINT32ellectual Property Assertion or UINT32ellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS UINT32ERRUPTION).
#
#
##
*/


#ifndef _CSR_H_
#define _CSR_H_

#define STABLE_TIMER_PERIODIC_EN    (1UL << 62)
#define STABLE_TIMER_EN             (1UL << 61)
#define GSCFG_FLT_EN                (1UL << 7)
#define GSCFG_VLT_EN                (1UL << 6)
#define STABLE_TIMER_CFG            0x1060
#define STABLE_TIMER_TICKS          0x1070

#define LOONGSON_CPUCFG_CONFIG_FIELD2    0x2
#define LOONGSON_CPUCFG_CONFIG_FIELD4    0x4
#define LOONGSON_CPUCFG_CONFIG_FIELD5    0x5

// UINT32
// EFIAPI
// ReadCfg (
//   IN UINT32 Reg
//   );

// UINTN
// EFIAPI
// CsrReadTime (
//   VOID
//   );

// UINT32
// EFIAPI
// CalcConstFreq (
//   VOID
//   );

#define ReadCpucfg(val, reg)                    \
do {                                            \
        UINT64 __res;                           \
        /* cpucfg val, reg */                   \
        __asm__ __volatile__(                   \
                "cpucfg  %0, %1 \n\t"           \
                :"=r"(__res)                    \
                :"r"(reg)                       \
                :                               \
                );                              \
        val = (UINT32)__res;                    \
} while(0)


#define CsrRead32(Sel)                              \
({    UINT32 __Res;                                 \
      __asm__ __volatile__(                         \
              "csrrd\t%0, " #Sel "\n\t"             \
              : "=r" (__Res));                      \
      __Res;                                        \
})

#define CsrRead64(Sel)                              \
({    UINT64 __Res;                                 \
      __asm__ __volatile__(                         \
              "csrrd\t%0, " #Sel "\n\t"             \
              : "=r" (__Res));                      \
      __Res;                                        \
})

#define CsrWrite32(Sel, Value)                      \
do {    volatile UINT32 Val = Value;                \
        __asm__ __volatile__(                       \
            "csrwr\t%0, " #Sel "\n\t"               \
            :                                       \
            : "Jr" (Val)                            \
            : "%0");                                \
} while (0)

#define CsrWrite64(Sel, Value)                      \
do {    volatile UINT64 Val = Value;                \
        __asm__ __volatile__(                       \
            "csrwr\t%0, " #Sel "\n\t"               \
            :                                       \
            : "Jr" (Val)                            \
            : "%0");                                \
} while (0)

#define IoCsrRead8(Sel)                             \
({    UINT8 __Res;                                  \
      __asm__ __volatile__(                         \
              "iocsrrd.b\t%0, \t%1\n\t"             \
              : "=r" (__Res)                        \
              : "r" (Sel));                         \
      __Res;                                        \
})

#define IoCsrRead16(Sel)                            \
({    UINT16 __Res;                                 \
      __asm__ __volatile__(                         \
              "iocsrrd.h\t%0, \t%1\n\t"             \
              : "=r" (__Res)                        \
              : "r" (Sel));                         \
      __Res;                                        \
})

#define IoCsrRead32(Sel)                            \
({    UINT32 __Res;                                 \
      __asm__ __volatile__(                         \
              "iocsrrd.w\t%0, \t%1\n\t"             \
              : "=r" (__Res)                        \
              : "r" (Sel));                         \
      __Res;                                        \
})

#define IoCsrRead64(Sel)                            \
({    UINT64 __Res;                                 \
      __asm__ __volatile__(                         \
              "iocsrrd.d\t%0, \t%1\n\t"             \
              : "=r" (__Res)                        \
              : "r" (Sel));                         \
      __Res;                                        \
})

#define IoCsrWrite8(Sel, Value)                     \
do {                                                \
        __asm__ __volatile__(                       \
            "iocsrwr.b\t%z0, \t%1\n\t"              \
            :                                       \
            : "Jr" (Value), "r" (Sel)               \
            : "memory");                            \
} while (0)

#define IoCsrWrite16(Sel, Value)                    \
do {                                                \
        __asm__ __volatile__(                       \
            "iocsrwr.h\t%z0, \t%1\n\t"              \
            :                                       \
            : "Jr" (Value), "r" (Sel)               \
            : "memory");                            \
} while (0)

#define IoCsrWrite32(Sel, Value)                    \
do {                                                \
        __asm__ __volatile__(                       \
            "iocsrwr.w\t%z0, \t%1\n\t"              \
            :                                       \
            : "Jr" (Value), "r" (Sel)               \
            : "memory");                            \
} while (0)


#define IoCsrWrite64(Sel, Value)                    \
do {                                                \
        __asm__ __volatile__(                       \
            "iocsrwr.d\t%z0, \t%1\n\t"              \
            :                                       \
            : "Jr" (Value), "r" (Sel)               \
            : "memory");                            \
} while (0)
#endif
