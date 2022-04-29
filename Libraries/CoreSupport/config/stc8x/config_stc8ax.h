/**
 * @file config_stc8ax.h
 * @author MContour (m-contour@qq.com)
 * @brief core and peripheral definition for stc8x 
 * @details This file contains all the peripheral register's definitions, bits 
 *          definitions and memory mapping for STC8x Connectivity line.
 * @version 0.1
 * @date 2022-04-27
 * 
 * @copyright Apache 2.0 LICENSE
 * 
 *****************************************************************************
 * Copyright (c) [2022-04-27] [MContour m-contour@qq.com]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************
 */
#ifndef __CONFIG_STC8AX_H__
#define __CONFIG_STC8AX_H__
//包含本头文件后,不用另外再包含"REG51.H"

/**
 * @defgroup Special_function_registers
 * Special function registers locate at the expension RAM area.
 * If you want to read/write the special function registers, the BIT7 of P_SW2 needs to be set as 1.
 */

/** @addtogroup STC
 * @{
 */

/** @defgroup Core
 * @{
 */


//! kernel special function register setting 
sfr ACC         =   0xe0;
sfr B           =   0xf0;
sfr PSW         =   0xd0;
sfr SP          =   0x81;
sfr DPL         =   0x82;
sfr DPH         =   0x83;
sfr TA          =   0xae;
sfr DPS         =   0xe3;
sfr DPL1        =   0xe4;
sfr DPH1        =   0xe5;

sbit CY         =   PSW^7;
sbit AC         =   PSW^6;
sbit F0         =   PSW^5;
sbit RS1        =   PSW^4;
sbit RS0        =   PSW^3;
sbit OV         =   PSW^2;
sbit F1         =   PSW^1;
sbit P          =   PSW^0;

/** @defgroup Peripheral_memory_map
 * @{
 */

//! system management special function register setting 
/* bus speed control register setting */
#define     BUS_SPEED_ADDR              ((uint8_t)0xA1)

/* Bit definition for BUS_SPEED registor */
//! RW_S configuration
#define     BUS_SPEED_RW_S              ((uint8_t)0xC0) /*!< RW_S[1:0] bits (control line selection) */
#define     BUS_SPEED_RW_S_0            ((uint8_t)0x40) /*!< Bit 0 */
#define     BUS_SPEED_RW_S_1            ((uint8_t)0x80) /*!< Bit 1 */

#define     BUS_SPEED_RW_S_GROUP1       ((uint8_t)0x00) /*!< select P4.4 pin as RD and P4.3 as WR */
#define     BUS_SPEED_RW_S_GROUP2       ((uint8_t)0x40) /*!< select P3.7 pin as RD and P3.6 as WR */
#define     BUS_SPEED_RW_S_GROUP3       ((uint8_t)0x80) /*!< select P4.2 pin as RD and P4.0 as WR */

//! SPEED configuration
#define     BUS_SPEED_SPEED             ((uint8_t)0x03) /*!< SPEED[1:0] bits (bus read/write speed control) */
#define     BUS_SPEED_SPEED_0           ((uint8_t)0x01) /*!< Bit 0 */
#define     BUS_SPEED_SPEED_1           ((uint8_t)0x02) /*!< Bit 1 */

#define     BUS_SPEED_SPEED_VAL0        ((uint8_t)0x00) /*!< speed numeric value 0 */
#define     BUS_SPEED_SPEED_VAL1        ((uint8_t)0x01) /*!< speed numeric value 1 */
#define     BUS_SPEED_SPEED_VAL2        ((uint8_t)0x02) /*!< speed numeric value 2 */
#define     BUS_SPEED_SPEED_VAL3        ((uint8_t)0x03) /*!< speed numeric value 3 */


sfr BUS_SPEED   =   BUS_SPEED_ADDR; //! bus speed control register

/*  auxiliary register setting */
#define     AUXR_ADDR                   ((uint8_t)0x8E)
sfr AUXR        =   AUXR_ADDR;      //! auxiliary register

/* peripheral port switch register setting */
#define     PERIPHERAL_PORT_SW1_ADDR    ((uint8_t)0xA2)
#define     PERIPHERAL_PORT_SW2_ADDR    ((uint8_t)0xBA)

/* Bit definition for P_SW1 registor */
//! S1_S configuration
#define     P_SW1_S1_S                  ((uint8_t)0xC0) /*!< S1_S[1:0] bits (serial port 1 function pins selection) */
#define     P_SW1_S1_S_0                ((uint8_t)0x40) /*!< Bit 0 TxD */
#define     P_SW1_S1_S_1                ((uint8_t)0x80) /*!< Bit 1 RxD */

#define     P_SW1_S1_S_GROUP1           ((uint8_t)0x00) /*!< select P3.0 pin as RxD and P3.1 as TxD */
#define     P_SW1_S1_S_GROUP2           ((uint8_t)0x40) /*!< select P3.6 pin as RxD and P3.7 as TxD */
#define     P_SW1_S1_S_GROUP3           ((uint8_t)0x80) /*!< select P1.6 pin as RxD and P1.7 as TxD */
#define     P_SW1_S1_S_GROUP4           ((uint8_t)0xC0) /*!< select P4.3 pin as RxD and P4.4 as TxD */

//! CCP_S configuration
#define     P_SW1_CCP_S                 ((uint8_t)0x30) /*!< CCP_S[1:0] bits (PCA function pins selection) */
#define     P_SW1_CCP_S_0               ((uint8_t)0x10) /*!< Bit 0 */
#define     P_SW1_CCP_S_1               ((uint8_t)0x20) /*!< Bit 1 */

#define     P_SW1_CCP_S_GROUP1          ((uint8_t)0x00) /*!< select P1.2 pin as ECI, P1.7 as CPP0, P1.6 as CPP1, P1.5 as CPP2, P1.4 as CPP3 */
#define     P_SW1_CCP_S_GROUP2          ((uint8_t)0x10) /*!< select P2.2 pin as ECI, P2.3 as CPP0, P2.4 as CPP1, P2.5 as CPP2, P2.6 as CPP3 */
#define     P_SW1_CCP_S_GROUP3          ((uint8_t)0x20) /*!< select P7.4 pin as ECI, P7.0 as CPP0, P7.1 as CPP1, P7.2 as CPP2, P7.3 as CPP3 */
#define     P_SW1_CCP_S_GROUP4           ((uint8_t)0x30) /*!< select P3.5 pin as ECI, P3.3 as CPP0, P3.2 as CPP1, P3.1 as CPP2, P3.0 as CPP3 */

//! SPI_S configuration
#define     P_SW1_SPI_S                 ((uint8_t)0x0C) /*!< SPI_S[1:0] bits (SPI function pins selection) */
#define     P_SW1_SPI_S_0               ((uint8_t)0x04) /*!< Bit 0 */
#define     P_SW1_SPI_S_1               ((uint8_t)0x08) /*!< Bit 1 */

#define     P_SW1_SPI_S_GROUP1          ((uint8_t)0x00) /*!< select P1.2 pin as SS, P1.3 as MOSI, P1.4 as MISO, P1.5 as SCLK */
#define     P_SW1_SPI_S_GROUP2          ((uint8_t)0x04) /*!< select P2.2 pin as SS, P2.3 as MOSI, P2.4 as MISO, P2.5 as SCLK */
#define     P_SW1_SPI_S_GROUP3          ((uint8_t)0x08) /*!< select P7.4 pin as SS, P7.5 as MOSI, P7.6 as MISO, P7.7 as SCLK */
#define     P_SW1_SPI_S_GROUP4          ((uint8_t)0x0C) /*!< select P3.5 pin as SS, P3.4 as MOSI, P3.3 as MISO, P3.2 as SCLK */

/* Bit definition for P_SW1 registor */
//! EAXFR configuration
#define     P_SW2_EAXFR                 ((uint8_t)0x80) /*!< XFR control register */

//! I2C_S configuration
#define     P_SW2_I2C_S                 ((uint8_t)0x30) /*!< I2C_S[1:0] bits (I2C function pins selection) */
#define     P_SW2_I2C_S_0               ((uint8_t)0x10) /*!< Bit 0 SDA */
#define     P_SW2_I2C_S_1               ((uint8_t)0x20) /*!< Bit 1 SCL */



//! CMPO_S configuration
#define     P_SW2_CMPO_S                ((uint8_t)0x08) /*!< CMPO selection */

//! S4_S configuration
#define     P_SW2_S4_S                  ((uint8_t)0x04) /*!< serial port 4 function pins selection */

//! S3_S configuration
#define     P_SW2_S3_S                  ((uint8_t)0x02) /*!< serial port 3 function pins selection */

//! S2_S configuration
#define     P_SW2_S2_S                  ((uint8_t)0x01) /*!< serial port 2 function pins selection */

sfr P_SW1       =   PERIPHERAL_PORT_SW1_ADDR;
sfr P_SW2       =   PERIPHERAL_PORT_SW2_ADDR;

/**
 * @brief system clock control interface
 * 
 */
typedef struct
{
    __IO uint8_t RCC_CKSEL;
    __IO uint8_t RCC_CLKDIV;
    __IO uint8_t RCC_IRC24MCR;
    __IO uint8_t RCC_XOSCCR;
    __IO uint8_t RCC_IRC32KCR;
} SYSCLK_TypeDef;

//! system clock special function register setting in internal expansion RAM 
/* internel oscillator frequency control and adjustment registers*/
#define     IRTRIM_ADDR                 ((uint8_t)0x9F)
#define     LIRTRIM_ADDR                ((uint8_t)0x9E)

/* Bit definition for LIRTRIM registor */
#define     LIRTRIM_BITS                ((uint8_t)0x03)
#define     LIRTRIM_BITS_0              ((uint8_t)0x01)
#define     LIRTRIM_BITS_1              ((uint8_t)0x02)

sfr IRTRIM      =   IRTRIM_ADDR;    //! internal IRC frequency adjustment register
sfr LIRTRIM     =   LIRTRIM_ADDR;   //! internal IRC frequency subtle adjustment register

/* system clock base address */
#define     SYSCLK_BASE     ((uint16_t)0xFE00)

#define     CKSEL_ADDR      (SYSCLK_BASE + 0x0000)
#define     CLKDIV_ADDR     (SYSCLK_BASE + 0x0001)
#define     IRC24MCR_ADDR   (SYSCLK_BASE + 0x0002)
#define     XOSCCR_ADDR     (SYSCLK_BASE + 0x0003)
#define     IRC32KCR_ADDR   (SYSCLK_BASE + 0x0004)

/* Bit definition for CKSEL registor */
//! MCLKODIV configuration
#define     CKSEL_MCLKODIV              ((uint8_t)0xF0) /*!< MCLKODIV[3:0] bits (main clock output division coefficient) */
#define     CKSEL_MCLKODIV_0            ((uint8_t)0x10) /*!< Bit 0 */
#define     CKSEL_MCLKODIV_1            ((uint8_t)0x20) /*!< Bit 1 */
#define     CKSEL_MCLKODIV_2            ((uint8_t)0x40) /*!< Bit 2 */
#define     CKSEL_MCLKODIV_3            ((uint8_t)0x80) /*!< Bit 3 */

#define     CKSEL_MCLKODIV_DIV0         ((uint8_t)0x00) /*!< no MCLK output */
#define     CKSEL_MCLKODIV_DIV1         ((uint8_t)0x10) /*!< MCLK input clock not divided */
#define     CKSEL_MCLKODIV_DIV2         ((uint8_t)0x20) /*!< MCLK input clock divided by 2 */
#define     CKSEL_MCLKODIV_DIV4         ((uint8_t)0x40) /*!< MCLK input clock divided by 4 */
#define     CKSEL_MCLKODIV_DIV8         ((uint8_t)0x60) /*!< MCLK input clock divided by 8 */
#define     CKSEL_MCLKODIV_DIV16        ((uint8_t)0x80) /*!< MCLK input clock divided by 16 */
#define     CKSEL_MCLKODIV_DIV32        ((uint8_t)0xA0) /*!< MCLK input clock divided by 32 */
#define     CKSEL_MCLKODIV_DIV64        ((uint8_t)0xC0) /*!< MCLK input clock divided by 64 */
#define     CKSEL_MCLKODIV_DIV128       ((uint8_t)0xE0) /*!< MCLK input clock divided by 128 */

//! MCLKO_S configuration
#define     CKSEL_MCLKO_S               ((uint8_t)0x08) /*!< divided MCLK output pin */

//! MCKSEL configuration
#define     CKSEL_MCKSEL                ((uint8_t)0x03) /*!< MCLKODIV[1:0] bits (system clock output division coefficient) */
#define     CKSEL_MCKSEL_0              ((uint8_t)0x01) /*!< Bit 0 */
#define     CKSEL_MCKSEL_1              ((uint8_t)0x02) /*!< Bit 1 */

/* Bit definition for CLKDIV registor */

/* Bit definition for IRC24MCR registor */
//! ENIRC24M configuration
#define     IRC24MCR_ENIRC24M           ((uint8_t)0x80) /*!< IRC24MCR enabling control */

//! IRC24MST configuration
#define     IRC24MCR_IRC24MST           ((uint8_t)0x01) /*!< IRC24MCR stable state sign */

/* Bit definition for XOSCCR registor */
//! ENXOSC configuration
#define     XOSCCR_ENXOSC               ((uint8_t)0x80) /*!< external oscillator enabling control */

//! XITYPE configuration
#define     XOSCCR_XITYPE               ((uint8_t)0x40) /*!< external oscillator type selection */

//! XOSCST configuration
#define     XOSCCR_XOSCST               ((uint8_t)0x01) /*!< external oscillator stable state sign */

/* Bit definition for IRC32KCR registor */
//! ENIRC32K configuration
#define     IRC24MCR_ENIRC32K           ((uint8_t)0x80) /*!< IRC32KCR enabling control */

//! IRC32KST configuration
#define     IRC24MCR_IRC32KST           ((uint8_t)0x01) /*!< IRC32KCR stable state sign */

/**
 * @brief define the SYSCLK type
 * @note To access XDATA peripherals, use the `volatile` keyword to ensure 
 * that the C compiler does not optimize out necessary memory accesses.
 */
#define     SYSCLK          ((SYSCLK_TypeDef xdata *) SYSCLK_BASE)

#define     CKSEL           ((__IO uint8_t xdata *)   CKSEL_ADDR)     //!< system clock selection
#define     CLKDIV          ((__IO uint8_t xdata *)   CLKDIV_ADDR)    //!< system clock division
#define     IRC24MCR        ((__IO uint8_t xdata *)   IRC24MCR_ADDR)  //!< internal 24MHz oscillator control
#define     XOSCCR          ((__IO uint8_t xdata *)   XOSCCR_ADDR)    //!< external oscillator control
#define     IRC32KCR        ((__IO uint8_t xdata *)   IRC32KCR_ADDR)  //!< internal 32kHz oscillator control

//! system power special function register setting
/* power control base address */
#define     PCON_ADDR       0x87
#define     VOCTRL_ADDR     0xBB
sfr PCON        =   PCON_ADDR;
sfr VOCTRL      =   VOCTRL_ADDR;

//! system reset special function register control
/* reset control base address */
#define     WDT_ADDR         0xC1u
// #define     IAP_ADDR      0xC7u
#define     RSTCFG_ADDR      0xFFu

/* Bit definition for WDT_CONTR registor */
#define     WDT_FLAG         0x80
#define     EN_WDT           0x20
#define     CLR_WDT          0x10
#define     IDL_WDT          0x08

sfr WDT_CONTR   =   WDT_ADDR;
// sfr IAP_CONTR   =   IAP_ADDR;
sfr RSTCFG      =   RSTCFG_ADDR;

/* wake up timer base address */
#define     WKT_BASE        0xAAu
#define     WKTCL_ADDR      (WKT_BASE + 0x0000u)
#define     WKTCH_ADDR      (WKT_BASE + 0x0001u)

/* WKTCH offset */
#define WKTEN       0x80

sfr WKTCL       =   WKTCL_ADDR;
sfr WKTCH       =   WKTCH_ADDR;

//! GPIO peripherals
#define     GPIO_BASE       0x80u
#define     PxM0_BASE       0x94u
#define     PxM1_BASE       0x93u
#define     PxPU_BASE       0xFE10u
#define     PxNCS_BASE      0xFE18u

/* GPIO address definition */
#define     P0_ADDR         (GPIO_BASE + 0x00u) 
#define     P1_ADDR         (GPIO_BASE + 0x10u) 
#define     P2_ADDR         (GPIO_BASE + 0x20u) 
#define     P3_ADDR         (GPIO_BASE + 0x30u) 
#define     P4_ADDR         (GPIO_BASE + 0x40u) 
#define     P5_ADDR         (GPIO_BASE + 0x48u) 
#define     P6_ADDR         (GPIO_BASE + 0x68u) 
#define     P7_ADDR         (GPIO_BASE + 0x78u) 

/* PxM0: GPIO configuration registers 0 address definition */
#define     P0M0_ADDR       PxM0_BASE 
#define     P1M0_ADDR       0x92u
#define     P2M0_ADDR       0x96u
#define     P3M0_ADDR       0xB2u
#define     P4M0_ADDR       0xB4u
#define     P5M0_ADDR       0xCAu
#define     P6M0_ADDR       0xCCu
#define     P7M0_ADDR       0xE2u

/* PxM1: GPIO configuration registers 1 address definition */
#define     P0M1_ADDR       PxM1_BASE 
#define     P1M1_ADDR       0x91u
#define     P2M1_ADDR       0x95u
#define     P3M1_ADDR       0xB1u
#define     P4M1_ADDR       0xB3u
#define     P5M1_ADDR       0xC9u
#define     P6M1_ADDR       0xCBu
#define     P7M1_ADDR       0xE1u

/* GPIO pull up resistor control register address definition */
#define     P0PU_ADDR        (PxPU_BASE + 0x00u)
#define     P1PU_ADDR        (PxPU_BASE + 0x01u)
#define     P2PU_ADDR        (PxPU_BASE + 0x02u)
#define     P3PU_ADDR        (PxPU_BASE + 0x03u)
#define     P4PU_ADDR        (PxPU_BASE + 0x04u)
#define     P5PU_ADDR        (PxPU_BASE + 0x05u)
#define     P6PU_ADDR        (PxPU_BASE + 0x06u)
#define     P7PU_ADDR        (PxPU_BASE + 0x07u)

/*GPIO schmidt trigger control register address definition */

#define    P0NCS_ADDR       (PxNCS_BASE + 0x00u)
#define    P1NCS_ADDR       (PxNCS_BASE + 0x01u)
#define    P2NCS_ADDR       (PxNCS_BASE + 0x02u)
#define    P3NCS_ADDR       (PxNCS_BASE + 0x03u)
#define    P4NCS_ADDR       (PxNCS_BASE + 0x04u)
#define    P5NCS_ADDR       (PxNCS_BASE + 0x05u)
#define    P6NCS_ADDR       (PxNCS_BASE + 0x06u)
#define    P7NCS_ADDR       (PxNCS_BASE + 0x07u)


/* GPIO registers */
sfr P0          =   P0_ADDR;
sfr P1          =   P1_ADDR;
sfr P2          =   P2_ADDR;
sfr P3          =   P3_ADDR;
sfr P4          =   P4_ADDR;
sfr P5          =   P5_ADDR;
sfr P6          =   P6_ADDR;
sfr P7          =   P7_ADDR;

/* GPIO configuration registers 0 */
sfr P0M0        =   P0M0_ADDR;
sfr P1M0        =   P1M0_ADDR;
sfr P2M0        =   P2M0_ADDR;
sfr P3M0        =   P3M0_ADDR;
sfr P4M0        =   P4M0_ADDR;
sfr P5M0        =   P5M0_ADDR;
sfr P6M0        =   P6M0_ADDR;
sfr P7M0        =   P7M0_ADDR;

/* GPIO configuration registers 1 */
sfr P0M1        =   P0M1_ADDR;
sfr P1M1        =   P1M1_ADDR;
sfr P2M1        =   P2M1_ADDR;
sfr P3M1        =   P3M1_ADDR;
sfr P4M1        =   P4M1_ADDR;
sfr P5M1        =   P5M1_ADDR;
sfr P6M1        =   P6M1_ADDR;
sfr P7M1        =   P7M1_ADDR;

/* specific pins of GPIO registers */
sbit P00        =   P0^0;
sbit P01        =   P0^1;
sbit P02        =   P0^2;
sbit P03        =   P0^3;
sbit P04        =   P0^4;
sbit P05        =   P0^5;
sbit P06        =   P0^6;
sbit P07        =   P0^7;
sbit P10        =   P1^0;
sbit P11        =   P1^1;
sbit P12        =   P1^2;
sbit P13        =   P1^3;
sbit P14        =   P1^4;
sbit P15        =   P1^5;
sbit P16        =   P1^6;
sbit P17        =   P1^7;
sbit P20        =   P2^0;
sbit P21        =   P2^1;
sbit P22        =   P2^2;
sbit P23        =   P2^3;
sbit P24        =   P2^4;
sbit P25        =   P2^5;
sbit P26        =   P2^6;
sbit P27        =   P2^7;
sbit P30        =   P3^0;
sbit P31        =   P3^1;
sbit P32        =   P3^2;
sbit P33        =   P3^3;
sbit P34        =   P3^4;
sbit P35        =   P3^5;
sbit P36        =   P3^6;
sbit P37        =   P3^7;
sbit P40        =   P4^0;
sbit P41        =   P4^1;
sbit P42        =   P4^2;
sbit P43        =   P4^3;
sbit P44        =   P4^4;
sbit P45        =   P4^5;
sbit P46        =   P4^6;
sbit P47        =   P4^7;
sbit P50        =   P5^0;
sbit P51        =   P5^1;
sbit P52        =   P5^2;
sbit P53        =   P5^3;
sbit P54        =   P5^4;
sbit P55        =   P5^5;
sbit P56        =   P5^6;
sbit P57        =   P5^7;
sbit P60        =   P6^0;
sbit P61        =   P6^1;
sbit P62        =   P6^2;
sbit P63        =   P6^3;
sbit P64        =   P6^4;
sbit P65        =   P6^5;
sbit P66        =   P6^6;
sbit P67        =   P6^7;
sbit P70        =   P7^0;
sbit P71        =   P7^1;
sbit P72        =   P7^2;
sbit P73        =   P7^3;
sbit P74        =   P7^4;
sbit P75        =   P7^5;
sbit P76        =   P7^6;
sbit P77        =   P7^7;

/* GPIO control register */
#define     P0PU        (*(__IO uint8_t xdata *) P0PU_ADDR)             // Extended SFR
#define     P1PU        (*(__IO uint8_t xdata *) P1PU_ADDR)             // Extended SFR
#define     P2PU        (*(__IO uint8_t xdata *) P2PU_ADDR)             // Extended SFR
#define     P3PU        (*(__IO uint8_t xdata *) P3PU_ADDR)             // Extended SFR
#define     P4PU        (*(__IO uint8_t xdata *) P4PU_ADDR)             // Extended SFR
#define     P5PU        (*(__IO uint8_t xdata *) P5PU_ADDR)             // Extended SFR
#define     P6PU        (*(__IO uint8_t xdata *) P6PU_ADDR)             // Extended SFR
#define     P7PU        (*(__IO uint8_t xdata *) P7PU_ADDR)             // Extended SFR

#define     P0NCS       (*(__IO uint8_t xdata *) P1NCS_ADDR)            // Extended SFR
#define     P1NCS       (*(__IO uint8_t xdata *) P2NCS_ADDR)            // Extended SFR
#define     P2NCS       (*(__IO uint8_t xdata *) P3NCS_ADDR)            // Extended SFR
#define     P3NCS       (*(__IO uint8_t xdata *) P4NCS_ADDR)            // Extended SFR
#define     P4NCS       (*(__IO uint8_t xdata *) P5NCS_ADDR)            // Extended SFR
#define     P5NCS       (*(__IO uint8_t xdata *) P6NCS_ADDR)            // Extended SFR
#define     P6NCS       (*(__IO uint8_t xdata *) P7NCS_ADDR)            // Extended SFR
#define     P7NCS       (*(__IO uint8_t xdata *) P8NCS_ADDR)            // Extended SFR
/** @} */

//! ISR peripherals
/* ISR base address */
#define    IE_ADDR          0xA8u
#define    IE2_ADDR         0xAFu
#define    IP_ADDR          0xB8u
#define    IP2_ADDR         0xB5u
#define    IPH_ADDR         0xB7u
#define    IP2H_ADDR        0xB6u
#define    INTCLKO_ADDR     0x8Fu
#define    AUXINTIF_ADDR    0xEFu

/* IE2 offset */
#define     ET4             0x40u
#define     ET3             0x20u
#define     ES4             0x10u
#define     ES3             0x08u
#define     ET2             0x04u
#define     ESPI            0x02u
#define     ES2             0x01u

/* IP2 offset */
#define     PI2C            0x40u
#define     PCMP            0x20u
#define     PX4             0x10u
#define     PPWMFD          0x08u
#define     PPWM            0x04u
#define     PSPI            0x02u
#define     PS2             0x01u

/* IPH offset */
#define     PPCAH           0x80u
#define     PLVDH           0x40u
#define     PADCH           0x20u
#define     PSH             0x10u
#define     PT1H            0x08u
#define     PX1H            0x04u
#define     PT0H            0x02u
#define     PX0H            0x01u

/* IP2H offset */
#define     PI2CH           0x40u
#define     PCMPH           0x20u
#define     PX4H            0x10u
#define     PPWMFDH         0x08u
#define     PPWMH           0x04u
#define     PSPIH           0x02u
#define     PS2H            0x01u

/* INTCLKO offset */
#define     EX4             0x40u
#define     EX3             0x20u
#define     EX2             0x10u
#define     T2CLKO          0x04u
#define     T1CLKO          0x02u
#define     T0CLKO          0x01u

/* AUXINTIF offset */
#define     INT4IF          0x40u
#define     INT3IF          0x20u
#define     INT2IF          0x10u
#define     T4IF            0x04u
#define     T3IF            0x02u
#define     T2IF            0x01u

/* ISR register */
sfr IE          =   IE_ADDR;
sfr IE2         =   IE2_ADDR;
sfr IP          =   IP_ADDR;
sfr IP2         =   IP2H_ADDR;
sfr IPH         =   IPH_ADDR;
sfr IP2H        =   IP2H_ADDR;
sfr INTCLKO     =   INTCLKO_ADDR;
sfr AUXINTIF    =   AUXINTIF_ADDR;


// IE BITs (interuption enabling register)
sbit EA         =   IE^7;
sbit ELVD       =   IE^6;
sbit EADC       =   IE^5;
sbit ES         =   IE^4;
sbit ET1        =   IE^3;
sbit EX1        =   IE^2;
sbit ET0        =   IE^1;
sbit EX0        =   IE^0;

// IP BITs
sbit PPCA       =   IP^7;
sbit PLVD       =   IP^6;
sbit PADC       =   IP^5;
sbit PS         =   IP^4;
sbit PT1        =   IP^3;
sbit PX1        =   IP^2;
sbit PT0        =   IP^1;
sbit PX0        =   IP^0;

//! Timer peripherals
/* timer basic address definition */
#define     Timer_BASE      0x88u
#define     T2T3T4_BASE     0xD1u
#define     TCON_ADDR       (Timer_BASE + 0x00u)
#define     TMOD_ADDR       (Timer_BASE + 0x01u)
#define     TL0_ADDR        (Timer_BASE + 0x02u)
#define     TL1_ADDR        (Timer_BASE + 0x03u)
#define     TH0_ADDR        (Timer_BASE + 0x04u)
#define     TH1_ADDR        (Timer_BASE + 0x05u)
#define     T4T3M_ADDR      (T2T3T4_BASE + 0x00u)
#define     T4H_ADDR        (T2T3T4_BASE + 0x01u)
#define     T4L_ADDR        (T2T3T4_BASE + 0x02u)
#define     T3H_ADDR        (T2T3T4_BASE + 0x03u)
#define     T3L_ADDR        (T2T3T4_BASE + 0x04u)
#define     T2H_ADDR        (T2T3T4_BASE + 0x05u)
#define     T2L_ADDR        (T2T3T4_BASE + 0x06u)

/* TMOD offset */
#define     T1_GATE         0x80
#define     T1_CT           0x40
#define     T1_M1           0x20
#define     T1_M0           0x10
#define     T0_GATE         0x08
#define     T0_CT           0x04
#define     T0_M1           0x02
#define     T0_M0           0x01

/* T4T3M offset */
#define     T4R             0x80
#define     T4_CT           0x40
#define     T4x12           0x20
#define     T4CLKO          0x10
#define     T3R             0x08
#define     T3_CT           0x04
#define     T3x12           0x02
#define     T3CLKO          0x01

/* timer special function registers */
sfr TCON        =   TCON_ADDR;
sfr TMOD        =   TMOD_ADDR;
sfr TL0         =   TL0_ADDR;
sfr TL1         =   TL1_ADDR;
sfr TH0         =   TH0_ADDR;
sfr TH1         =   TH1_ADDR;
sfr T4T3M       =   T4T3M_ADDR;
sfr T4H         =   T4H_ADDR;
sfr T4L         =   T4L_ADDR;
sfr T3H         =   T3H_ADDR;
sfr T3L         =   T3L_ADDR;
sfr T2H         =   T2H_ADDR;
sfr T2L         =   T2L_ADDR;

/* TCON BITs setting */
sbit TF1        =   TCON^7;
sbit TR1        =   TCON^6;
sbit TF0        =   TCON^5;
sbit TR0        =   TCON^4;
sbit IE1        =   TCON^3;
sbit IT1        =   TCON^2;
sbit IE0        =   TCON^1;
sbit IT0        =   TCON^0;

//! UART peripherals
/* UART basic address definition */
#define     SCON_ADDR       0x98u
#define     SBUF_ADDR       0x99u
#define     S2CON_ADDR      0x9Au
#define     S2BUF_ADDR      0x9Bu
#define     S3CON_ADDR      0xACu
#define     S3BUF_ADDR      0xADu
#define     S4CON_ADDR      0x84u
#define     S4BUF_ADDR      0x85u
#define     SADDR_ADDR      0xA9u
#define     SADEN_ADDR      0xB9u

/* S2CON offset */
#define     S2SM0           0x80u
#define     S2ST4           0x40u
#define     S2SM2           0x20u
#define     S2REN           0x10u
#define     S2TB8           0x08u
#define     S2RB8           0x04u
#define     S2TI            0x02u
#define     S2RI            0x01u

/* S3CON offset */
#define     S3SM0           0x80u
#define     S3ST4           0x40u
#define     S3SM2           0x20u
#define     S3REN           0x10u
#define     S3TB8           0x08u
#define     S3RB8           0x04u
#define     S3TI            0x02u
#define     S3RI            0x01u

/* S4CON offset */
#define     S4SM0           0x80u
#define     S4ST4           0x40u
#define     S4SM2           0x20u
#define     S4REN           0x10u
#define     S4TB8           0x08u
#define     S4RB8           0x04u
#define     S4TI            0x02u
#define     S4RI            0x01u

/* UART special function registers */
sfr SCON        =   SCON_ADDR;
sfr SBUF        =   SBUF_ADDR; 
sfr S2CON       =   S2CON_ADDR;
sfr S2BUF       =   S2BUF_ADDR;
sfr S3CON       =   S3CON_ADDR;
sfr S3BUF       =   S3BUF_ADDR;
sfr S4CON       =   S4CON_ADDR;
sfr S4BUF       =   S4BUF_ADDR;
sfr SADDR       =   SADDR_ADDR;
sfr SADEN       =   SADEN_ADDR;

/* SCON BITs */
sbit SM0        =   SCON^7;
sbit SM1        =   SCON^6;
sbit SM2        =   SCON^5;
sbit REN        =   SCON^4;
sbit TB8        =   SCON^3;
sbit RB8        =   SCON^2;
sbit TI         =   SCON^1;
sbit RI         =   SCON^0;

//! COMP peripherals
/* COMP basic address definition */
#define     CMPCR1_ADDR     0xE6u
#define     CMPCR2_ADDR     0xE7u

/* CMPCR1 offset */
#define     CMPEN           0x80u
#define     CMPIF           0x40u
#define     PIE             0x20u
#define     NIE             0x10u
#define     PIS             0x08u
#define     NIS             0x04u
#define     CMPOE           0x02u
#define     CMPRES          0x01u

/* CMPAR2 offset */
#define     INVCMPO         0x80
#define     DISFLT          0x40

sfr CMPCR1      =   CMPCR1_ADDR;
sfr CMPCR2      =   CMPCR2_ADDR;

//! EEPROM peripherals
/* EEPROM basic address definition */
#define     IAP_DATA_ADDR   0xC2u
#define     IAP_ADDRH_ADDR  0xC3u
#define     IAP_ADDRL_ADDR  0xC4u
#define     IAP_CMD_ADDR    0xC5u
#define     IAP_TRIG_ADDR   0xC6u
#define     IAP_CONTR_ADDR  0xC7u

/* IAP_CONTR offset */
#define     IAPEN           0x80u
#define     SWBS            0x40u
#define     SWRST           0x20u
#define     CMD_FAIL        0x10u

/* IAP special function registers */
sfr IAP_DATA    =   IAP_DATA_ADDR;
sfr IAP_ADDRH   =   IAP_ADDRH_ADDR;
sfr IAP_ADDRL   =   IAP_ADDRL_ADDR;
sfr IAP_CMD     =   IAP_CMD_ADDR;
sfr IAP_TRIG    =   IAP_TRIG_ADDR;
sfr IAP_CONTR   =   IAP_CONTR_ADDR;

//! ADC peripherals
/* ADC basic address definition */
#define     ADC_CONTR_ADDR  0xBCu
#define     ADC_RESH_ADDR   0xBDu
#define     ADC_RESL_ADDR   0xBEu
#define     ADCCFG_ADDR     0xDEu

/* ADC_CONTR offset */
#define     ADC_POWER       0x80u
#define     ADC_START       0x40u
#define     ADC_FLAG        0x20u

/* ADCCFG offset */
#define     ADC_RESFMT      0x20u

/* ADC special function registers */
sfr ADC_CONTR   =   ADC_CONTR_ADDR;
sfr ADC_RESH    =   ADC_RESH_ADDR;
sfr ADC_RESL    =   ADC_RESL_ADDR;
sfr ADCCFG      =   ADCCFG_ADDR;

//! PCA peripherals
/* PCA basic address definition */
#define     CCON_ADDR       0xD8u
#define     CMOD_ADDR       0xD9u
#define     CL_ADDR         0xE9u
#define     CH_ADDR         0xF9u
#define     CCAPM0_ADDR     0xDAu
#define     CCAPM1_ADDR     0xDBu
#define     CCAPM2_ADDR     0xDCu
#define     CCAPM3_ADDR     0xDDu
#define     CCAP0L_ADDR     0xEAu
#define     CCAP1L_ADDR     0xEBu
#define     CCAP2L_ADDR     0xECu
#define     CCAP3L_ADDR     0xEDu
#define     CCAP0H_ADDR     0xFAu
#define     CCAP1H_ADDR     0xFBu
#define     CCAP2H_ADDR     0xFCu
#define     CCAP3H_ADDR     0xFDu
#define     PCA_PWM0_ADDR   0xF2u
#define     PCA_PWM1_ADDR   0xF3u
#define     PCA_PWM2_ADDR   0xF4u
#define     PCA_PWM3_ADDR   0xF5u

/* CMOD offset */
#define     CIDL            0x80u
#define     ECF             0x01u

/* CCAPM0 offset */
#define     ECOM0           0x40u
#define     CCAPP0          0x20u
#define     CCAPN0          0x10u
#define     MAT0            0x08u
#define     TOG0            0x04u
#define     PWM0            0x02u
#define     ECCF0           0x01u

/* CCAPM1 offset */
#define     ECOM1           0x40u
#define     CCAPP1          0x20u
#define     CCAPN1          0x10u
#define     MAT1            0x08u
#define     TOG1            0x04u
#define     PWM1            0x02u
#define     ECCF1           0x01u

/* CCAMP2 offset */
#define     ECOM2           0x40u
#define     CCAPP2          0x20u
#define     CCAPN2          0x10u
#define     MAT2            0x08u
#define     TOG2            0x04u
#define     PWM2            0x02u
#define     ECCF2           0x01u

/* CCAPM3 offset */
#define     ECOM3           0x40u
#define     CCAPP3          0x20u
#define     CCAPN3          0x10u
#define     MAT3            0x08u
#define     TOG3            0x04u
#define     PWM3            0x02u
#define     ECCF3           0x01u

/* PCA special function registers */
sfr CCON        =   CCON_ADDR;
sfr CMOD        =   CMOD_ADDR;
sfr CL          =   CL_ADDR;
sfr CH          =   CH_ADDR;
sfr CCAPM0      =   CCAPM0_ADDR;
sfr CCAPM1      =   CCAPM1_ADDR;
sfr CCAPM2      =   CCAPM2_ADDR;
sfr CCAPM3      =   CCAPM3_ADDR;
sfr CCAP0L      =   CCAP0L_ADDR;
sfr CCAP1L      =   CCAP1L_ADDR;
sfr CCAP2L      =   CCAP2L_ADDR;
sfr CCAP3L      =   CCAP3L_ADDR;
sfr CCAP0H      =   CCAP0H_ADDR;
sfr CCAP1H      =   CCAP1H_ADDR;
sfr CCAP2H      =   CCAP2H_ADDR;
sfr CCAP3H      =   CCAP3H_ADDR;
sfr PCA_PWM0    =   PCA_PWM0_ADDR;
sfr PCA_PWM1    =   PCA_PWM1_ADDR;
sfr PCA_PWM2    =   PCA_PWM2_ADDR;
sfr PCA_PWM3    =   PCA_PWM3_ADDR;

/* CCON BITs */
sbit CF         =   CCON^7;
sbit CR         =   CCON^6;
sbit CCF3       =   CCON^3;
sbit CCF2       =   CCON^2;
sbit CCF1       =   CCON^1;
sbit CCF0       =   CCON^0;

//! PWM peripherals
/* PWM basic address definition */
#define     PWMCFG_ADDR     0xF1u
#define     PWMIF_ADDR      0xF6u
#define     PWMFDCR_ADDR    0xF7u
#define     PWMCR_ADDR      0xFEu

#define     PWM_BASE        0xFFF0u
#define     PWM0_BASE       0xFF00u
#define     PWM1_BASE       0xFF10u
#define     PWM2_BASE       0xFF20u
#define     PWM3_BASE       0xFF30u
#define     PWM4_BASE       0xFF40u
#define     PWM5_BASE       0xFF50u
#define     PWM6_BASE       0xFF60u
#define     PWM7_BASE       0xFF70u

#define     PWMC_ADDR       (PWM_BASE + 0x00u)
#define     PWMCH_ADDR      (PWMC_ADDR +  0x00u)
#define     PWMCL_ADDR      (PWMCH_ADDR + 0x01u)
#define     PWMCKS_ADDR     (PWMCL_ADDR + 0x01u)

#define     TADCP_ADDR      (PWMCKS_ADDR + 0x01u)
#define     TADCPH_ADDR     (TADCP_ADDR +  0x00u)
#define     TADCPL_ADDR     (TADCPH_ADDR + 0x01u)

#define     PWM0T1_ADDR     (PWM0_BASE + 0x00u)
#define     PWM0T1H_ADDR    (PWM0T1_ADDR +  0x00u)
#define     PWM0T1L_ADDR    (PWM0T1H_ADDR + 0x01u)
#define     PWM0T2_ADDR     (PWM0T1L_ADDR + 0x01u)
#define     PWM0T2H_ADDR    (PWM0T2_ADDR +  0x00u)
#define     PWM0T2L_ADDR    (PWM0T2H_ADDR + 0x01u)
#define     PWM0CR_ADDR     (PWM0T2L_ADDR + 0x01u)
#define     PWM0HLD_ADDR    (PWM0CR_ADDR +  0x01u)

#define     PWM1T1_ADDR     (PWM1_BASE + 0x00u)
#define     PWM1T1H_ADDR    (PWM1T1_ADDR +  0x00u)
#define     PWM1T1L_ADDR    (PWM1T1H_ADDR + 0x01u)
#define     PWM1T2_ADDR     (PWM1T1L_ADDR + 0x01u)
#define     PWM1T2H_ADDR    (PWM1T2_ADDR +  0x00u)
#define     PWM1T2L_ADDR    (PWM1T2H_ADDR + 0x01u)
#define     PWM1CR_ADDR     (PWM1T2L_ADDR + 0x01u)
#define     PWM1HLD_ADDR    (PWM1CR_ADDR +  0x01u)

#define     PWM2T1_ADDR     (PWM2_BASE + 0x00u)
#define     PWM2T1H_ADDR    (PWM2T1_ADDR +  0x00u)
#define     PWM2T1L_ADDR    (PWM2T1H_ADDR + 0x01u)
#define     PWM2T2_ADDR     (PWM2T1L_ADDR + 0x01u)
#define     PWM2T2H_ADDR    (PWM2T2_ADDR +  0x00u)
#define     PWM2T2L_ADDR    (PWM2T2H_ADDR + 0x01u)
#define     PWM2CR_ADDR     (PWM2T2L_ADDR + 0x01u)
#define     PWM2HLD_ADDR    (PWM2CR_ADDR +  0x01u)

#define     PWM3T1_ADDR     (PWM3_BASE + 0x00u)
#define     PWM3T1H_ADDR    (PWM3T1_ADDR +  0x00u)
#define     PWM3T1L_ADDR    (PWM3T1H_ADDR + 0x01u)
#define     PWM3T2_ADDR     (PWM3T1L_ADDR + 0x01u)
#define     PWM3T2H_ADDR    (PWM3T2_ADDR +  0x00u)
#define     PWM3T2L_ADDR    (PWM3T2H_ADDR + 0x01u)
#define     PWM3CR_ADDR     (PWM3T2L_ADDR + 0x01u)
#define     PWM3HLD_ADDR    (PWM3CR_ADDR +  0x01u)

#define     PWM4T1_ADDR     (PWM4_BASE + 0x00u)
#define     PWM4T1H_ADDR    (PWM4T1_ADDR +  0x00u)
#define     PWM4T1L_ADDR    (PWM4T1H_ADDR + 0x01u)
#define     PWM4T2_ADDR     (PWM4T1L_ADDR + 0x01u)
#define     PWM4T2H_ADDR    (PWM4T2_ADDR +  0x00u)
#define     PWM4T2L_ADDR    (PWM4T2H_ADDR + 0x01u)
#define     PWM4CR_ADDR     (PWM4T2L_ADDR + 0x01u)
#define     PWM4HLD_ADDR    (PWM4CR_ADDR +  0x01u)

#define     PWM5T1_ADDR     (PWM5_BASE + 0x00u)
#define     PWM5T1H_ADDR    (PWM5T1_ADDR +  0x00u)
#define     PWM5T1L_ADDR    (PWM5T1H_ADDR + 0x01u)
#define     PWM5T2_ADDR     (PWM5T1L_ADDR + 0x01u)
#define     PWM5T2H_ADDR    (PWM5T2_ADDR +  0x00u)
#define     PWM5T2L_ADDR    (PWM5T2H_ADDR + 0x01u)
#define     PWM5CR_ADDR     (PWM5T2L_ADDR + 0x01u)
#define     PWM5HLD_ADDR    (PWM5CR_ADDR +  0x01u)

#define     PWM6T1_ADDR     (PWM6_BASE + 0x00u)
#define     PWM6T1H_ADDR    (PWM6T1_ADDR +  0x00u)
#define     PWM6T1L_ADDR    (PWM6T1H_ADDR + 0x01u)
#define     PWM6T2_ADDR     (PWM6T1L_ADDR + 0x01u)
#define     PWM6T2H_ADDR    (PWM6T2_ADDR +  0x00u)
#define     PWM6T2L_ADDR    (PWM6T2H_ADDR + 0x01u)
#define     PWM6CR_ADDR     (PWM6T2L_ADDR + 0x01u)
#define     PWM6HLD_ADDR    (PWM6CR_ADDR +  0x01u)

#define     PWM7T1_ADDR     (PWM7_BASE + 0x00u)
#define     PWM7T1H_ADDR    (PWM7T1_ADDR +  0x00u)
#define     PWM7T1L_ADDR    (PWM7T1H_ADDR + 0x01u)
#define     PWM7T2_ADDR     (PWM7T1L_ADDR + 0x01u)
#define     PWM7T2H_ADDR    (PWM7T2_ADDR +  0x00u)
#define     PWM7T2L_ADDR    (PWM7T2H_ADDR + 0x01u)
#define     PWM7CR_ADDR     (PWM7T2L_ADDR + 0x01u)
#define     PWM7HLD_ADDR    (PWM7CR_ADDR +  0x01u)

/* PWMCFG offset */
#define     CBIF            0x80u
#define     ETADC           0x40u

/* PWMIF offset */
#define     C7IF            0x80u
#define     C6IF            0x40u
#define     C5IF            0x20u
#define     C4IF            0x10u
#define     C3IF            0x08u
#define     C2IF            0x04u
#define     C1IF            0x02u
#define     C0IF            0x01u

/* PWMFDCR offset */
#define     INVCMP          0x80u
#define     INVIO           0x40u
#define     ENFD            0x20u
#define     FLTFLIO         0x10u
#define     EFDI            0x08u
#define     FDCMP           0x04u
#define     FDIO            0x02u
#define     FDIF            0x01u

/* PWMCR offset */
#define     ENPWM           0x80u
#define     ECBI            0x40u

/* PWM special function registers */
sfr PWMCFG      =   PWMCFG_ADDR;
sfr PWMIF       =   PWMIF_ADDR;
sfr PWMFDCR     =   PWMFDCR_ADDR;
sfr PWMCR       =   PWMCR_ADDR;

#define     PWMC            (*(__IO uint16_t xdata *) PWMC_ADDR)
#define     PWMCH           (*(__IO uint8_t xdata *)  PWMCH_ADDR)
#define     PWMCL           (*(__IO uint8_t xdata *)  PWMCL_ADDR)
#define     PWMCKS          (*(__IO uint8_t xdata *)  PWMCKS_ADDR)
#define     TADCP           (*(__IO uint8_t xdata *)  TADCP_ADDR)
#define     TADCPH          (*(__IO uint8_t xdata *)  TADCPH_ADDR)
#define     TADCPL          (*(__IO uint8_t xdata *)  TADCPL_ADDR)

#define     PWMxT1(PWMxT1_ADDR)      ( *(__IO uint16_t xdata *) PWMxT1_ADDR)
#define     PWMxT2(PWMxT2_ADDR)      ( *(__IO uint16_t xdata *) PWMxT2_ADDR)
#define     PWMxCR(PWMxCR_ADDR)      ( *(__IO uint8_t  xdata *) PWMxCR_ADDR)
#define     PWMxHLD(PWMxHLD_ADDR)    ( *(__IO uint8_t  xdata *)PWMxHLD_ADDR)

#define     PWM0T1          (*(__IO uint16_t xdata *) PWM0T1_ADDR)
#define     PWM0T1H         (*(__IO uint8_t  xdata *) PWM0T1H_ADDR)
#define     PWM0T1L         (*(__IO uint8_t  xdata *) PWM0T1L_ADDR)
#define     PWM0T2          (*(__IO uint16_t xdata *) PWM0T2_ADDR)
#define     PWM0T2H         (*(__IO uint8_t  xdata *) PWM0T2H_ADDR)
#define     PWM0T2L         (*(__IO uint8_t  xdata *) PWM0T2L_ADDR)
#define     PWM0CR          (*(__IO uint8_t  xdata *) PWM0CR_ADDR)
#define     PWM0HLD         (*(__IO uint8_t  xdata *) PWM0HLD_ADDR)

#define     PWM1T1          (*(__IO uint16_t xdata *) PWM1T1_ADDR)
#define     PWM1T1H         (*(__IO uint8_t  xdata *) PWM1T1H_ADDR)
#define     PWM1T1L         (*(__IO uint8_t  xdata *) PWM1T1L_ADDR)
#define     PWM1T2          (*(__IO uint16_t xdata *) PWM1T2_ADDR)
#define     PWM1T2H         (*(__IO uint8_t  xdata *) PWM1T2H_ADDR)
#define     PWM1T2L         (*(__IO uint8_t  xdata *) PWM1T2L_ADDR)
#define     PWM1CR          (*(__IO uint8_t  xdata *) PWM1CR_ADDR)
#define     PWM1HLD         (*(__IO uint8_t  xdata *) PWM1HLD_ADDR)

#define     PWM2T1          (*(__IO uint16_t xdata *) PWM2T1_ADDR)
#define     PWM2T1H         (*(__IO uint8_t  xdata *) PWM2T1H_ADDR)
#define     PWM2T1L         (*(__IO uint8_t  xdata *) PWM2T1L_ADDR)
#define     PWM2T2          (*(__IO uint16_t xdata *) PWM2T2_ADDR)
#define     PWM2T2H         (*(__IO uint8_t  xdata *) PWM2T2H_ADDR)
#define     PWM2T2L         (*(__IO uint8_t  xdata *) PWM2T2L_ADDR)
#define     PWM2CR          (*(__IO uint8_t  xdata *) PWM2CR_ADDR)
#define     PWM2HLD         (*(__IO uint8_t  xdata *) PWM2HLD_ADDR)

#define     PWM3T1          (*(__IO uint16_t xdata *) PWM3T1_ADDR)
#define     PWM3T1H         (*(__IO uint8_t  xdata *) PWM3T1H_ADDR)
#define     PWM3T1L         (*(__IO uint8_t  xdata *) PWM3T1L_ADDR)
#define     PWM3T2          (*(__IO uint16_t xdata *) PWM3T2_ADDR)
#define     PWM3T2H         (*(__IO uint8_t  xdata *) PWM3T2H_ADDR)
#define     PWM3T2L         (*(__IO uint8_t  xdata *) PWM3T2L_ADDR)
#define     PWM3CR          (*(__IO uint8_t  xdata *) PWM3CR_ADDR)
#define     PWM3HLD         (*(__IO uint8_t  xdata *) PWM3HLD_ADDR)

#define     PWM4T1          (*(__IO uint16_t xdata *) PWM4T1_ADDR)
#define     PWM4T1H         (*(__IO uint8_t  xdata *) PWM4T1H_ADDR)
#define     PWM4T1L         (*(__IO uint8_t  xdata *) PWM4T1L_ADDR)
#define     PWM4T2          (*(__IO uint16_t xdata *) PWM4T2_ADDR)
#define     PWM4T2H         (*(__IO uint8_t  xdata *) PWM4T2H_ADDR)
#define     PWM4T2L         (*(__IO uint8_t  xdata *) PWM4T2L_ADDR)
#define     PWM4CR          (*(__IO uint8_t  xdata *) PWM4CR_ADDR)
#define     PWM4HLD         (*(__IO uint8_t  xdata *) PWM4HLD_ADDR)

#define     PWM5T1          (*(__IO uint16_t xdata *) PWM5T1_ADDR)
#define     PWM5T1H         (*(__IO uint8_t  xdata *) PWM5T1H_ADDR)
#define     PWM5T1L         (*(__IO uint8_t  xdata *) PWM5T1L_ADDR)
#define     PWM5T2          (*(__IO uint16_t xdata *) PWM5T2_ADDR)
#define     PWM5T2H         (*(__IO uint8_t  xdata *) PWM5T2H_ADDR)
#define     PWM5T2L         (*(__IO uint8_t  xdata *) PWM5T2L_ADDR)
#define     PWM5CR          (*(__IO uint8_t  xdata *) PWM5CR_ADDR)
#define     PWM5HLD         (*(__IO uint8_t  xdata *) PWM5HLD_ADDR)

#define     PWM6T1          (*(__IO uint16_t xdata *) PWM6T1_ADDR)
#define     PWM6T1H         (*(__IO uint8_t  xdata *) PWM6T1H_ADDR)
#define     PWM6T1L         (*(__IO uint8_t  xdata *) PWM6T1L_ADDR)
#define     PWM6T2          (*(__IO uint16_t xdata *) PWM6T2_ADDR)
#define     PWM6T2H         (*(__IO uint8_t  xdata *) PWM6T2H_ADDR)
#define     PWM6T2L         (*(__IO uint8_t  xdata *) PWM6T2L_ADDR)
#define     PWM6CR          (*(__IO uint8_t  xdata *) PWM6CR_ADDR)
#define     PWM6HLD         (*(__IO uint8_t  xdata *) PWM6HLD_ADDR)

#define     PWM7T1          (*(__IO uint16_t xdata *) PWM7T1_ADDR)
#define     PWM7T1H         (*(__IO uint8_t  xdata *) PWM7T1H_ADDR)
#define     PWM7T1L         (*(__IO uint8_t  xdata *) PWM7T1L_ADDR)
#define     PWM7T2          (*(__IO uint16_t xdata *) PWM7T2_ADDR)
#define     PWM7T2H         (*(__IO uint8_t  xdata *) PWM7T2H_ADDR)
#define     PWM7T2L         (*(__IO uint8_t  xdata *) PWM7T2L_ADDR)
#define     PWM7CR          (*(__IO uint8_t  xdata *) PWM7CR_ADDR)
#define     PWM7HLD         (*(__IO uint8_t  xdata *) PWM7HLD_ADDR)

//! SPI peripherals
/* SPI basic address definition */
#define     SPSTAT_ADDR     0xCD
#define     SPCTL_ADDR      0xCE
#define     SPDAT_ADDR      0xCF

/* Bit definition for SPSTAT */
#define     SPIF            0x80
#define     WCOL            0x40

/* SPCTL offset */
#define     SSIG            0x80
#define     SPEN            0x40
#define     DORD            0x20
#define     MSTR            0x10
#define     CPOL            0x08
#define     CPHA            0x04

/* SPI special function registers */
sfr SPSTAT      =   SPSTAT_ADDR;
sfr SPCTL       =   SPCTL_ADDR;
sfr SPDAT       =   SPDAT_ADDR;


//! I2C peripherals

/* Base address */
#define     I2C_BASE         0xFE80u
#define     I2CCFG_ADDR   (I2C_BASE + 0x00U)
#define     I2CMSCR_ADDR  (I2C_BASE + 0x01U)
#define     I2CMSST_ADDR  (I2C_BASE + 0x02U)
#define     I2CSLCR_ADDR  (I2C_BASE + 0x03U)
#define     I2CSLST_ADDR  (I2C_BASE + 0x04U)
#define     I2CSLADR_ADDR (I2C_BASE + 0x05U)
#define     I2CTXD_ADDR   (I2C_BASE + 0x06U)
#define     I2CRXD_ADDR   (I2C_BASE + 0x07U)

/* I2CCFG */
#define     ENI2C           0x80u
#define     MSSL            0x40u

/* I2CMSCR offset */
#define     EMSI            0x80u

/* I2CMSST offset */
#define     MSBUSY          0x80u
#define     MSIF            0x40u
#define     MSACKI          0x02u
#define     MSACKO          0x01u

/* I2CSLCR offset */
#define     ESTAI           0x40u
#define     ERXI            0x20u
#define     ETXI            0x10u
#define     ESTOI           0x08u
#define     SLRST           0x01u

/* I2CSLST offset */
#define     SLBUSY          0x80u
#define     STAIF           0x40u
#define     RXIF            0x20u
#define     TXIF            0x10u
#define     STOIF           0x08u
#define     TXING           0x04u
#define     SLACKI          0x02u
#define     SLACKO          0x01u

/* I2C special function registers */
#define     I2CCFG          (*(__IO uint8_t xdata *) I2CCFG_ADDR)
#define     I2CMSCR         (*(__IO uint8_t xdata *) I2CMSCR_ADDR)
#define     I2CMSST         (*(__IO uint8_t xdata *) I2CMSST_ADDR)
#define     I2CSLCR         (*(__IO uint8_t xdata *) I2CSLCR_ADDR)
#define     I2CSLST         (*(__IO uint8_t xdata *) I2CSLST_ADDR)
#define     I2CSLADR        (*(__IO uint8_t xdata *) I2CSLADR_ADDR)
#define     I2CTXD          (*(__IO uint8_t xdata *) I2CTXD_ADDR)
#define     I2CRXD          (*(__IO uint8_t xdata *) I2CRXD_ADDR)


/** @} */

/** @} */

/** @} */

#endif

