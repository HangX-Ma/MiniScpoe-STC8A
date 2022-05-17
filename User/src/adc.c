/**
 * @file adc.c
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
#include "adc.h"



uint16_t GetADC_CHx(uint8_t chx) {
    uint16_t ADC_res = 0;
    uint8_t i;

    ADC_CHx_SEL(chx);
    ADC_Init(ADCCFG_ADC_RESFMT, ADCCFG_SPEED_512tk);

    // read twice but abandon
    ADC_GetSampleVal_Enquiry(chx);
    ADC_GetSampleVal_Enquiry(chx);

    for (i = 0; i < 16; i++) {
        ADC_GetSampleVal_Enquiry(chx);
        ADC_res += G_ADC_data; 
    }
    ADC_res >>= 4;

    return ADC_res;
}

uint16_t Get_RAM_REFV(void) {
    uint16_t BGV_res;

    BGV_res = (uint16_t)(BGV_HByte) << 4 + (uint16_t)(BGV_LByte);

    return BGV_res;
}

uint16_t Get_BATV(uint8_t chx, uint8_t ratio) {
    uint16_t ADC_Sampled_Bandgap, ADC_RAM_Bandgap, ADC_CHx_Val;
    uint16_t Vin;

    /* get internal 1.344V REFV average value by ADC */
    ADC_GetSampleVal_Enquiry(ADC_CONTR_ADC_CHS_VAL15);
    ADC_Sampled_Bandgap = G_ADC_data;
    /* read internal 1.344V REFV */
    ADC_RAM_Bandgap = Get_RAM_REFV();
    /* get ADC channel x average value */
    ADC_GetSampleVal_Enquiry(chx);
    ADC_CHx_Val = G_ADC_data;
    
    Vin = (uint16_t) (ADC_RAM_Bandgap/ADC_Sampled_Bandgap) * ADC_CHx_Val /ratio *100;

    return Vin;
}

uint16_t *GetWaveADC(uint8_t chx, uint8_t scale_h) {
    uint8_t i, j;
    static uint16_t ADCSampling[SAMPLE_NUM];
    /* single trigger buffer, effective sampling point is PRE_BUF_NUM, the additional position copy the last position value, creating a loop */
    uint16_t ADCPreSampling[PRE_BUF_NUM + 1];

}
