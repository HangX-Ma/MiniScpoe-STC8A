/**
 * @file main.c
 * @author MContour (m-contour@qq.com)
 * @brief main program
 * @version 0.1
 * @date 2022-05-06
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

#include "main.h"
#include "STDIO.H"
#include "STDLIB.H"
#include "STRING.H"
#include "stc8x_sysclk.h"
/**
 * @brief Rewrite 'putchar' function, remapped to UARTx output
 * 
 * @param c input character
 * @return char 
 */
char putchar (char c) {
    UARTx_Send(c);

    return c;
}


int main (void) {
    uint16_t ADC_Sampled_REFV;
    uint16_t *ADC_REFV;
    uint16_t Vin;
    uint16_t VSampled;
    // P0M1 |= 0x50;
    // P0M0 &= ~0x50;
    /* Set ADC port P0.4, P0.6 as high-impedance */
    ADC_CHx_SEL(ADC_CONTR_ADC_CHS_VAL12);
    ADC_CHx_SEL(ADC_CONTR_ADC_CHS_VAL14);

    UART1_Init();
    delay_nus(20);

    EA = SETBIT;

    while (1) {
        ADC_REFV = Get_RAM_REFV();
        printf("Ref ADC value: %d\r\n", *ADC_REFV);
        VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL12, VBAT_RATIO);
        printf("VBAT value: %d\r\n", VBAT);
        ADC_GetSampleVal_Enquiry(ADC_CONTR_ADC_CHS_VAL15);
        ADC_Sampled_REFV = G_ADC_data;
        printf("ADC_Sampled_REFV: %d\r\n", ADC_Sampled_REFV);
        ADC_GetSampleVal_Enquiry(ADC_CONTR_ADC_CHS_VAL12);
        VSampled = G_ADC_data;
        printf("VSampled value: %d\r\n", VSampled);
        Vin = (uint32_t)(*ADC_REFV) * VSampled * VBAT_RATIO/100 /ADC_Sampled_REFV;
        printf("Vin value: %d\r\n", Vin);
        delay_nms(1000);
    }
    return 0;
}