/**
 * @file setting.h
 * @author MContour (m-contour@qq.com)
 * @brief Waveform setting options basic function definitions
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

#ifndef __SETTING__H__
#define __SETTING__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif


//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------
#define SVin_ratio_MAX          750 
#define SVin_ratio_MIN          10

#define CHX_Vin_ratio_MAX       750
#define CHX_Vin_ratio_MIN       10

#define OLED_BRIGHTNESS_MAX     25
#define OLED_BRIGHTNESS_MIN     1

//* ------------     functions     ------------



/**
 * @brief interval validation checker
 * @details MCU speed limitation result in the Single Sweep 
 * and Normal Sweep calculations could not complete in 100us time interval.
 */
void CheckMinTimeInterval(void);

void Change_ScaleH(bit ifNext);

void Change_SVin_ratio(bit ifNext);

void Change_TriggerMode(bit ifNext);

void Change_TriggerPosOffset(bit ifNext);

void Change_OptionInChart(bit ifNext);

void Change_OptionInSetting(bit ifNext);

void Change_OLED_Brightness(bit ifNext);

void Change_PlotMode(void);

#endif  //!__SETTING__H__