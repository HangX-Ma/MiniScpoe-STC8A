/**
 * @file chart.h
 * @author MContour (m-contour@qq.com)
 * @brief Wave diplay function definitions
 * @version 0.1
 * @date 2022-05-15
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

#ifndef __CHART__H__
#define __CHART__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif

#include "adc.h"
#include "ssd1306.h"

//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------
/* Region for waveform on display*/
#define CHART_H_MAX 127
#define CHART_H_MIN 32
#define CHART_V_MAX 53
#define CHART_V_MIN 10
//* ------------     functions     ------------

/**
 * @brief main interface 
 */
void PlotChart(void);

void PlotSettings(void);

void PlotSaveStatus(bit _saveStatus);

void GetWaveData(void);

bit GetTriggerPos(uint16_t d1, uint16_t d2, uint16_t dTrigger, bit triSlope);

int32_t Convert_mv_ADC(int32_t mv, uint16_t *BGV, uint16_t ADCbg, uint16_t lsb);

void AnalyseData();

void PlotWave();

#endif  //!__CHART__H__