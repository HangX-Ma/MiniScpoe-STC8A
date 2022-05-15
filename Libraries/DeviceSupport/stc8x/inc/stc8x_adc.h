/**
 * @file stc8x_adc.h
 * @author MContour (m-contour@qq.com)
 * @brief STC8x ADC basic function definitions 
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

#ifndef __STC8X_ADC__H__
#define __STC8X_ADC__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif

/** @addtogroup STC
 * @{
 */

/** @addtogroup Peripheral
 * @{
 */


//* ------------   GLOBAL variables   ------------
extern __IO uint16_t ADC_data;
extern void delay_nms(uint16_t nms);

//* ------------ developer definitions ------------

//* ------------     functions     ------------

/**
 * @brief Initialize ADC registers and select ADC conversion speed, alignment type. 
 * Power on ADC and wait for ADC power source stable.
 * 
 * @param _ADC_align alignment types: left or right
 * @param _ADC_speed ADC conversion speed, less than 800KHz for 12 bits ADC
 */
void ADC_Init(uint8_t _ADC_align, uint8_t _ADC_speed);


/**
 * @brief ADC sampling by enquiring method
 * 
 * @param _ADC_CHx select ADC channel for ADC conversion: 0~15
 */
void ADC_GetSampleVal_Enquiry(uint8_t _ADC_CHx);

/**
 * @brief ADC sampling by interrupt method
 * 
 * @param _ADC_CHx select ADC channel for ADC conversion: 0~15
 * @param _ADC_NVIC_Priority ADC interrupt priority
 */
void ADC_GetSampleVal_Interrupt(uint8_t _ADC_CHx, ISR_PRx _ADC_NVIC_Priority);

/**
 * @brief GPIO - ADC channel selection \c MACRO
 * @param _ADC_CHx ADC channel value from 0~15; Channel \c 15 is internal 1.19V reference voltage
 */
void ADC_CHx_SEL(uint8_t _ADC_CHx);

/** @} */

/** @} */


#endif  //!__STC8X_ADC__H__