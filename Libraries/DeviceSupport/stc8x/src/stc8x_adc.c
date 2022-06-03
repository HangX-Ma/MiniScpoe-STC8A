/**
 * @file stc8x_adc.c
 * @author MContour (m-contour@qq.com)
 * @brief STC8x ADC basic function realization 
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

#include "stc8x_adc.h"
#include "stc8x_delay.h"
#include "INTRINS.H"

__IO uint16_t G_ADC_data;  //!< ADC data container

void ADC_CHx_SEL(uint8_t _ADC_CHx) {
    uint8_t var = (_ADC_CHx&0x08) >> 3;
    if (var == 0) {
        P0M1 |= (1 << (_ADC_CHx & 0x07));
        P0M0 = 0x00;
    }
    else if (var == 1) {
        P1M1 |= (1 << (_ADC_CHx & 0x07));
        P1M0 = 0x00;
    }
}

void ADC_Init(uint8_t _ADC_align, uint8_t _ADC_speed) {
    /* enable external special registers */
    P_SW2 |= P_SW2_EAXFR;
    #if (LIB_MCU_MODULE == STC8Hx) 
        ADCTIM = ADCTIM_CSHOLD_2tk | ADCTIM_SMPDUT_32tk;
    #endif
    /* disable external special registers */
    P_SW2 &= ~P_SW2_EAXFR;
    /* clear ADCCFG register */
    ADCCFG     = 0x00;
    /* set align type and ADC conversion frequency */
    ADCCFG     = _ADC_align | _ADC_speed;
    /* power on ADC */
    ADC_CONTR |= ADC_CONTR_ADC_POWER;
    /* wait for stable power source */
    delay_nms(5);
}


void ADC_GetSampleVal_Enquiry(uint8_t _ADC_CHx) {
    /* clear previous result */
    ADC_RESH = 0x00;
    ADC_RESL = 0x00;
    /* clear previous ADC_CHS value */
    ADC_CONTR &= 0xF0;
    /* select GPIO for ADC function */
    // ADC_CHx_SEL(_ADC_CHx);
    ADC_CONTR |= _ADC_CHx;
    /* start conversion */
    ADC_CONTR |= ADC_CONTR_ADC_START;
    _nop_();
    _nop_();

    while (!(ADC_CONTR & ADC_CONTR_ADC_FLAG)) {
        ;
    } // wait for conversion complete

    /* clear ADC flag for next conversion */
    ADC_CONTR &= ~ADC_CONTR_ADC_FLAG;

    G_ADC_data = (ADC_RESH) << 8 | ADC_RESL;
}


void ADC_GetSampleVal_Interrupt(uint8_t _ADC_CHx, ISR_PRx _ADC_NVIC_Priority) {
    /* clear previous result */
    ADC_RESH = 0x00;
    ADC_RESL = 0x00;
    /* select GPIO for ADC function */
    // ADC_CHx_SEL(_ADC_CHx);
    /* enable ADC interrupt */
    EADC = SETBIT;
    /* enable interrupt system */
    EA   = SETBIT;
    /* set interrupt priority */
    IP  = (IPH & 0xDF) | (((uint8_t)_ADC_NVIC_Priority & 0x01) << 5);
    IPH = (IPH & 0xDF) | (((uint8_t)_ADC_NVIC_Priority & 0x02) << 4);
    /* ensure interrupt flag cleared */
    ADC_CONTR &= ~ADC_CONTR_ADC_FLAG;
    /* start conversion */
    ADC_CONTR |= ADC_CONTR_ADC_START;
    ADC_CONTR |= _ADC_CHx;
}


void ADC_ISR_Handler(void) interrupt(ADC_VECTOR) {
    /* disable interrupt system to protect value */
    EA      = SETBIT;
    G_ADC_data = ((uint16_t)(ADC_RESH) << 8) | (uint16_t)(ADC_RESL);
    /* ensure interrupt flag cleared */
    ADC_CONTR &= ~ADC_CONTR_ADC_FLAG;
    /* enable interrupt system */
    EA      = CLRBIT;
    /* start conversion */
    ADC_CONTR |= ADC_CONTR_ADC_START;
}