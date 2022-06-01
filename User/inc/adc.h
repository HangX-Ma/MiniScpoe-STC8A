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
#include "global_var.h"

//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------
#define BGV_ADDR        ((uint16_t idata *)0xEF)    //!< internal 1.344V(1.19V D64 version) reference voltage high byte

sbit ADC_Sample_Ready_LED = P1^7;

//* ------------     functions     ------------

void Wave_ADC_Init(ScaleSel_TypeDef scale_h);

/**
 * @brief Get ADC channel x conversion result
 * 
 * @param chx ADC channel x
 * @return 16 bits ADC conversion result 
 */
uint16_t GetADC_CHx(uint8_t chx);

/**
 * @brief Get REFV
 * 
 * @return Reference voltage value stroed in RAM 
 */
uint16_t *Get_RAM_REFV(void);

/**
 * @brief Get battery voltage or CHx channel ADC input voltage
 * 
 * @param chx ADC channel x
 * @param ratio voltage division ratio, ratio = ratio_origin x 100
 * @return V 
 */
uint16_t Get_BATV(uint8_t chx, uint8_t ratio);

int32_t ConvertUnit_mV2ADC(int32_t _mV, uint16_t *_ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint8_t ratio);

int32_t ConvertUnit_ADC2mV(int32_t _ADCx, uint16_t *_ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint8_t ratio);

/**
 * @brief Get the Wave ADC
 * 
 * @note The screen is divided to 4 girds, in each grid deploying 25 sampling points
 * @param chx ADC channel x 
 * @param scale_h scale ratio
 * @return ADC sampling result 
 */
uint16_t *GetWaveADC(uint8_t chx, uint8_t scale_h);

/**
 * @brief Get the Trigger position
 * 
 * @param d1 
 * @param d2 
 * @param dTrigger 
 * @param _triggerSlope 
 * @return bit 
 */
bit GetTriggerPos(uint16_t d1, uint16_t d2, uint16_t dTrigger, bit _triggerSlope);

#endif  //!__ADC__H__