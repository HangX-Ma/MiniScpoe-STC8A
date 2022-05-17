/**
 * @file adc.h
 * @author MContour (m-contour@qq.com)
 * @brief ADC
 * @version 0.1
 * @date 2022-05-16
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
#ifndef __ADC__H__
#define __ADC__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif

#include "stc8x_adc.h"
#include "stc8x_delay.h"
#include "INTRINS.H"

//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------
#define BGV_HByte       (*(uint8_t idata *)0xEF)   //!< internal 1.344V reference voltage high byte
#define BGV_LByte       (*(uint8_t idata *)0xF0)   //!< internal 1.344V reference voltage low byte

#define Vol_DIVIDED_RATIO   20  //!< 'voltage divided ratio' * 100, (2K)/(2K+8K)*100=0.2*100=20


//* ------------     functions     ------------

uint16_t GetADC_CHx(uint8_t chx);

uint16_t Get_RAM_REFV(void);

uint16_t Get_BATV(uint8_t chx, uint8_t ratio);

uint16_t *GetWaveADC(uint8_t chx, uint8_t scale_h);


#endif  //!__ADC__H__