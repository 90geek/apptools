/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/

#ifndef __LS_REG_DEF_H__
#define __LS_REG_DEF_H__


//
// loongarch memory map definition
//
#define ARCH 3
#if (ARCH==1)
#define CACHED_MEMORY_ADDR         0x9000000000000000
#define UNCACHED_MEMORY_ADDR       0x8000000000000000
#elif (ARCH==2) // mips64el
#define CACHED_MEMORY_ADDR         0x9800000000000000
#define UNCACHED_MEMORY_ADDR       0x9000000000000000
#else
#define CACHED_MEMORY_ADDR         0x0000000000000000
#define UNCACHED_MEMORY_ADDR       0x0000000000000000
#endif

#define PHYS_TO_CACHED(x)       ((UINT64)CACHED_MEMORY_ADDR | (x))
#define PHYS_TO_UNCACHED(x)     ((UINT64)UNCACHED_MEMORY_ADDR | (x))
#define CACHED_TO_UNCACHED(x)   (PHYS_TO_UNCACHED(VA_TO_PHYS(x)))
#define UNCACHED_TO_CACHED(x)   (PHYS_TO_CACHED(VA_TO_PHYS(x)))
#define VA_TO_PHYS(x)           ((UINT64)(x) & 0xffffffffffffULL)

/*****************************************************************************
 *                                cpu                                     *
 *****************************************************************************/

#define CPU_TEMP_SAMPLE_BASE  PHYS_TO_UNCACHED(0x1fe00198)  //Temperature sampling register
#define TEMP_SENSOR_VALUE_OFFSET    0x4

/*****************************************************************************
 *                                Common                                     *
 *****************************************************************************/
#define HT_CONF_TYPE0_ADDR              PHYS_TO_UNCACHED(0xefdfe000000)
#define HT_CONF_TYPE1_ADDR              PHYS_TO_UNCACHED(0xefdff000000)
#define HT1_MEM_BASE_ADDR               PHYS_TO_UNCACHED(0xe0000000000)
#define HT1_CONTROLLER_CONF_BASE_ADDR   PHYS_TO_UNCACHED(0xefdfb000000)
#define CONFBUS_BASE_ADDR               0x10010000
#define INT_BASE_ADDR                   0x10000000
#define HPET_BASE_ADDR                  0x10001000
#define NODE_OFFSET                     44
#define LS7A_CONFBUS_BASE_ADDR          (HT1_MEM_BASE_ADDR | CONFBUS_BASE_ADDR)

#define CONF_HT_CLKEN_OFFSET            0x418

#define CONF_HT_ROUTE_OFFSET            0x41c
#define CONF_HT_ROUTE_ADDR              (LS7A_CONFBUS_BASE_ADDR | CONF_HT_ROUTE_OFFSET)

#define LS7A_NOR_CONF0_OFFSET           0x420
#define LS7A_NOR_CONF0_ADDR             (LS7A_CONFBUS_BASE_ADDR | LS7A_NOR_CONF0_OFFSET)

#define LS7A_NOR_CONF1_OFFSET           0x430
#define LS7A_NOR_CONF1_ADDR             (LS7A_CONFBUS_BASE_ADDR | LS7A_NOR_CONF1_OFFSET)

#define CONF_PAD_OFFSET                 0x438

#define CONF_REUSE_OFFSET               0x440

#define HT_CFG_HEADER_ADDR(dev, fun)    (HT_CONF_TYPE0_ADDR | (dev << 11) | (fun << 8))

#define LS7A_VERSION   (!(Readb(PHYS_TO_UNCACHED(0xefdfe000108)) == 0))
//
// PCIE configuration space definitions
//
#define PCIE_TYPE0_VID                  0x0
#define PCIE_TYPE0_DID                  0x2
#define PCIE_TYPE0_CMD                  0x4
#define PCIE_TYPE0_STS                  0x6
#define PCIE_TYPE0_REV                  0x8
#define PCIE_TYPE0_CLASS                0x9
#define PCIE_TYPE0_BAR0                 0x10
#define PCIE_TYPE0_BAR1                 0x14
#define PCIE_TYPE0_BAR2                 0x18
#define PCIE_TYPE0_BAR3                 0x1c
#define PCIE_TYPE0_BAR4                 0x20
#define PCIE_TYPE0_BAR5                 0x24
#define PCIE_TYPE0_SUB_SYS_VID          0x2c
#define PCIE_TYPE0_SUB_SYS_ID           0x2e

/*****************************************************************************
 *                                  Pll                                      *
 *****************************************************************************/
#define CONF_PLL0_OFFSET                0x480
#define CONF_PLL0_ADDR                  (LS7A_CONFBUS_BASE_ADDR | CONF_PLL0_OFFSET)
#define CONF_PLL1_OFFSET                0x490
#define CONF_PLL1_ADDR                  (LS7A_CONFBUS_BASE_ADDR | CONF_PLL1_OFFSET)
#define CONF_PLL2_OFFSET                0x4a0
#define CONF_PLL2_ADDR                  (LS7A_CONFBUS_BASE_ADDR | CONF_PLL2_OFFSET)
#define CONF_PLL3_OFFSET                0x4b0
#define CONF_PLL3_ADDR                  (LS7A_CONFBUS_BASE_ADDR | CONF_PLL3_OFFSET)
#define CONF_PLL4_OFFSET                0x4c0
#define CONF_PLL4_ADDR                  (LS7A_CONFBUS_BASE_ADDR | CONF_PLL4_OFFSET)
#define LS7A_PLL_DIV_REFC_OFFSET        0
#define LS7A_PLL_LOCK_OFFSET            7
#define LS7A_PLL_SEL0_OFFSET            8
#define LS7A_PLL_SEL1_OFFSET            9
#define LS7A_PLL_SEL2_OFFSET            10
#define LS7A_PLL_SET_OFFSET             11
#define LS7A_PLL_BYPASS_OFFSET          12
#define LS7A_PLL_PD_OFFSET              13
#define LS7A_PLL_DIV0_OFFSET            0
#define LS7A_PLL_DIV1_OFFSET            7
#define LS7A_PLL_DIV2_OFFSET            14
#define LS7A_PLL_LOOPC_OFFSET           21

#define LS7A_PLL_VALUE(LOOPC, DIV2, DIV1, DIV0) (\
  (LOOPC << LS7A_PLL_LOOPC_OFFSET) |             \
  (DIV2 << LS7A_PLL_DIV2_OFFSET)   |             \
  (DIV1 << LS7A_PLL_DIV1_OFFSET)   |             \
  (DIV0 << LS7A_PLL_DIV0_OFFSET)                 \
  )

/*****************************************************************************
 *                                  Misc                                     *
 *****************************************************************************/
#define MISC_BASE_ADDR                  0x10080000
#define LS7A_MISC_BASE                  (HT1_MEM_BASE_ADDR | MISC_BASE_ADDR)

/*****************************************************************************
 *                                  RTC                                      *
 *****************************************************************************/
#define LS7A_RTC_REG_OFFSE              0x50100
#define LS7A_RTC_REG_BASE               (LS7A_MISC_BASE + LS7A_RTC_REG_OFFSE)

/*****************************************************************************
 *                              Pcie-Common                                  *
 *****************************************************************************/
#define PCIE_MAX_SIZE   12
#define PCIE_PD_LOOP    20

/*****************************************************************************
 *                                Pcie-F0                                    *
 *****************************************************************************/
#define PCIE_F0_REG0_OFFSET             0x580
#define PCIE_F0_REG0_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_F0_REG0_OFFSET)

#define PCIE_F0_REG1_OFFSET             0x584
#define PCIE_F0_REG1_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_F0_REG1_OFFSET)

#define PCIE_F0_PHY_CONF_OFFSET         0x588
#define PCIE_F0_PHY_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | PCIE_F0_PHY_CONF_OFFSET)

#define PCIE_F0_PHY_ACCESS_OFFSET       0x590
#define PCIE_F0_PHY_ACCESS_ADDR         (LS7A_CONFBUS_BASE_ADDR | PCIE_F0_PHY_ACCESS_OFFSET)

/*****************************************************************************
 *                                Pcie-F1                                    *
 *****************************************************************************/
#define PCIE_F1_REG0_OFFSET             0x5a0
#define PCIE_F1_REG0_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_F1_REG0_OFFSET)

#define PCIE_F1_REG1_OFFSET             0x5a4
#define PCIE_F1_REG1_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_F1_REG1_OFFSET)

#define PCIE_F1_PHY_CONF_OFFSET         0x5a8
#define PCIE_F1_PHY_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | PCIE_F1_PHY_CONF_OFFSET)

#define PCIE_F1_PHY_ACCESS_OFFSET       0x5b0
#define PCIE_F1_PHY_ACCESS_ADDR         (LS7A_CONFBUS_BASE_ADDR | PCIE_F1_PHY_ACCESS_OFFSET)

/*****************************************************************************
 *                                 Pcie-H                                    *
 *****************************************************************************/
#define PCIE_H_REG0_OFFSET              0x5c0
#define PCIE_H_REG0_ADDR                (LS7A_CONFBUS_BASE_ADDR | PCIE_H_REG0_OFFSET)

#define PCIE_H_REG1_OFFSET              0x5c4
#define PCIE_H_REG1_ADDR                (LS7A_CONFBUS_BASE_ADDR | PCIE_H_REG1_OFFSET)

#define PCIE_H_PHY_CONF_OFFSET          0x5c8
#define PCIE_H_PHY_CONF_ADDR            (LS7A_CONFBUS_BASE_ADDR | PCIE_H_PHY_CONF_OFFSET)

#define PCIE_H_PHY_LOW_ACCESS_OFFSET    0x5d0
#define PCIE_H_PHY_LOW_ACCESS_ADDR      (LS7A_CONFBUS_BASE_ADDR | PCIE_H_PHY_LOW_ACCESS_OFFSET)

#define PCIE_H_PHY_HI_ACCESS_OFFSET     0x5d8
#define PCIE_H_PHY_HI_ACCESS_ADDR       (LS7A_CONFBUS_BASE_ADDR | PCIE_H_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                Pcie-G0                                    *
 *****************************************************************************/
#define PCIE_G0_REG0_OFFSET             0x5e0
#define PCIE_G0_REG0_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_G0_REG0_OFFSET)

#define PCIE_G0_REG1_OFFSET             0x5e4
#define PCIE_G0_REG1_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_G0_REG1_OFFSET)

#define PCIE_G0_PHY_CONF_OFFSET         0x5e8
#define PCIE_G0_PHY_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | PCIE_G0_PHY_CONF_OFFSET)

#define PCIE_G0_PHY_LOW_ACCESS_OFFSET   0x5f0
#define PCIE_G0_PHY_LOW_ACCESS_ADDR     (LS7A_CONFBUS_BASE_ADDR | PCIE_G0_PHY_LOW_ACCESS_OFFSET)

#define PCIE_G0_PHY_HI_ACCESS_OFFSET    0x5f8
#define PCIE_G0_PHY_HI_ACCESS_ADDR      (LS7A_CONFBUS_BASE_ADDR | PCIE_G0_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                Pcie-G1                                    *
 *****************************************************************************/
#define PCIE_G1_REG0_OFFSET             0x600
#define PCIE_G1_REG0_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_G1_REG0_OFFSET)

#define PCIE_G1_REG1_OFFSET             0x604
#define PCIE_G1_REG1_ADDR               (LS7A_CONFBUS_BASE_ADDR | PCIE_G1_REG1_OFFSET)

#define PCIE_G1_PHY_CONF_OFFSET         0x608
#define PCIE_G1_PHY_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | PCIE_G1_PHY_CONF_OFFSET)

#define PCIE_G1_PHY_LOW_ACCESS_OFFSET   0x610
#define PCIE_G1_PHY_LOW_ACCESS_ADDR     (LS7A_CONFBUS_BASE_ADDR | PCIE_G1_PHY_LOW_ACCESS_OFFSET)

#define PCIE_G1_PHY_HI_ACCESS_OFFSET    0x618
#define PCIE_G1_PHY_HI_ACCESS_ADDR      (LS7A_CONFBUS_BASE_ADDR | PCIE_G1_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                 SATA-0                                    *
 *****************************************************************************/
#define SATA0_PHY_CONF_OFFSET           0x740
#define SATA0_PHY_CONF_ADDR             (LS7A_CONFBUS_BASE_ADDR | SATA0_PHY_CONF_OFFSET)

#define SATA0_PHY_LOW_ACCESS_OFFSET     0x748
#define SATA0_PHY_LOW_ACCESS_ADDR       (LS7A_CONFBUS_BASE_ADDR | SATA0_PHY_LOW_ACCESS_OFFSET)

#define SATA0_PHY_HI_ACCESS_OFFSET      0x74c
#define SATA0_PHY_HI_ACCESS_ADDR        (LS7A_CONFBUS_BASE_ADDR | SATA0_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                 SATA-1                                    *
 *****************************************************************************/
#define SATA1_PHY_CONF_OFFSET           0x750
#define SATA1_PHY_CONF_ADDR             (LS7A_CONFBUS_BASE_ADDR | SATA1_PHY_CONF_OFFSET)

#define SATA1_PHY_LOW_ACCESS_OFFSET     0x758
#define SATA1_PHY_LOW_ACCESS_ADDR       (LS7A_CONFBUS_BASE_ADDR | SATA1_PHY_LOW_ACCESS_OFFSET)

#define SATA1_PHY_HI_ACCESS_OFFSET      0x75c
#define SATA1_PHY_HI_ACCESS_ADDR        (LS7A_CONFBUS_BASE_ADDR | SATA1_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                 SATA-2                                    *
 *****************************************************************************/
#define SATA2_PHY_CONF_OFFSET           0x760
#define SATA2_PHY_CONF_ADDR             (LS7A_CONFBUS_BASE_ADDR | SATA2_PHY_CONF_OFFSET)

#define SATA2_PHY_LOW_ACCESS_OFFSET     0x768
#define SATA2_PHY_LOW_ACCESS_ADDR       (LS7A_CONFBUS_BASE_ADDR | SATA2_PHY_LOW_ACCESS_OFFSET)

#define SATA2_PHY_HI_ACCESS_OFFSET      0x76c
#define SATA2_PHY_HI_ACCESS_ADDR        (LS7A_CONFBUS_BASE_ADDR | SATA2_PHY_HI_ACCESS_OFFSET)

/*****************************************************************************
 *                                 USB-0                                     *
 *****************************************************************************/
#define USB0_PORT0_CONF_OFFSET          0x700
#define USB0_PORT0_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB0_PORT0_CONF_OFFSET)

#define USB0_PORT1_CONF_OFFSET          0x704
#define USB0_PORT1_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB0_PORT1_CONF_OFFSET)

#define USB0_PORT2_CONF_OFFSET          0x708
#define USB0_PORT2_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB0_PORT2_CONF_OFFSET)

/*****************************************************************************
 *                                 USB-1                                     *
 *****************************************************************************/
#define USB1_PORT0_CONF_OFFSET          0x710
#define USB1_PORT0_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB1_PORT0_CONF_OFFSET)

#define USB1_PORT1_CONF_OFFSET          0x714
#define USB1_PORT1_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB1_PORT1_CONF_OFFSET)

#define USB1_PORT2_CONF_OFFSET          0x718
#define USB1_PORT2_CONF_ADDR           (LS7A_CONFBUS_BASE_ADDR | USB1_PORT2_CONF_OFFSET)
/*****************************************************************************
 *                                   GMEM                                    *
 *****************************************************************************/
#define GPU_CFG_HEADER_ADDR             HT_CFG_HEADER_ADDR(6, 0)
#define GMEM_TEMP_ADDR                  0x40000000
#define LS7A_GMEM_TEMP_ADDR             (HT1_MEM_BASE_ADDR | GMEM_TEMP_ADDR)
#define MC_REGS_COUNT                   118
#define CONF_GMEM_BAR_MASK_OFFSET       0x3838
#define CONF_GMEM_BAR_MASK_ADDR         (LS7A_CONFBUS_BASE_ADDR | CONF_GMEM_BAR_MASK_OFFSET)

/*****************************************************************************
 *                                 ACPI Reg                                  *
 *****************************************************************************/
#define LS7A_ACPI_OFFSET                0x50000
#define LS7A_ACPI_BASE_ADDR             (LS7A_MISC_BASE | LS7A_ACPI_OFFSET)
#define ACPI_PMCON_RESUME_REG_OFFSET    0x4
#define ACPI_PM1_STS_REG_OFFSET         0xc
#define ACPI_PM1_CNT_REG_OFFSET         0x14
#define ACPI_GPE0_STS_REG_OFFSET        0x28
#define ACPI_GPE0_EN_REG_OFFSET         0x2c
#define ACPI_RST_CNT_REG_OFFSET         0x30
#define ACPI_RTC1_OFFSET                0x50
#define ACPI_RTC2_OFFSET                0x54
#define ACPI_RESET_VAL                  0x1
#define ACPI_PM1_STS_CLEAN_VAL          0xffffffff
#define ACPI_SHUT_DOWN_VAL              0x3c00

/*****************************************************************************
 *                                  PWM Reg                                  *
 *****************************************************************************/
#define LS7A_PWM_REG_BASE              (LS7A_MISC_BASE + 0x20000)
#define LS7A_PWM0_REG_BASE              (0)
#define LS7A_PWM0_LOW                   (LS7A_PWM0_REG_BASE  + 0x4)
#define LS7A_PWM0_FULL                  (LS7A_PWM0_REG_BASE  + 0x8)
#define LS7A_PWM0_CTRL                  (LS7A_PWM0_REG_BASE  + 0xc)

#define LS7A_PWM1_REG_BASE              (0x100)
#define LS7A_PWM1_LOW                   (LS7A_PWM1_REG_BASE  + 0x4)
#define LS7A_PWM1_FULL                  (LS7A_PWM1_REG_BASE  + 0x8)
#define LS7A_PWM1_CTRL                  (LS7A_PWM1_REG_BASE  + 0xc)

#define LS7A_PWM2_REG_BASE              (0x200)
#define LS7A_PWM2_LOW                   (LS7A_PWM2_REG_BASE  + 0x4)
#define LS7A_PWM2_FULL                  (LS7A_PWM2_REG_BASE  + 0x8)
#define LS7A_PWM2_CTRL                  (LS7A_PWM2_REG_BASE  + 0xc)

#define LS7A_PWM3_REG_BASE              (0x300)
#define LS7A_PWM3_LOW                   (LS7A_PWM3_REG_BASE  + 0x4)
#define LS7A_PWM3_FULL                  (LS7A_PWM3_REG_BASE  + 0x8)
#define LS7A_PWM3_CTRL                  (LS7A_PWM3_REG_BASE  + 0xc)

/*****************************************************************************
 *                                  GPIO Reg                                  *
 *****************************************************************************/
#define LS7A_GPIO_REG_BASE              (LS7A_MISC_BASE + 0x60000)
#define LS7A_GPIO_REG_DIR_BASE          (LS7A_GPIO_REG_BASE + 0x800)
#define LS7A_GPIO_REG_OUT_BASE          (LS7A_GPIO_REG_BASE + 0x900)
#define LS7A_GPIO_REG_IN_BASE           (LS7A_GPIO_REG_BASE + 0xA00)
#define LS7A_GPIO_REG_INT_BASE          (LS7A_GPIO_REG_BASE + 0xB00)

#endif

