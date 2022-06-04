/**
 * @file global_var.c
 * @author MContour (m-contour@qq.com)
 * @brief Global variables
 * @version 0.1
 * @date 2022-05-17
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
#include "global_var.h"

uint16_t VBAT;
uint16_t SVin_ratio;

uint16_t *BGV;
uint16_t ADCbg;

uint8_t  G_WaveLength;
uint32_t G_WaveFreq;
uint16_t G_WaveAvgLengthSumNum;
uint32_t G_WaveAvgLengthSum;

bit      G_ADC_Complete_FLAG;
bit      G_ADC_Interrupt_FLAG;
bit      G_ADC_Running_FLAG;

bit      G_SEL_CONFIRM_FLAG;

bit      G_PlotModeSel;
bit      G_MeasureWaySel;

bit      G_Scale_Auto_FLAG;
bit      G_WaveScroll_FLAG;
bit      G_WaveUpdate_FLAG;
bit      G_DisplayUpdate_FLAG;
bit      G_ClearDisplay_FLAG;
bit      G_ClearWave_FLAG;

int8_t   G_OLED_Brightness = 10;
bit      G_OLED_BrightnessChanged_FLAG;


uint16_t G_TriggerLevel_mV;
uint16_t G_TriggerLevel_mV_Modified;
uint16_t G_TriggerADCx;
int8_t   G_TriggerPosOffset;
int8_t   G_TriggerPos;
int8_t   G_TriggerMode;
bit      G_TriggerSlope_FLAG;
bit      G_TriggerFail_FLAG;

int8_t   G_ScaleH = Scale_100ms;
int8_t   G_ScaleH_tmp;

int32_t  G_VolV_Min;
int32_t  G_VolV_Max;
int32_t  G_VolV_Min_Modified;
int32_t  G_VolV_Max_Modified;
int32_t  G_VMax;
int32_t  G_VMin;
int32_t  G_VMax_Modified;
int32_t  G_VMin_Modified;
int32_t  G_Voltage_Modified;
int32_t  G_Bias_Voltage = 0;

int8_t   G_OptionInSettings;
int8_t   G_OptionInChart;

bit      G_State_Chart_FLAG;
bit      G_State_Settings_FLAG;
bit      G_State_OptionChanged_FLAG;

bit      G_UpdateVBAT_FLAG;

bit     G_SELOption_FLAG;
bit     G_SELOption_Next;

uint16_t ADCbuf[ADC_SAMPLE_BUF_SIZE];
const uint8_t code *PlotModeTxt[] = {"Vect", "Dots"};
const uint8_t code *TriggerModeTxt[] = {"Auto  ", "Normal", "Single"};
const uint8_t code *ScaleHTxt[] = {
    "500ms", //0
    "200ms", //1
    "100ms", //2
    " 50ms", //3
    " 20ms", //4
    " 10ms", //5
    "  5ms", //6
    "  2ms", //7
    "  1ms", //8
    "500us", //9
    "200us", //10
    "100us"  //11
};

