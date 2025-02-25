/**@file

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson Technology
  Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

**/

#include "loongson/LsRegDef.h"
#include "edk_api.h"

#define AVS_BASE PHYS_TO_UNCACHED(0x1fe00160)
#define AVS_CSR  0
#define AVS_MREG 0x4
#define AVS_SREG 0x8

/* p: 1    n: 0*/
VOID
AvsSetVol (
  UINT64 TotNode,
  INTN RailSel,
  INTN Vol,
  INTN RxDelay,
  INTN ClkDiv
  )
{
    UINT64 Base ;
    void * vaddr = NULL;
    int memoffset=0;
    vaddr=p2v_mem_mapping(AVS_BASE | (TotNode << NODE_OFFSET),64, &memoffset);
    if(vaddr==NULL)
      return EFI_LOAD_ERROR;
    Base = (UINT64)vaddr;
    if ((Vol < 600) || (Vol > 1300)) {
      printf("\r\nAVS: Set Vol range error!!!\r\n");
      return;
    }

    Writel(Base | AVS_CSR, 0x10000 | (ClkDiv << 17) | (0x7 << 25) | (RxDelay << 20));
    Writel(Base | AVS_MREG, 0x80000000 | (Vol << 4) | (RailSel << 20));

    while (Readl(Base | AVS_SREG) & 0x80000000);

    if ((Readl(Base | AVS_SREG) & 0x60000000)) {
      printf("set avs_Vol erro!\r\n");
    }
    p2v_mem_clean(vaddr, memoffset);
}

/* p: 1    n: 0*/
RETURN_STATUS
AvsGetVol (
  UINT64 TotNode,
  INTN RailSel,
  INTN RxDelay,
  INTN ClkDiv
  )
{
    UINT64 Base;
    void * vaddr = NULL;
    int memoffset=0;
    vaddr=p2v_mem_mapping(AVS_BASE | (TotNode << NODE_OFFSET),64, &memoffset);
    if(vaddr==NULL)
      return EFI_LOAD_ERROR;
    Base = (UINT64)vaddr;
    Writel(Base | AVS_CSR, 0x10000 | (ClkDiv << 17) | (0x7 << 25) | (RxDelay << 20));
    Writel(Base | AVS_MREG, 0xe0000000 | (RailSel << 20));

    while (Readl(Base | AVS_SREG) & 0x80000000);

    if ((Readl(Base | AVS_SREG) & 0x60000000))
      return 0;
    else
      return (Readl(Base | AVS_SREG) & 0xffff);
    p2v_mem_clean(vaddr, memoffset);

  return 0;
}

/* p: 1    n: 0*/
VOID
AvsVolPrint (
  UINT64 TotNode
  )
{
  UINT32 Val;
  Val = AvsGetVol(TotNode, 0, 0, 4);
  if (!Val) {
    printf("AVS: Get Vddn error!\r\n");
  } else {
    printf("AVS: Get Vddn value is: 0x%x",Val);
    printf("\r\n");
  }
  Val = AvsGetVol(TotNode, 1, 0, 4);
  if (!Val) {
    printf("AVS: Get Vddp error!\r\n");
  } else {
    printf("AVS: Get Vddp value is: 0x%x",Val);
    printf("\r\n");
  }
}

VOID
AvsAdjustVol (
  UINT64 TotNode,
  INTN Vddn,
  INTN Vddp
  )
{
  /* set vddn */
  AvsSetVol(TotNode, 0, Vddn, 0, 4);
  /* set vddp */
  AvsSetVol(TotNode, 1, Vddp, 0, 4);
  AvsVolPrint(TotNode);
}
