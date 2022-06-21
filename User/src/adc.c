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
#include "stc8x_delay.h"
#include "INTRINS.H"
#include "STRING.H"

void Wave_ADC_Init(uint8_t scale_h) {
    uint8_t ADC_SPEED;

    switch ((uint8_t)scale_h) {
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
    uint16_t ADC_res;
    uint8_t i;

    ADC_res = 0;
    // ADC_CHx_SEL(chx);
    Wave_ADC_Init(Scale_500ms);

    // read twice but abandon
    ADC_GetSampleVal_Enquiry(chx);
    ADC_GetSampleVal_Enquiry(chx);

    /* ADC interrupt method is less efficient than enquiry method */
    // ADC_GetSampleVal_Interrupt(chx, ISR_PR0, ADCEXCFG_CVTIMESEL_16_CONVERSION_AVG);
    // while (!ADC_INTERRUPT_FLAG) {
    //     ;
    // } // Wait for data conversion complete.
    // ADC_res            = G_ADC_data;
    // ADC_INTERRUPT_FLAG = CLRBIT;
    // ADC_BMM_config(chx, BMM_ADC_CFG2_CVTIMESEL_CONT16);
    
    // while (ADC_BMM_FLAG) {
    //     ;
    // } // Wait for data conversion complete
    // ADC_BMM_FLAG = CLRBIT;
    // ADC_res = (uint16_t)(ADC_BMM_Buffer[chx][ADC_Data_size-2] << 8 + ADC_BMM_Buffer[chx][ADC_Data_size-1]);

    /* ADC enquiry method */
    for (i = 0; i < 16; i++) {
        ADC_GetSampleVal_Enquiry(chx);
        ADC_res += G_ADC_data;
    }
    ADC_res >>= 4; // Get average value.

    return ADC_res;
}

uint16_t *Get_RAM_REFV(void) {
    uint16_t *BGV_resPtr;

    BGV_resPtr = (uint16_t idata *)BGV_ADDR;

    return BGV_resPtr;
}

uint16_t Get_BATV(uint8_t chx, uint16_t ratio) {
    uint16_t ADC_Sampled_Bandgap, ADC_CHx_Val;
    uint16_t Vin;

    /* ADC_RAM_Bandgap/(ADC_Sampled_Bandgap/2^12) = Vin/(ADC_CHx_Val/2^12) */
    Wave_ADC_Init(Scale_500ms);

    /* get internal 1.19V REFV average value by ADC */
    ADC_Sampled_Bandgap = GetADC_CHx(ADC_CONTR_ADC_CHS_VAL15);
    /* get ADC channel x average value */
    ADC_CHx_Val         = GetADC_CHx(chx);
    
    Vin = (uint32_t)(*BGV_RAM) * ADC_CHx_Val * ratio /100 /ADC_Sampled_Bandgap;

    return Vin;
}

int32_t ConvertUnit_mV2ADC(int32_t _mV, uint16_t *_ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint16_t ratio) {
    int32_t ADCx;
    ADCx = (int32_t)_mV * _ADC_Sampled_Bandgap *100 / ratio / (*_ADC_RAM_Bandgap) ;

    return ADCx;
}

int32_t ConvertUnit_ADC2mV(int32_t _ADCx, uint16_t *_ADC_RAM_Bandgap, uint16_t _ADC_Sampled_Bandgap,  uint16_t ratio) {
    int32_t mv;
    mv = (int32_t)_ADCx *(*_ADC_RAM_Bandgap) * ratio/100 / _ADC_Sampled_Bandgap;

    return mv;
}


void SwitchDelay(uint8_t scale_h) { 
    switch (scale_h)
    {
        case Scale_500ms: delay_nus(19971); break;
        case Scale_200ms: delay_nus(7971);  break;
        case Scale_100ms: delay_nus(3971);  break;
        case Scale_50ms:  delay_nus(1971);  break;
        case Scale_20ms:  delay_nus(771);   break;
        case Scale_10ms:  delay_nus(371);   break;
        case Scale_5ms:   delay_nus(171);   break;
        case Scale_2ms:   delay_nus(51);    break;
        case Scale_1ms:   delay_nus(18);    break;
        case Scale_500us: delay_nus(6);     break;
        case Scale_200us: delay_nus(1);     break;
        case Scale_100us: delay_nus(0); _nop_(); break;
    }
}

uint16_t* GetWaveADC(uint8_t chx, uint8_t scale_h) {
    uint8_t i, j;
    // uint16_t ADC_Sampled_Bandgap, ADC_RAM_Bandgap;
    static uint16_t ADCSampling[ADC_SAMPLE_BUF_SIZE];
    /* single trigger buffer, effective sampling point is PRE_BUF_NUM, the additional position copy the last position value, creating a loop */
    uint16_t ADC_STri_PreSampling[ADC_STRI_BUF_SIZE + 1];

    /* Clear ADC sampling complete flag */
    G_ADC_Complete_FLAG = CLRBIT;

    if (G_ADC_Interrupt_FLAG) {
        return ADCSampling;
    }

    /* Clear Cache */
    memset(ADCSampling, 0x00, ADC_SAMPLE_BUF_SIZE*2);
    memset(ADC_STri_PreSampling, 0x00, (ADC_STRI_BUF_SIZE+ 1) * 2);

    // ADC_RAM_Bandgap     = Get_RAM_REFV(); // Read Bandgap voltage value stored in RAM
    ADCbg           = GetADC_CHx(ADC_CONTR_ADC_CHS_VAL15); // Get sampled Bandgap voltage value from ADC
    /* Convert trigger voltage set by user to ADC value */
    G_TriggerADCx   = ConvertUnit_mV2ADC(G_TriggerLevel_mV, BGV_RAM, ADCbg, SVin_ratio);

    /* ADC initialization needs power stabilization process */
    Wave_ADC_Init(scale_h);

    /* Read initial two ADC sampled values but discarded */
    ADC_GetSampleVal_Enquiry(chx);
    ADC_GetSampleVal_Enquiry(chx);
    
    /* 100us time scale mode cannot support single trigger function, 4us limitation */
    if (scale_h == Scale_100us) {
        /* ADC sampling indicator */
        ADC_Sample_Ready_LED = SETBIT;
        for (i = 0; i < ADC_SAMPLE_BUF_SIZE; i++) {
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }
            ADCSampling[i] = ADC_GetSampleVal_Enquiry(chx);
        }
        ADC_Sample_Ready_LED = CLRBIT;
    }
    else if (G_TriggerMode) {
        ADC_Sample_Ready_LED = CLRBIT;  // Preloaded cache, close indicator LED. No input trigger signal until preloaded cache is processed.
        for (j = 1; j <= ADC_STRI_BUF_SIZE; j++) {  // Preload cache with ADC_STRI_BUF_SIZE size. First position is reserved to store the last position value.
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }

            delay_nus(3);           // The time interval of modified cyclic sampling is 3 US slower than that of continuous sampling
            SwitchDelay(scale_h);   // Sampling process delay
            ADCSampling[i] = ADC_GetSampleVal_Enquiry(chx);
        }
        ADC_Sample_Ready_LED = SETBIT;  // Preloading cache process has finished, close indicator.

        // Loop cache ADC_STRI_BUF_SIZE points before triggering, and check whether the sampling points meet the trigger conditions
        while (1) {
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }

            if (j > ADC_STRI_BUF_SIZE) {
                j = 1;
                ADC_STri_PreSampling[0] = ADC_STri_PreSampling[ADC_STRI_BUF_SIZE];
            } // cache overflow, copy the last value to the first place

            SwitchDelay(scale_h); // Sample delay

            ADC_STri_PreSampling[i] = ADC_GetSampleVal_Enquiry(chx);
            if (GetTriggerPos(ADC_STri_PreSampling[j - 1], ADC_STri_PreSampling[j], G_TriggerADCx, G_TriggerSlope_FLAG)) {
                ADC_Sample_Ready_LED = CLRBIT;  // Trigger success, jump out the loop.
                break;
            }
            j++;
        }

        // Continuously sample the remained (ADC_SAMPLE_BUF_SIZE - ADC_STRI_BUF_SIZE) points.
        for (i = 0; i < ADC_SAMPLE_BUF_SIZE - ADC_STRI_BUF_SIZE; i++) {
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }

            delay_nus(3);
            SwitchDelay(scale_h);
            ADCSampling[i + ADC_STRI_BUF_SIZE] = ADC_GetSampleVal_Enquiry(chx);
        }

        // Combine the sampled points to complete waveform
        for (i = 0; i < ADC_STRI_BUF_SIZE; i++) { // Preloaded cache first value is equal to last value, so we abandon the first value.
                                                  // Order the remaining ADC_STRI_BUF_SIZE sampling points in sequence.
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }

            if (++j > ADC_STRI_BUF_SIZE) {
                j = 1;
            } // Preloaded cache overflow, jump to the preloaded cache second position
            ADCSampling[i] = ADC_STri_PreSampling[j];
        }
    } // Single or Normal Trigger Mode
    else {
        ADC_Sample_Ready_LED = SETBIT;  // Start sampling
        for (i = 0; i < ADC_SAMPLE_BUF_SIZE; i++) {
            if (G_ADC_Interrupt_FLAG) {
                return ADCSampling;
            }
            
            ADCSampling[i] = ADC_GetSampleVal_Enquiry(chx);
            delay_nus(3);
            SwitchDelay(scale_h);
        }
        ADC_Sample_Ready_LED = CLRBIT;  // Sample complete
    } // Auto Trigger Mode
    
    G_ADC_Complete_FLAG = SETBIT; // Set complete flag

    return ADCSampling;
}

bit GetTriggerPos(uint16_t d1, uint16_t d2, uint16_t dTrigger, bit _triggerSlope) {
    if (_triggerSlope) {
        if (d1 <= dTrigger && d2 >= dTrigger) {
            return 1;
        }
    } // Rising edge trigger
    else {
        if (d1 >= dTrigger && d2 <= dTrigger) {
            return 1;
        }
    } // Falling edge trigger

    return 0;
}
