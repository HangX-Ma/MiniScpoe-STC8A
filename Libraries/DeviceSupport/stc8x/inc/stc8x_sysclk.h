/**
 * @file stc8x_sysclk.h
 * @author MContour (m-contour@qq.com)
 * @brief STC8x system clock basic function definitions 
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Apache 2.0 LICENSE
 * 
 *****************************************************************************
 * Copyright (c) 2022 MContour m-contour@qq.com
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

#ifndef __STC8X_SYSCLK__H__
#define __STC8X_SYSCLK__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
    sfr VRTRIM = 0xA6;
#endif

/** @addtogroup STC
 * @{
 */

/** @addtogroup Peripheral
 * @{
 */

//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------

//* ------------     functions     ------------


/**
 * @brief power on clock setting
 */
void RCC_DeInit(void);


#if  (LIB_MCU_MODULE == STC8Ax) 
    // * read from RAM
    #define T22M_RAM    (*(__IO int8_t idata *)0xFA)
    #define T24M_RAM    (*(__IO int8_t idata *)0xFB)

    // * read from ROM
    #define T22M_ROM    (*(__IO int8_t code *)0xFDF4)
    #define T24M_ROM    (*(__IO int8_t code *)0xFDF3)

    /**
     * @brief User defines the frequency of the MCU clock
     * 
     * @param[in] _irtrim
     * @param[in] _clkdiv Clock frequency division value.
     * @param[in] _mclk_div_val MCLK divsion value.
     * @param[in] _mclk_output_pin Choose the outpout pin of MCLK, '0' means no output.
     */
    void STC8Ax_RCC_UserIRC(uint8_t _irtrim, uint8_t _clkdiv, uint8_t _mclk_div_val, uint8_t _mclk_output_pin);

#elif  (LIB_MCU_MODULE == STC8Hx) 
    // * read from CHIPID
    #define CPUIDBASE       ((uint16_t)0xFDE0)
    #define ID_CHIPID       (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x00))
    #define VREF_CHIPID     (*(__IO uint16_t xdata *)(CPUIDBASE + 0x07)) // Internal 1.19V reference source byte
    #define F32K_CHIPID     (*(__IO uint16_t xdata *)(CPUIDBASE + 0x09)) // 32K power-down wake-up timer frequency byte
    #define T22M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x0B)) // 22.1184MHz
    #define T24M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x0C)) // 24MHz
    #define T20M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x0D)) // 20MHz
    #define T27M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x0E)) // 27MHz
    #define T30M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x0F)) // 30MHz
    #define T33M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x10)) // 33.1776MHz
    #define T35M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x11)) // 35MHz
    #define T36M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x12)) // 36.864MHz
    #define T40M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x13)) // 40MHz
    #define T45M_CHIPID     (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x14)) // 45MHz
    #define VRT6M_CHIPID    (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x15)) // VRTRIM_6M
    #define VRT10M_CHIPID   (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x16)) // VRTRIM_10M
    #define VRT27M_CHIPID   (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x17)) // VRTRIM_27M
    #define VRT44M_CHIPID   (*(__IO uint8_t  xdata *)(CPUIDBASE + 0x18)) // VRTRIM_44M

    // * read from ROM
    #define ROMBASE     ((uint16_t)0xFDE7)
    #define ID_ROM      (*(__IO uint8_t  code *)(ROMBASE + 0x12))
    #define VREF_ROM    (*(__IO uint16_t code *)(ROMBASE + 0x10)) // Internal 1.19V reference source byte
    #define F32K_ROM    (*(__IO uint16_t code *)(ROMBASE + 0x0E)) // 32K power-down wake-up timer frequency byte
    #define T22M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x0D)) // 22.1184MHz
    #define T24M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x0C)) // 24MHz
    #define T20M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x0B)) // 20MHz
    #define T27M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x0A)) // 27MHz
    #define T30M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x09)) // 30MHz
    #define T33M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x08)) // 33.1776MHz
    #define T35M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x07)) // 35MHz
    #define T36M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x06)) // 36.864MHz
    #define T40M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x05)) // 40MHz
    #define T45M_ROM    (*(__IO uint8_t  code *)(ROMBASE + 0x04)) // 45MHz
    #define VRT6M_ROM   (*(__IO uint8_t  code *)(ROMBASE + 0x03)) // VRTRIM_6M
    #define VRT10M_ROM  (*(__IO uint8_t  code *)(ROMBASE + 0x02)) // VRTRIM_10M
    #define VRT27M_ROM  (*(__IO uint8_t  code *)(ROMBASE + 0x01)) // VRTRIM_27M
    #define VRT44M_ROM  (*(__IO uint8_t  code *)(ROMBASE + 0x00)) // VRTRIM_44M
    
    /**
     * @brief User defines the frequency of the MCU clock
     * 
     * @param[in] _irtrim
     * @param[in] _vrtrim
     * @param[in] _clkdiv Clock frequency division value.
     * @param[in] _mclk_div_val MCLK divsion value.
     * @param[in] _mclk_output_pin Choose the outpout pin of MCLK, '0' means no output.
     */
    void STC8Hx_RCC_UserIRC(uint8_t _irtrim, uint8_t _vrtrim, uint8_t _ircband, uint8_t _clkdiv, uint8_t _mclk_div_val, uint8_t _mclk_output_pin);


#endif

/** @} */

/** @} */


#endif  //!__STC8X_SYSCLK__H__