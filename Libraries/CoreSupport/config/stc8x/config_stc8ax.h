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
sfr ACC         =   (uint8_t)0xe0;
sfr B           =   (uint8_t)0xf0;
sfr PSW         =   (uint8_t)0xd0;                               /*!< program state register */
sfr SP          =   (uint8_t)0x81;
sfr DPL         =   (uint8_t)0x82;
sfr DPH         =   (uint8_t)0x83;
sfr TA          =   (uint8_t)0xae;
sfr DPS         =   (uint8_t)0xe3;
sfr DPL1        =   (uint8_t)0xe4;
sfr DPH1        =   (uint8_t)0xe5;

sbit CY         =   PSW^7;                              /*!< carry/borrow flag bit */
sbit AC         =   PSW^6;                              /*!< auxiliary carry/borrow flag bit */
sbit F0         =   PSW^5;                              /*!< user flag bit 0 */
sbit RS1        =   PSW^4;                              /*!< working state selection bit RS1 */
sbit RS0        =   PSW^3;                              /*!< working state selection bit RS0 */
sbit OV         =   PSW^2;                              /*!< overflow flag bit */
sbit F1         =   PSW^1;                              /*!< user flag bit 1 */
sbit P          =   PSW^0;                              /*!< party check flag bit */

/** @defgroup Peripheral_memory_map
 * @{
 */

//! system management special function register setting 
/* bus speed control register setting */
#define     BUS_SPEED_ADDR              ((uint8_t)0xA1)

/* Bit definition for BUS_SPEED register */
// * RW_S configuration
#define     BUS_SPEED_RW_S              ((uint8_t)0xC0) /*!< RW_S[1:0] bits (control line selection) */
#define     BUS_SPEED_RW_S_0            ((uint8_t)0x40) /*!< Bit 0 */
#define     BUS_SPEED_RW_S_1            ((uint8_t)0x80) /*!< Bit 1 */

#define     BUS_SPEED_RW_S_GROUP1       ((uint8_t)0x00) /*!< select P4.4 pin as RD and P4.3 as WR */
#define     BUS_SPEED_RW_S_GROUP2       ((uint8_t)0x40) /*!< select P3.7 pin as RD and P3.6 as WR */
#define     BUS_SPEED_RW_S_GROUP3       ((uint8_t)0x80) /*!< select P4.2 pin as RD and P4.0 as WR */

// * SPEED configuration
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

/* Bit definition for AUXR register */
// * EXTRAM configuration
#define     AUXR_EXTRAM                 ((uint8_t)0x02) /*!< external RAM access control */

sfr AUXR        =   AUXR_ADDR;      //! auxiliary register

/* peripheral port switch register setting */
#define     PERIPHERAL_PORT_SW1_ADDR    ((uint8_t)0xA2)
#define     PERIPHERAL_PORT_SW2_ADDR    ((uint8_t)0xBA)

/* Bit definition for P_SW1 register */
// * S1_S configuration
#define     P_SW1_S1_S                  ((uint8_t)0xC0) /*!< S1_S[1:0] bits (serial port 1 function pins selection) */
#define     P_SW1_S1_S_0                ((uint8_t)0x40) /*!< Bit 0 TxD */
#define     P_SW1_S1_S_1                ((uint8_t)0x80) /*!< Bit 1 RxD */

#define     P_SW1_S1_S_GROUP1           ((uint8_t)0x00) /*!< select P3.0 pin as RxD and P3.1 as TxD */
#define     P_SW1_S1_S_GROUP2           ((uint8_t)0x40) /*!< select P3.6 pin as RxD and P3.7 as TxD */
#define     P_SW1_S1_S_GROUP3           ((uint8_t)0x80) /*!< select P1.6 pin as RxD and P1.7 as TxD */
#define     P_SW1_S1_S_GROUP4           ((uint8_t)0xC0) /*!< select P4.3 pin as RxD and P4.4 as TxD */

// * CCP_S configuration
#define     P_SW1_CCP_S                 ((uint8_t)0x30) /*!< CCP_S[1:0] bits (PCA function pins selection) */
#define     P_SW1_CCP_S_0               ((uint8_t)0x10) /*!< Bit 0 */
#define     P_SW1_CCP_S_1               ((uint8_t)0x20) /*!< Bit 1 */

#define     P_SW1_CCP_S_GROUP1          ((uint8_t)0x00) /*!< select P1.2 pin as ECI, P1.7 as CPP0, P1.6 as CPP1, P1.5 as CPP2, P1.4 as CPP3 */
#define     P_SW1_CCP_S_GROUP2          ((uint8_t)0x10) /*!< select P2.2 pin as ECI, P2.3 as CPP0, P2.4 as CPP1, P2.5 as CPP2, P2.6 as CPP3 */
#define     P_SW1_CCP_S_GROUP3          ((uint8_t)0x20) /*!< select P7.4 pin as ECI, P7.0 as CPP0, P7.1 as CPP1, P7.2 as CPP2, P7.3 as CPP3 */
#define     P_SW1_CCP_S_GROUP4          ((uint8_t)0x30) /*!< select P3.5 pin as ECI, P3.3 as CPP0, P3.2 as CPP1, P3.1 as CPP2, P3.0 as CPP3 */

// * SPI_S configuration
#define     P_SW1_SPI_S                 ((uint8_t)0x0C) /*!< SPI_S[1:0] bits (SPI function pins selection) */
#define     P_SW1_SPI_S_0               ((uint8_t)0x04) /*!< Bit 0 */
#define     P_SW1_SPI_S_1               ((uint8_t)0x08) /*!< Bit 1 */

#define     P_SW1_SPI_S_GROUP1          ((uint8_t)0x00) /*!< select P1.2 pin as SS, P1.3 as MOSI, P1.4 as MISO, P1.5 as SCLK */
#define     P_SW1_SPI_S_GROUP2          ((uint8_t)0x04) /*!< select P2.2 pin as SS, P2.3 as MOSI, P2.4 as MISO, P2.5 as SCLK */
#define     P_SW1_SPI_S_GROUP3          ((uint8_t)0x08) /*!< select P7.4 pin as SS, P7.5 as MOSI, P7.6 as MISO, P7.7 as SCLK */
#define     P_SW1_SPI_S_GROUP4          ((uint8_t)0x0C) /*!< select P3.5 pin as SS, P3.4 as MOSI, P3.3 as MISO, P3.2 as SCLK */

/* Bit definition for P_SW1 register */
// * EAXFR configuration
#define     P_SW2_EAXFR                 ((uint8_t)0x80) /*!< XFR control register */

// * I2C_S configuration
#define     P_SW2_I2C_S                 ((uint8_t)0x30) /*!< I2C_S[1:0] bits (I2C function pins selection) */
#define     P_SW2_I2C_S_0               ((uint8_t)0x10) /*!< Bit 0 SDA */
#define     P_SW2_I2C_S_1               ((uint8_t)0x20) /*!< Bit 1 SCL */

#define     P_SW2_I2C_S_GROUP1          ((uint8_t)0x00) /*!< select P1.5 pin as SCL, P1.4 as SDA */
#define     P_SW2_I2C_S_GROUP2          ((uint8_t)0x10) /*!< select P2.5 pin as SCL, P2.4 as SDA */
#define     P_SW2_I2C_S_GROUP3          ((uint8_t)0x20) /*!< select P7.7 pin as SCL, P7.6 as SDA */
#define     P_SW2_I2C_S_GROUP4          ((uint8_t)0x30) /*!< select P3.2 pin as SCL, P3.3 as SDA */

// * CMPO_S configuration
#define     P_SW2_CMPO_S                ((uint8_t)0x08) /*!< CMPO selection: [0] P3.4; [1] P4.1 */

// * S4_S configuration
#define     P_SW2_S4_S                  ((uint8_t)0x04) /*!< serial port 4 function pins selection: [0] RxD4=P0.2, TxD4=P0.3; [1] RxD4=P5.2, TxD4=P5.3 */

// * S3_S configuration
#define     P_SW2_S3_S                  ((uint8_t)0x02) /*!< serial port 3 function pins selection: [0] RxD3=P0.0, TxD3=P0.1; [1] RxD3=P5.0, TxD3=P5.1  */

// * S2_S configuration
#define     P_SW2_S2_S                  ((uint8_t)0x01) /*!< serial port 2 function pins selection: [0] RxD2=P1.0, TxD2=P1.1; [1] RxD2=P4.0, TxD2=P4.2  */

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

// * system clock special function register setting in internal expansion RAM 
/* internel oscillator frequency control and adjustment registers*/
#define     IRTRIM_ADDR                 ((uint8_t)0x9F)
#define     LIRTRIM_ADDR                ((uint8_t)0x9E)

/* Bit definition for IRTRIM register */
// user define the value from 0~255 for this register

/* Bit definition for LIRTRIM register */
#define     LIRTRIM_BITS                ((uint8_t)0x03)
#define     LIRTRIM_BITS_0              ((uint8_t)0x01)
#define     LIRTRIM_BITS_1              ((uint8_t)0x02)

#define     LIRTRIM_BITS_FT000          ((uint8_t)0x00) /*!< on fine-turning */
#define     LIRTRIM_BITS_FT001          ((uint8_t)0x01) /*!< Internal high accurate IRC fine-turning 0.01% */
#define     LIRTRIM_BITS_FT004          ((uint8_t)0x02) /*!< Internal high accurate IRC fine-turning 0.04% */
#define     LIRTRIM_BITS_FT010          ((uint8_t)0x03) /*!< Internal high accurate IRC fine-turning 0.10% */


sfr IRTRIM      =   IRTRIM_ADDR;    //! internal IRC frequency adjustment register
sfr LIRTRIM     =   LIRTRIM_ADDR;   //! internal IRC frequency subtle adjustment register

/* system clock base address */
#define     SYSCLK_BASE                 ((uint16_t)0xFE00)

#define     CKSEL_ADDR                  (SYSCLK_BASE + 0x0000)
#define     CLKDIV_ADDR                 (SYSCLK_BASE + 0x0001)
#define     IRC24MCR_ADDR               (SYSCLK_BASE + 0x0002)
#define     XOSCCR_ADDR                 (SYSCLK_BASE + 0x0003)
#define     IRC32KCR_ADDR               (SYSCLK_BASE + 0x0004)

/* Bit definition for CKSEL register */
// * MCLKODIV configuration
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

// * MCLKO_S configuration
#define     CKSEL_MCLKO_S               ((uint8_t)0x08) /*!< divided MCLK output pin: [0] P5.4; [1] P1.6 */

// * MCKSEL configuration
#define     CKSEL_MCKSEL                ((uint8_t)0x03) /*!< MCLKODIV[1:0] bits (system clock output division coefficient) */
#define     CKSEL_MCKSEL_0              ((uint8_t)0x01) /*!< Bit 0 */
#define     CKSEL_MCKSEL_1              ((uint8_t)0x02) /*!< Bit 1 */

#define     CKSEL_MCKSEL_IRC24MCR       ((uint8_t)0x00) /*!< select internal 24MHz High accurate IRC */
#define     CKSEL_MCKSEL_XOSCCR_O       ((uint8_t)0x01) /*!< select outer clock input */
#define     CKSEL_MCKSEL_XOSCCR_C       ((uint8_t)0x02) /*!< select outer clock input */
#define     CKSEL_MCKSEL_IRC32KST       ((uint8_t)0x03) /*!< select internal 32kHz low speed IRC */

/* Bit definition for CLKDIV register */
// user define the value from 0~255 for this register. Maximum 0.55% per bit, minium 0.02% per bit, average 0.24% per bit

/* Bit definition for IRC24MCR register */
// * ENIRC24M configuration
#define     IRC24MCR_ENIRC24M           ((uint8_t)0x80) /*!< IRC24MCR enabling control */

// * IRC24MST configuration
#define     IRC24MCR_IRC24MST           ((uint8_t)0x01) /*!< IRC24MCR stable state flag */

/* Bit definition for XOSCCR register */
// * ENXOSC configuration
#define     XOSCCR_ENXOSC               ((uint8_t)0x80) /*!< external oscillator enabling control */

// * XITYPE configuration
#define     XOSCCR_XITYPE               ((uint8_t)0x40) /*!< external oscillator type selection */

// * XOSCST configuration
#define     XOSCCR_XOSCST               ((uint8_t)0x01) /*!< external oscillator stable state flag */

/* Bit definition for IRC32KCR register */
// * ENIRC32K configuration
#define     IRC24MCR_ENIRC32K           ((uint8_t)0x80) /*!< IRC32KCR enabling control */

// * IRC32KST configuration
#define     IRC24MCR_IRC32KST           ((uint8_t)0x01) /*!< IRC32KCR stable state flag */

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
#define     PCON_ADDR                   ((uint8_t)0x87)
#define     VOCTRL_ADDR                 ((uint8_t)0xBB)

/* Bit definition for PCON register */
// * LVDF configuration
#define     PCON_LVDF                   ((uint8_t)0x20)     /*!< low voltage check flag bit */

// * POF configuration
#define     PCON_POF                    ((uint8_t)0x10)     /*!< Power-on reset flag bit */

// * PD configuration
#define     PCON_PD                     ((uint8_t)0x02)     /*!< peripherals stopping mode control bit */

// * IDL configuration
#define     PCON_IDL                    ((uint8_t)0x01)     /*!< peripherals IDLE mode control bit */

/* Bit definition for VOCTRL register */
// * SCC configuration
#define     VOCTRL_SCC                   ((uint8_t)0x80)     /*!< static current control bit: [0] internal circuit 1.5uA; [1] external circuit lower than 0.15uA */

sfr PCON        =   PCON_ADDR;
sfr VOCTRL      =   VOCTRL_ADDR;

//! system reset special function register control
/* reset control base address */
#define     WDT_CONTR_ADDR              ((uint8_t)0xC1u)
// #define     IAP_ADDR      0xC7u
#define     RSTCFG_ADDR                 ((uint8_t)0xFFu)

/* Bit definition for WDT_CONTR register */
// * WDT_FLAG configuration
#define     WDT_CONTR_WDT_FLAG          ((uint8_t)0x80)     /*!< Watch dog overflow flag */

// * EN_WDT configuration
#define     WDT_CONTR_EN_WDT            ((uint8_t)0x20)     /*!< Watch dog enabling bit：[0] off; [1] on */

// * CLR_WDT configuration
#define     WDT_CONTR_CLR_WDT           ((uint8_t)0x10)     /*!< Watch dog timer auto clearance by firmware：[0] off; [1] on */

// * IDL_WDT configuration
#define     WDT_CONTR_IDL_WDT           ((uint8_t)0x08)     /*!< Watch dog counting function control on IDLE model [0] stop; [1] resume */

// * WDT_PS configuration
#define     WDT_CONTR_WDT_PS            ((uint8_t)0x07)     /*!< WDT_PS[2:0] bits (watch dog timer clock division coefficient) */
#define     WDT_CONTR_WDT_PS0           ((uint8_t)0x01)     /*!< Bit 0 */
#define     WDT_CONTR_WDT_PS1           ((uint8_t)0x02)     /*!< Bit 1 */
#define     WDT_CONTR_WDT_PS2           ((uint8_t)0x04)     /*!< Bit 2 */

/**
 * @note  watch dog overflow time = (12x32768x2^(WDT_PS+1))/SYSCLK
 */
#define     WDT_CONTR_WDT_PS_DIV2       ((uint8_t)0x00)     /*!< Watch dog input clock divided by 2 */
#define     WDT_CONTR_WDT_PS_DIV4       ((uint8_t)0x01)     /*!< Watch dog input clock divided by 4 */
#define     WDT_CONTR_WDT_PS_DIV8       ((uint8_t)0x02)     /*!< Watch dog input clock divided by 8 */
#define     WDT_CONTR_WDT_PS_DIV16      ((uint8_t)0x03)     /*!< Watch dog input clock divided by 16 */
#define     WDT_CONTR_WDT_PS_DIV32      ((uint8_t)0x04)     /*!< Watch dog input clock divided by 32 */
#define     WDT_CONTR_WDT_PS_DIV64      ((uint8_t)0x05)     /*!< Watch dog input clock divided by 64 */
#define     WDT_CONTR_WDT_PS_DIV128     ((uint8_t)0x06)     /*!< Watch dog input clock divided by 128 */
#define     WDT_CONTR_WDT_PS_DIV256     ((uint8_t)0x07)     /*!< Watch dog input clock divided by 256 */

/* Bit definition for RSTCFG register */
// * ENLVR configuration
#define     RSTCFG_ENLVR                ((uint8_t)0x40)     /*!< low voltage reset control bit */

// * P54RST configuration
#define     RSTCFG_P54RST               ((uint8_t)0x10)     /*!< RST pin selection: [0] regard P5.4 as normal pin; [1]: regard P5.4 as RST */

// * LVDS configuration
#define     RSTCFG_LVDS                 ((uint8_t)0x03)     /*!< WDT_PS[1:0] bits（low voltage threshold selection） */
#define     RSTCFG_LVDS_0               ((uint8_t)0x01)     /*!< Bit 0 */
#define     RSTCFG_LVDS_1               ((uint8_t)0x02)     /*!< Bit 1 */

#define     RSTCFG_LVDS_2V2             ((uint8_t)0x00)     /*!< low voltage threshold 2.2V */
#define     RSTCFG_LVDS_2V4             ((uint8_t)0x01)     /*!< low voltage threshold 2.4V */
#define     RSTCFG_LVDS_2V7             ((uint8_t)0x02)     /*!< low voltage threshold 2.7V */
#define     RSTCFG_LVDS_3V0             ((uint8_t)0x03)     /*!< low voltage threshold 3.0V */

sfr WDT_CONTR   =   WDT_CONTR_ADDR;
// sfr IAP_CONTR   =   IAP_ADDR;
sfr RSTCFG      =   RSTCFG_ADDR;

/* wake up timer base address */
#define     WKT_BASE                    ((uint8_t)0xAA)
#define     WKTCL_ADDR                  (WKT_BASE + 0x0000u)
#define     WKTCH_ADDR                  (WKT_BASE + 0x0001u)

/* WKTCH offset */
#define WKTEN       0x80

sfr WKTCL       =   WKTCL_ADDR;
sfr WKTCH       =   WKTCH_ADDR;

//! GPIO peripherals
#define     GPIO_BASE       ((uint8_t)0x80)
#define     PxM0_BASE       ((uint8_t)0x94)
#define     PxM1_BASE       ((uint8_t)0x93)
#define     PxPU_BASE       ((uint16_t)0xFE10)
#define     PxNCS_BASE      ((uint16_t)0xFE18)

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
#define     P1M0_ADDR       ((uint8_t)0x92)
#define     P2M0_ADDR       ((uint8_t)0x96)
#define     P3M0_ADDR       ((uint8_t)0xB2)
#define     P4M0_ADDR       ((uint8_t)0xB4)
#define     P5M0_ADDR       ((uint8_t)0xCA)
#define     P6M0_ADDR       ((uint8_t)0xCC)
#define     P7M0_ADDR       ((uint8_t)0xE2)

/* PxM1: GPIO configuration registers 1 address definition */
#define     P0M1_ADDR       PxM1_BASE 
#define     P1M1_ADDR       ((uint8_t)0x91)
#define     P2M1_ADDR       ((uint8_t)0x95)
#define     P3M1_ADDR       ((uint8_t)0xB1)
#define     P4M1_ADDR       ((uint8_t)0xB3)
#define     P5M1_ADDR       ((uint8_t)0xC9)
#define     P6M1_ADDR       ((uint8_t)0xCB)
#define     P7M1_ADDR       ((uint8_t)0xE1)

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

/**
 * @note GPIO mode selection
 * \verbatim 
 * | PnM1.x | PnM0.x |         Pn.x Mode         |
 * |   0    |   0    | quasi- bidirectional port | ([weak pull up] up to 20mA constant-current; pull current between 150~270uA)
 * |   0    |   1    |      push-pul output      | ([strong pull up] up to 20mA contant-current; current-limited resistor is needed)
 * |   1    |   0    |   high impedance output   | (no current)
 * |   1    |   1    |     open drain output     | (internal pull up resistor disconnected; external pull up resistor is needed)
 * \endverbatim
 */

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
// * GPIO pull up resistor control registers: [0] off; [1] on*/
#define     P0PU        ((__IO uint8_t xdata *) P0PU_ADDR)             // Extended SFR
#define     P1PU        ((__IO uint8_t xdata *) P1PU_ADDR)             // Extended SFR
#define     P2PU        ((__IO uint8_t xdata *) P2PU_ADDR)             // Extended SFR
#define     P3PU        ((__IO uint8_t xdata *) P3PU_ADDR)             // Extended SFR
#define     P4PU        ((__IO uint8_t xdata *) P4PU_ADDR)             // Extended SFR
#define     P5PU        ((__IO uint8_t xdata *) P5PU_ADDR)             // Extended SFR
#define     P6PU        ((__IO uint8_t xdata *) P6PU_ADDR)             // Extended SFR
#define     P7PU        ((__IO uint8_t xdata *) P7PU_ADDR)             // Extended SFR

// * GPIO Schmitt trigger control registers: [0] off; [1] on */
#define     P0NCS       ((__IO uint8_t xdata *) P1NCS_ADDR)            // Extended SFR
#define     P1NCS       ((__IO uint8_t xdata *) P2NCS_ADDR)            // Extended SFR
#define     P2NCS       ((__IO uint8_t xdata *) P3NCS_ADDR)            // Extended SFR
#define     P3NCS       ((__IO uint8_t xdata *) P4NCS_ADDR)            // Extended SFR
#define     P4NCS       ((__IO uint8_t xdata *) P5NCS_ADDR)            // Extended SFR
#define     P5NCS       ((__IO uint8_t xdata *) P6NCS_ADDR)            // Extended SFR
#define     P6NCS       ((__IO uint8_t xdata *) P7NCS_ADDR)            // Extended SFR
#define     P7NCS       ((__IO uint8_t xdata *) P8NCS_ADDR)            // Extended SFR

//! ISR peripherals
/* ISR base address */
#define    IE_ADDR          ((uint8_t)0xA8)
#define    IE2_ADDR         ((uint8_t)0xAF)
#define    IP_ADDR          ((uint8_t)0xB8)
#define    IP2_ADDR         ((uint8_t)0xB5)
#define    IPH_ADDR         ((uint8_t)0xB7)
#define    IP2H_ADDR        ((uint8_t)0xB6)
#define    INTCLKO_ADDR     ((uint8_t)0x8F)
#define    AUXINTIF_ADDR    ((uint8_t)0xEF)

/* IE2 offset */
#define     ET4             ((uint8_t)0x40)
#define     ET3             ((uint8_t)0x20)
#define     ES4             ((uint8_t)0x10)
#define     ES3             ((uint8_t)0x08)
#define     ET2             ((uint8_t)0x04)
#define     ESPI            ((uint8_t)0x02)
#define     ES2             ((uint8_t)0x01)

/* IP2 offset */
#define     PI2C            ((uint8_t)0x40)
#define     PCMP            ((uint8_t)0x20)
#define     PX4             ((uint8_t)0x10)
#define     PPWMFD          ((uint8_t)0x08)
#define     PPWM            ((uint8_t)0x04)
#define     PSPI            ((uint8_t)0x02)
#define     PS2             ((uint8_t)0x01)

/* IPH offset */
#define     PPCAH           ((uint8_t)0x80)
#define     PLVDH           ((uint8_t)0x40)
#define     PADCH           ((uint8_t)0x20)
#define     PSH             ((uint8_t)0x10)
#define     PT1H            ((uint8_t)0x08)
#define     PX1H            ((uint8_t)0x04)
#define     PT0H            ((uint8_t)0x02)
#define     PX0H            ((uint8_t)0x01)

/* IP2H offset */
#define     PI2CH           ((uint8_t)0x40)
#define     PCMPH           ((uint8_t)0x20)
#define     PX4H            ((uint8_t)0x10)
#define     PPWMFDH         ((uint8_t)0x08)
#define     PPWMH           ((uint8_t)0x04)
#define     PSPIH           ((uint8_t)0x02)
#define     PS2H            ((uint8_t)0x01)

/* INTCLKO offset */
#define     EX4             ((uint8_t)0x40)
#define     EX3             ((uint8_t)0x20)
#define     EX2             ((uint8_t)0x10)
#define     T2CLKO          ((uint8_t)0x04)
#define     T1CLKO          ((uint8_t)0x02)
#define     T0CLKO          ((uint8_t)0x01)

/* AUXINTIF offset */
#define     INT4IF          ((uint8_t)0x40)
#define     INT3IF          ((uint8_t)0x20)
#define     INT2IF          ((uint8_t)0x10)
#define     T4IF            ((uint8_t)0x04)
#define     T3IF            ((uint8_t)0x02)
#define     T2IF            ((uint8_t)0x01)

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
#define     Timer_BASE      ((uint8_t)0x88)
#define     T2T3T4_BASE     ((uint8_t)0xD1)
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
#define     T1_GATE         ((uint8_t)0x80)
#define     T1_CT           ((uint8_t)0x40)
#define     T1_M1           ((uint8_t)0x20)
#define     T1_M0           ((uint8_t)0x10)
#define     T0_GATE         ((uint8_t)0x08)
#define     T0_CT           ((uint8_t)0x04)
#define     T0_M1           ((uint8_t)0x02)
#define     T0_M0           ((uint8_t)0x01)

/* T4T3M offset */
#define     T4R             ((uint8_t)0x80)
#define     T4_CT           ((uint8_t)0x40)
#define     T4x12           ((uint8_t)0x20)
#define     T4CLKO          ((uint8_t)0x10)
#define     T3R             ((uint8_t)0x08)
#define     T3_CT           ((uint8_t)0x04)
#define     T3x12           ((uint8_t)0x02)
#define     T3CLKO          ((uint8_t)0x01)

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
#define     SCON_ADDR       ((uint8_t)0x98)
#define     SBUF_ADDR       ((uint8_t)0x99)
#define     S2CON_ADDR      ((uint8_t)0x9A)
#define     S2BUF_ADDR      ((uint8_t)0x9B)
#define     S3CON_ADDR      ((uint8_t)0xAC)
#define     S3BUF_ADDR      ((uint8_t)0xAD)
#define     S4CON_ADDR      ((uint8_t)0x84)
#define     S4BUF_ADDR      ((uint8_t)0x85)
#define     SADDR_ADDR      ((uint8_t)0xA9)
#define     SADEN_ADDR      ((uint8_t)0xB9)

/* S2CON offset */
#define     S2SM0           ((uint8_t)0x80)
#define     S2ST4           ((uint8_t)0x40)
#define     S2SM2           ((uint8_t)0x20)
#define     S2REN           ((uint8_t)0x10)
#define     S2TB8           ((uint8_t)0x08)
#define     S2RB8           ((uint8_t)0x04)
#define     S2TI            ((uint8_t)0x02)
#define     S2RI            ((uint8_t)0x01)

/* S3CON offset */
#define     S3SM0           ((uint8_t)0x80)
#define     S3ST4           ((uint8_t)0x40)
#define     S3SM2           ((uint8_t)0x20)
#define     S3REN           ((uint8_t)0x10)
#define     S3TB8           ((uint8_t)0x08)
#define     S3RB8           ((uint8_t)0x04)
#define     S3TI            ((uint8_t)0x02)
#define     S3RI            ((uint8_t)0x01)

/* S4CON offset */
#define     S4SM0           ((uint8_t)0x80)
#define     S4ST4           ((uint8_t)0x40)
#define     S4SM2           ((uint8_t)0x20)
#define     S4REN           ((uint8_t)0x10)
#define     S4TB8           ((uint8_t)0x08)
#define     S4RB8           ((uint8_t)0x04)
#define     S4TI            ((uint8_t)0x02)
#define     S4RI            ((uint8_t)0x01)

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
#define     CMPCR1_ADDR     ((uint8_t)0xE6)
#define     CMPCR2_ADDR     ((uint8_t)0xE7)

/* CMPCR1 offset */
#define     CMPEN           ((uint8_t)0x80)
#define     CMPIF           ((uint8_t)0x40)
#define     PIE             ((uint8_t)0x20)
#define     NIE             ((uint8_t)0x10)
#define     PIS             ((uint8_t)0x08)
#define     NIS             ((uint8_t)0x04)
#define     CMPOE           ((uint8_t)0x02)
#define     CMPRES          ((uint8_t)0x01)

/* CMPAR2 offset */
#define     INVCMPO         ((uint8_t)0x80
#define     DISFLT          ((uint8_t)0x40

sfr CMPCR1      =   CMPCR1_ADDR;
sfr CMPCR2      =   CMPCR2_ADDR;

//! EEPROM peripherals
/* EEPROM basic address definition */
#define     IAP_DATA_ADDR   ((uint8_t)0xC2)
#define     IAP_ADDRH_ADDR  ((uint8_t)0xC3)
#define     IAP_ADDRL_ADDR  ((uint8_t)0xC4)
#define     IAP_CMD_ADDR    ((uint8_t)0xC5)
#define     IAP_TRIG_ADDR   ((uint8_t)0xC6)
#define     IAP_CONTR_ADDR  ((uint8_t)0xC7)

/* IAP_CONTR offset */
#define     IAPEN           ((uint8_t)0x80)
#define     SWBS            ((uint8_t)0x40)
#define     SWRST           ((uint8_t)0x20)
#define     CMD_FAIL        ((uint8_t)0x10)

/* IAP special function registers */
sfr IAP_DATA    =   IAP_DATA_ADDR;
sfr IAP_ADDRH   =   IAP_ADDRH_ADDR;
sfr IAP_ADDRL   =   IAP_ADDRL_ADDR;
sfr IAP_CMD     =   IAP_CMD_ADDR;
sfr IAP_TRIG    =   IAP_TRIG_ADDR;
sfr IAP_CONTR   =   IAP_CONTR_ADDR;

//! ADC peripherals
/* ADC basic address definition */
#define     ADC_CONTR_ADDR  ((uint8_t)0xBC)
#define     ADC_RESH_ADDR   ((uint8_t)0xBD)
#define     ADC_RESL_ADDR   ((uint8_t)0xBE)
#define     ADCCFG_ADDR     ((uint8_t)0xDE)

/* ADC_CONTR offset */
#define     ADC_POWER       ((uint8_t)0x80)
#define     ADC_START       ((uint8_t)0x40)
#define     ADC_FLAG        ((uint8_t)0x20)

/* ADCCFG offset */
#define     ADC_RESFMT      ((uint8_t)0x20)

/* ADC special function registers */
sfr ADC_CONTR   =   ADC_CONTR_ADDR;
sfr ADC_RESH    =   ADC_RESH_ADDR;
sfr ADC_RESL    =   ADC_RESL_ADDR;
sfr ADCCFG      =   ADCCFG_ADDR;

//! PCA peripherals
/* PCA basic address definition */
#define     CCON_ADDR       ((uint8_t)0xD8)
#define     CMOD_ADDR       ((uint8_t)0xD9)
#define     CL_ADDR         ((uint8_t)0xE9)
#define     CH_ADDR         ((uint8_t)0xF9)
#define     CCAPM0_ADDR     ((uint8_t)0xDA)
#define     CCAPM1_ADDR     ((uint8_t)0xDB)
#define     CCAPM2_ADDR     ((uint8_t)0xDC)
#define     CCAPM3_ADDR     ((uint8_t)0xDD)
#define     CCAP0L_ADDR     ((uint8_t)0xEA)
#define     CCAP1L_ADDR     ((uint8_t)0xEB)
#define     CCAP2L_ADDR     ((uint8_t)0xEC)
#define     CCAP3L_ADDR     ((uint8_t)0xED)
#define     CCAP0H_ADDR     ((uint8_t)0xFA)
#define     CCAP1H_ADDR     ((uint8_t)0xFB)
#define     CCAP2H_ADDR     ((uint8_t)0xFC)
#define     CCAP3H_ADDR     ((uint8_t)0xFD)
#define     PCA_PWM0_ADDR   ((uint8_t)0xF2)
#define     PCA_PWM1_ADDR   ((uint8_t)0xF3)
#define     PCA_PWM2_ADDR   ((uint8_t)0xF4)
#define     PCA_PWM3_ADDR   ((uint8_t)0xF5)

/* CMOD offset */
#define     CIDL            ((uint8_t)0x80)
#define     ECF             ((uint8_t)0x01)

/* CCAPM0 offset */
#define     ECOM0           ((uint8_t)0x40)
#define     CCAPP0          ((uint8_t)0x20)
#define     CCAPN0          ((uint8_t)0x10)
#define     MAT0            ((uint8_t)0x08)
#define     TOG0            ((uint8_t)0x04)
#define     PWM0            ((uint8_t)0x02)
#define     ECCF0           ((uint8_t)0x01)

/* CCAPM1 offset */
#define     ECOM1           ((uint8_t)0x40)
#define     CCAPP1          ((uint8_t)0x20)
#define     CCAPN1          ((uint8_t)0x10)
#define     MAT1            ((uint8_t)0x08)
#define     TOG1            ((uint8_t)0x04)
#define     PWM1            ((uint8_t)0x02)
#define     ECCF1           ((uint8_t)0x01)

/* CCAMP2 offset */
#define     ECOM2           ((uint8_t)0x40)
#define     CCAPP2          ((uint8_t)0x20)
#define     CCAPN2          ((uint8_t)0x10)
#define     MAT2            ((uint8_t)0x08)
#define     TOG2            ((uint8_t)0x04)
#define     PWM2            ((uint8_t)0x02)
#define     ECCF2           ((uint8_t)0x01)

/* CCAPM3 offset */
#define     ECOM3           ((uint8_t)0x40)
#define     CCAPP3          ((uint8_t)0x20)
#define     CCAPN3          ((uint8_t)0x10)
#define     MAT3            ((uint8_t)0x08)
#define     TOG3            ((uint8_t)0x04)
#define     PWM3            ((uint8_t)0x02)
#define     ECCF3           ((uint8_t)0x01)

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
#define     PWMCFG_ADDR     ((uint8_t)0xF1)
#define     PWMIF_ADDR      ((uint8_t)0xF6)
#define     PWMFDCR_ADDR    ((uint8_t)0xF7)
#define     PWMCR_ADDR      ((uint8_t)0xFE)

#define     PWM_BASE        ((uint16_t)0xFFF0)
#define     PWM0_BASE       ((uint16_t)0xFF00)
#define     PWM1_BASE       ((uint16_t)0xFF10)
#define     PWM2_BASE       ((uint16_t)0xFF20)
#define     PWM3_BASE       ((uint16_t)0xFF30)
#define     PWM4_BASE       ((uint16_t)0xFF40)
#define     PWM5_BASE       ((uint16_t)0xFF50)
#define     PWM6_BASE       ((uint16_t)0xFF60)
#define     PWM7_BASE       ((uint16_t)0xFF70)

#define     PWMC_ADDR       (PWM_BASE + 0x00)
#define     PWMCH_ADDR      (PWMC_ADDR +  0x00)
#define     PWMCL_ADDR      (PWMCH_ADDR + 0x01)
#define     PWMCKS_ADDR     (PWMCL_ADDR + 0x01)

#define     TADCP_ADDR      (PWMCKS_ADDR + 0x01)
#define     TADCPH_ADDR     (TADCP_ADDR +  0x00)
#define     TADCPL_ADDR     (TADCPH_ADDR + 0x01)

#define     PWM0T1_ADDR     (PWM0_BASE + 0x00)
#define     PWM0T1H_ADDR    (PWM0T1_ADDR +  0x00)
#define     PWM0T1L_ADDR    (PWM0T1H_ADDR + 0x01)
#define     PWM0T2_ADDR     (PWM0T1L_ADDR + 0x01)
#define     PWM0T2H_ADDR    (PWM0T2_ADDR +  0x00)
#define     PWM0T2L_ADDR    (PWM0T2H_ADDR + 0x01)
#define     PWM0CR_ADDR     (PWM0T2L_ADDR + 0x01)
#define     PWM0HLD_ADDR    (PWM0CR_ADDR +  0x01)

#define     PWM1T1_ADDR     (PWM1_BASE + 0x00)
#define     PWM1T1H_ADDR    (PWM1T1_ADDR +  0x00)
#define     PWM1T1L_ADDR    (PWM1T1H_ADDR + 0x01)
#define     PWM1T2_ADDR     (PWM1T1L_ADDR + 0x01)
#define     PWM1T2H_ADDR    (PWM1T2_ADDR +  0x00)
#define     PWM1T2L_ADDR    (PWM1T2H_ADDR + 0x01)
#define     PWM1CR_ADDR     (PWM1T2L_ADDR + 0x01)
#define     PWM1HLD_ADDR    (PWM1CR_ADDR +  0x01)

#define     PWM2T1_ADDR     (PWM2_BASE + 0x00)
#define     PWM2T1H_ADDR    (PWM2T1_ADDR +  0x00)
#define     PWM2T1L_ADDR    (PWM2T1H_ADDR + 0x01)
#define     PWM2T2_ADDR     (PWM2T1L_ADDR + 0x01)
#define     PWM2T2H_ADDR    (PWM2T2_ADDR +  0x00)
#define     PWM2T2L_ADDR    (PWM2T2H_ADDR + 0x01)
#define     PWM2CR_ADDR     (PWM2T2L_ADDR + 0x01)
#define     PWM2HLD_ADDR    (PWM2CR_ADDR +  0x01)

#define     PWM3T1_ADDR     (PWM3_BASE + 0x00)
#define     PWM3T1H_ADDR    (PWM3T1_ADDR +  0x00)
#define     PWM3T1L_ADDR    (PWM3T1H_ADDR + 0x01)
#define     PWM3T2_ADDR     (PWM3T1L_ADDR + 0x01)
#define     PWM3T2H_ADDR    (PWM3T2_ADDR +  0x00)
#define     PWM3T2L_ADDR    (PWM3T2H_ADDR + 0x01)
#define     PWM3CR_ADDR     (PWM3T2L_ADDR + 0x01)
#define     PWM3HLD_ADDR    (PWM3CR_ADDR +  0x01)

#define     PWM4T1_ADDR     (PWM4_BASE + 0x00)
#define     PWM4T1H_ADDR    (PWM4T1_ADDR +  0x00)
#define     PWM4T1L_ADDR    (PWM4T1H_ADDR + 0x01)
#define     PWM4T2_ADDR     (PWM4T1L_ADDR + 0x01)
#define     PWM4T2H_ADDR    (PWM4T2_ADDR +  0x00)
#define     PWM4T2L_ADDR    (PWM4T2H_ADDR + 0x01)
#define     PWM4CR_ADDR     (PWM4T2L_ADDR + 0x01)
#define     PWM4HLD_ADDR    (PWM4CR_ADDR +  0x01)

#define     PWM5T1_ADDR     (PWM5_BASE + 0x00)
#define     PWM5T1H_ADDR    (PWM5T1_ADDR +  0x00)
#define     PWM5T1L_ADDR    (PWM5T1H_ADDR + 0x01)
#define     PWM5T2_ADDR     (PWM5T1L_ADDR + 0x01)
#define     PWM5T2H_ADDR    (PWM5T2_ADDR +  0x00)
#define     PWM5T2L_ADDR    (PWM5T2H_ADDR + 0x01)
#define     PWM5CR_ADDR     (PWM5T2L_ADDR + 0x01)
#define     PWM5HLD_ADDR    (PWM5CR_ADDR +  0x01)

#define     PWM6T1_ADDR     (PWM6_BASE + 0x00)
#define     PWM6T1H_ADDR    (PWM6T1_ADDR +  0x00)
#define     PWM6T1L_ADDR    (PWM6T1H_ADDR + 0x01)
#define     PWM6T2_ADDR     (PWM6T1L_ADDR + 0x01)
#define     PWM6T2H_ADDR    (PWM6T2_ADDR +  0x00)
#define     PWM6T2L_ADDR    (PWM6T2H_ADDR + 0x01)
#define     PWM6CR_ADDR     (PWM6T2L_ADDR + 0x01)
#define     PWM6HLD_ADDR    (PWM6CR_ADDR +  0x01)

#define     PWM7T1_ADDR     (PWM7_BASE + 0x00)
#define     PWM7T1H_ADDR    (PWM7T1_ADDR +  0x00)
#define     PWM7T1L_ADDR    (PWM7T1H_ADDR + 0x01)
#define     PWM7T2_ADDR     (PWM7T1L_ADDR + 0x01)
#define     PWM7T2H_ADDR    (PWM7T2_ADDR +  0x00)
#define     PWM7T2L_ADDR    (PWM7T2H_ADDR + 0x01)
#define     PWM7CR_ADDR     (PWM7T2L_ADDR + 0x01)
#define     PWM7HLD_ADDR    (PWM7CR_ADDR +  0x01)

/* PWMCFG offset */
#define     CBIF            ((uint8_t)0x80)
#define     ETADC           ((uint8_t)0x40)

/* PWMIF offset */
#define     C7IF            ((uint8_t)0x80)
#define     C6IF            ((uint8_t)0x40)
#define     C5IF            ((uint8_t)0x20)
#define     C4IF            ((uint8_t)0x10)
#define     C3IF            ((uint8_t)0x08)
#define     C2IF            ((uint8_t)0x04)
#define     C1IF            ((uint8_t)0x02)
#define     C0IF            ((uint8_t)0x01)

/* PWMFDCR offset */
#define     INVCMP          ((uint8_t)0x80)
#define     INVIO           ((uint8_t)0x40)
#define     ENFD            ((uint8_t)0x20)
#define     FLTFLIO         ((uint8_t)0x10)
#define     EFDI            ((uint8_t)0x08)
#define     FDCMP           ((uint8_t)0x04)
#define     FDIO            ((uint8_t)0x02)
#define     FDIF            ((uint8_t)0x01)

/* PWMCR offset */
#define     ENPWM           ((uint8_t)0x80)
#define     ECBI            ((uint8_t)0x40)

/* PWM special function registers */
sfr PWMCFG      =   PWMCFG_ADDR;
sfr PWMIF       =   PWMIF_ADDR;
sfr PWMFDCR     =   PWMFDCR_ADDR;
sfr PWMCR       =   PWMCR_ADDR;

#define     PWMC            ((__IO uint16_t xdata *) PWMC_ADDR)
#define     PWMCH           ((__IO uint8_t xdata *)  PWMCH_ADDR)
#define     PWMCL           ((__IO uint8_t xdata *)  PWMCL_ADDR)
#define     PWMCKS          ((__IO uint8_t xdata *)  PWMCKS_ADDR)
#define     TADCP           ((__IO uint8_t xdata *)  TADCP_ADDR)
#define     TADCPH          ((__IO uint8_t xdata *)  TADCPH_ADDR)
#define     TADCPL          ((__IO uint8_t xdata *)  TADCPL_ADDR)

#define     PWMxT1(PWMxT1_ADDR)      ( *(__IO uint16_t xdata *) PWMxT1_ADDR)
#define     PWMxT2(PWMxT2_ADDR)      ( *(__IO uint16_t xdata *) PWMxT2_ADDR)
#define     PWMxCR(PWMxCR_ADDR)      ( *(__IO uint8_t  xdata *) PWMxCR_ADDR)
#define     PWMxHLD(PWMxHLD_ADDR)    ( *(__IO uint8_t  xdata *)PWMxHLD_ADDR)

#define     PWM0T1          ((__IO uint16_t xdata *) PWM0T1_ADDR)
#define     PWM0T1H         ((__IO uint8_t xdata *) PWM0T1H_ADDR)
#define     PWM0T1L         ((__IO uint8_t xdata *) PWM0T1L_ADDR)
#define     PWM0T2          ((__IO uint16_t xdata *) PWM0T2_ADDR)
#define     PWM0T2H         ((__IO uint8_t xdata *) PWM0T2H_ADDR)
#define     PWM0T2L         ((__IO uint8_t xdata *) PWM0T2L_ADDR)
#define     PWM0CR          ((__IO uint8_t xdata *) PWM0CR_ADDR)
#define     PWM0HLD         ((__IO uint8_t xdata *) PWM0HLD_ADDR)

#define     PWM1T1          ((__IO uint16_t xdata *) PWM1T1_ADDR)
#define     PWM1T1H         ((__IO uint8_t xdata *) PWM1T1H_ADDR)
#define     PWM1T1L         ((__IO uint8_t xdata *) PWM1T1L_ADDR)
#define     PWM1T2          ((__IO uint16_t xdata *) PWM1T2_ADDR)
#define     PWM1T2H         ((__IO uint8_t xdata *) PWM1T2H_ADDR)
#define     PWM1T2L         ((__IO uint8_t xdata *) PWM1T2L_ADDR)
#define     PWM1CR          ((__IO uint8_t xdata *) PWM1CR_ADDR)
#define     PWM1HLD         ((__IO uint8_t xdata *) PWM1HLD_ADDR)

#define     PWM2T1          ((__IO uint16_t xdata *) PWM2T1_ADDR)
#define     PWM2T1H         ((__IO uint8_t xdata *) PWM2T1H_ADDR)
#define     PWM2T1L         ((__IO uint8_t xdata *) PWM2T1L_ADDR)
#define     PWM2T2          ((__IO uint16_t xdata *) PWM2T2_ADDR)
#define     PWM2T2H         ((__IO uint8_t xdata *) PWM2T2H_ADDR)
#define     PWM2T2L         ((__IO uint8_t xdata *) PWM2T2L_ADDR)
#define     PWM2CR          ((__IO uint8_t xdata *) PWM2CR_ADDR)
#define     PWM2HLD         ((__IO uint8_t xdata *) PWM2HLD_ADDR)

#define     PWM3T1          ((__IO uint16_t xdata *) PWM3T1_ADDR)
#define     PWM3T1H         ((__IO uint8_t xdata *) PWM3T1H_ADDR)
#define     PWM3T1L         ((__IO uint8_t xdata *) PWM3T1L_ADDR)
#define     PWM3T2          ((__IO uint16_t xdata *) PWM3T2_ADDR)
#define     PWM3T2H         ((__IO uint8_t xdata *) PWM3T2H_ADDR)
#define     PWM3T2L         ((__IO uint8_t xdata *) PWM3T2L_ADDR)
#define     PWM3CR          ((__IO uint8_t xdata *) PWM3CR_ADDR)
#define     PWM3HLD         ((__IO uint8_t xdata *) PWM3HLD_ADDR)

#define     PWM4T1          ((__IO uint16_t xdata *) PWM4T1_ADDR)
#define     PWM4T1H         ((__IO uint8_t xdata *) PWM4T1H_ADDR)
#define     PWM4T1L         ((__IO uint8_t xdata *) PWM4T1L_ADDR)
#define     PWM4T2          ((__IO uint16_t xdata *) PWM4T2_ADDR)
#define     PWM4T2H         ((__IO uint8_t xdata *) PWM4T2H_ADDR)
#define     PWM4T2L         ((__IO uint8_t xdata *) PWM4T2L_ADDR)
#define     PWM4CR          ((__IO uint8_t xdata *) PWM4CR_ADDR)
#define     PWM4HLD         ((__IO uint8_t xdata *) PWM4HLD_ADDR)

#define     PWM5T1          ((__IO uint16_t xdata *) PWM5T1_ADDR)
#define     PWM5T1H         ((__IO uint8_t xdata *) PWM5T1H_ADDR)
#define     PWM5T1L         ((__IO uint8_t xdata *) PWM5T1L_ADDR)
#define     PWM5T2          ((__IO uint16_t xdata *) PWM5T2_ADDR)
#define     PWM5T2H         ((__IO uint8_t xdata *) PWM5T2H_ADDR)
#define     PWM5T2L         ((__IO uint8_t xdata *) PWM5T2L_ADDR)
#define     PWM5CR          ((__IO uint8_t xdata *) PWM5CR_ADDR)
#define     PWM5HLD         ((__IO uint8_t xdata *) PWM5HLD_ADDR)

#define     PWM6T1          ((__IO uint16_t xdata *) PWM6T1_ADDR)
#define     PWM6T1H         ((__IO uint8_t xdata *) PWM6T1H_ADDR)
#define     PWM6T1L         ((__IO uint8_t xdata *) PWM6T1L_ADDR)
#define     PWM6T2          ((__IO uint16_t xdata *) PWM6T2_ADDR)
#define     PWM6T2H         ((__IO uint8_t xdata *) PWM6T2H_ADDR)
#define     PWM6T2L         ((__IO uint8_t xdata *) PWM6T2L_ADDR)
#define     PWM6CR          ((__IO uint8_t xdata *) PWM6CR_ADDR)
#define     PWM6HLD         ((__IO uint8_t xdata *) PWM6HLD_ADDR)

#define     PWM7T1          ((__IO uint16_t xdata *) PWM7T1_ADDR)
#define     PWM7T1H         ((__IO uint8_t xdata *) PWM7T1H_ADDR)
#define     PWM7T1L         ((__IO uint8_t xdata *) PWM7T1L_ADDR)
#define     PWM7T2          ((__IO uint16_t xdata *) PWM7T2_ADDR)
#define     PWM7T2H         ((__IO uint8_t xdata *) PWM7T2H_ADDR)
#define     PWM7T2L         ((__IO uint8_t xdata *) PWM7T2L_ADDR)
#define     PWM7CR          ((__IO uint8_t xdata *) PWM7CR_ADDR)
#define     PWM7HLD         ((__IO uint8_t xdata *) PWM7HLD_ADDR)

//! SPI peripherals
/* SPI basic address definition */
#define     SPSTAT_ADDR     ((uint8_t)0xCD
#define     SPCTL_ADDR      ((uint8_t)0xCE
#define     SPDAT_ADDR      ((uint8_t)0xCF

/* Bit definition for SPSTAT */
#define     SPIF            ((uint8_t)0x80
#define     WCOL            ((uint8_t)0x40

/* SPCTL offset */
#define     SSIG            ((uint8_t)0x80
#define     SPEN            ((uint8_t)0x40
#define     DORD            ((uint8_t)0x20
#define     MSTR            ((uint8_t)0x10
#define     CPOL            ((uint8_t)0x08
#define     CPHA            ((uint8_t)0x04

/* SPI special function registers */
sfr SPSTAT      =   SPSTAT_ADDR;
sfr SPCTL       =   SPCTL_ADDR;
sfr SPDAT       =   SPDAT_ADDR;


//! I2C peripherals

/* Base address */
#define     I2C_BASE         ((uint8_t)0xFE80)
#define     I2CCFG_ADDR   (I2C_BASE + 0x00U)
#define     I2CMSCR_ADDR  (I2C_BASE + 0x01U)
#define     I2CMSST_ADDR  (I2C_BASE + 0x02U)
#define     I2CSLCR_ADDR  (I2C_BASE + 0x03U)
#define     I2CSLST_ADDR  (I2C_BASE + 0x04U)
#define     I2CSLADR_ADDR (I2C_BASE + 0x05U)
#define     I2CTXD_ADDR   (I2C_BASE + 0x06U)
#define     I2CRXD_ADDR   (I2C_BASE + 0x07U)

/* I2CCFG */
#define     ENI2C           ((uint8_t)0x80)
#define     MSSL            ((uint8_t)0x40)

/* I2CMSCR offset */
#define     EMSI            ((uint8_t)0x80)

/* I2CMSST offset */
#define     MSBUSY          ((uint8_t)0x80)
#define     MSIF            ((uint8_t)0x40)
#define     MSACKI          ((uint8_t)0x02)
#define     MSACKO          ((uint8_t)0x01)

/* I2CSLCR offset */
#define     ESTAI           ((uint8_t)0x40)
#define     ERXI            ((uint8_t)0x20)
#define     ETXI            ((uint8_t)0x10)
#define     ESTOI           ((uint8_t)0x08)
#define     SLRST           ((uint8_t)0x01)

/* I2CSLST offset */
#define     SLBUSY          ((uint8_t)0x80)
#define     STAIF           ((uint8_t)0x40)
#define     RXIF            ((uint8_t)0x20)
#define     TXIF            ((uint8_t)0x10)
#define     STOIF           ((uint8_t)0x08)
#define     TXING           ((uint8_t)0x04)
#define     SLACKI          ((uint8_t)0x02)
#define     SLACKO          ((uint8_t)0x01)

/* I2C special function registers */
#define     I2CCFG          ((__IO uint8_t xdata *) I2CCFG_ADDR)
#define     I2CMSCR         ((__IO uint8_t xdata *) I2CMSCR_ADDR)
#define     I2CMSST         ((__IO uint8_t xdata *) I2CMSST_ADDR)
#define     I2CSLCR         ((__IO uint8_t xdata *) I2CSLCR_ADDR)
#define     I2CSLST         ((__IO uint8_t xdata *) I2CSLST_ADDR)
#define     I2CSLADR        ((__IO uint8_t xdata *) I2CSLADR_ADDR)
#define     I2CTXD          ((__IO uint8_t xdata *) I2CTXD_ADDR)
#define     I2CRXD          ((__IO uint8_t xdata *) I2CRXD_ADDR)


/** @} */

/** @} */

/** @} */

#endif

