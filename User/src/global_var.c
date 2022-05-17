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

bit      G_ADC_Complete;
bit      G_ADC_Interrupt;

bit      G_EC11PressWithRotate;

bit      G_PlotMode;

int8_t   G_OLED_Brightness;
bit      G_OLED_BrightnessChanged;


uint16_t G_TriggerLevel_mV;
uint16_t G_TriggerLevel_mV_Modified;
uint16_t G_TriggerADCx;
int8_t   G_TriggerPosOffset;
int8_t   G_TriggerPos;
int8_t   G_TriggerMode;

int8_t   G_ScaleH = Scale_100ms;
int8_t   G_OptionInSettings;
int8_t   G_OptionInChart;

