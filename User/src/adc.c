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
#include "STRING.H"
#include "global_var.h"

void Wave_ADC_Init(ScaleSel_TypeDef scale_h) {
    uint8_t ADC_SPEED;

    switch (scale_h) {
        case 0:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //500ms
        case 1:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //200ms
        case 2:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //100ms
        case 3:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //50ms
        case 4:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //20ms
        case 5:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //10ms
        case 6:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //5ms
        case 7:  ADC_SPEED = ADCCFG_SPEED_512tk; break; //2ms
        case 8:  ADC_SPEED = ADCCFG_SPEED_352tk; break; //1ms
        case 9:  ADC_SPEED = ADCCFG_SPEED_192tk; break; //500us
        case 10: ADC_SPEED = ADCCFG_SPEED_32tk;  break; //200us
        case 11: ADC_SPEED = ADCCFG_SPEED_32tk;  break; //100us 
    }
    ADC_Init(ADCCFG_ADC_RESFMT, ADC_SPEED);
}

uint16_t GetADC_CHx(uint8_t chx) {
    uint16_t ADC_res = 0;
    uint8_t i;

    ADC_CHx_SEL(chx);
    Wave_ADC_Init(Scale_500ms);
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

    /* ADC_RAM_Bandgap/(ADC_Sampled_Bandgap/2^12) = Vin/(ADC_CHx_Val/2^12) */

    /* get internal 1.344V REFV average value by ADC */
    ADC_GetSampleVal_Enquiry(ADC_CONTR_ADC_CHS_VAL15);
    ADC_Sampled_Bandgap = G_ADC_data;
    /* read internal 1.344V REFV */
    ADC_RAM_Bandgap = Get_RAM_REFV();
    /* get ADC channel x average value */
    ADC_GetSampleVal_Enquiry(chx);
    ADC_CHx_Val = G_ADC_data;
    
    Vin = (uint16_t) (ADC_RAM_Bandgap/ADC_Sampled_Bandgap) * ADC_CHx_Val * (ratio/100);

    return Vin;
}

int32_t ConvertUnit_mV2ADC(int32_t _mV, uint16_t _ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint8_t ratio) {
    int32_t ADCx;
    ADCx = (int32_t)_mV * (_ADC_Sampled_Bandgap/_ADC_RAM_Bandgap) / (ratio/100);

    return ADCx;
}

int32_t ConvertUnit_ADC2mV(int32_t _ADCx, uint16_t _ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint8_t ratio) {
    int32_t mv;
    mv = (int32_t)_ADCx / (_ADC_RAM_Bandgap/_ADC_Sampled_Bandgap) * (ratio/100);

    return mv;
}


uint16_t* GetWaveADC(uint8_t chx, uint8_t scale_h) {
    uint8_t i, j;
    uint16_t ADC_Sampled_Bandgap, ADC_RAM_Bandgap;
    static uint16_t ADCSampling[ADC_SAMPLE_BUF_SIZE];
    /* single trigger buffer, effective sampling point is PRE_BUF_NUM, the additional position copy the last position value, creating a loop */
    uint16_t ADCSingleTriggerSampling[ADC_SINGLE_TRIGGER_BUF_SIZE + 1];

    if (G_ADC_Interrupt) {
        return ADCSampling;
    }

    memset(ADCSampling, 0x00, sizeof(ADCSampling));
    memset(ADCSingleTriggerSampling, 0x00, sizeof(ADCSingleTriggerSampling));

    ADC_RAM_Bandgap     = Get_RAM_REFV();
    ADC_Sampled_Bandgap = GetADC_CHx(ADC_CONTR_ADC_CHS_VAL15);
    G_TriggerADCx       = ConvertUnit_ADC2mV(G_TriggerLevel_mV, ADC_RAM_Bandgap, ADC_Sampled_Bandgap, SVin_ratio);

    


}
