/**
 * @file setting.c
 * @author MContour (m-contour@qq.com)
 * @brief Waveform setting options basic function realization
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

#include "setting.h"
#include "global_var.h"


void CheckMinTimeInterval(void) {
    if (G_TriggerMode) {
        if (G_ScaleH > Scale_200us) {
            G_ScaleH = Scale_200us;
        } 
    }
    else {
        if (G_ScaleH > Scale_100us) {
            G_ScaleH = Scale_100us;
        }
    } // check higher bound
}

void Change_SVin_ratio(bit ifNext) {
    if (ifNext) {
        SVin_ratio++;
        if (SVin_ratio > SVin_ratio_MAX) {
            SVin_ratio = SVin_ratio_MAX;
        }
    }
    else {
        SVin_ratio--;

        if (SVin_ratio < SVin_ratio_MIN) {
            SVin_ratio = SVin_ratio_MIN;
        }
    }
}

void Change_ScaleH(bit ifNext) {
    if (ifNext) {
        G_ScaleH++;
        CheckMinTimeInterval();
    }
    else {
        G_ScaleH--;
        if (G_ScaleH < Scale_500ms) {
            G_ScaleH = Scale_500ms;
        } // check lower bound
    }
}

void Change_TriggerMode(bit ifNext) {
    if (ifNext) {
        G_TriggerMode++;
        if (G_TriggerMode > TriggerSel_Single) {
            G_TriggerMode = TriggerSel_Auto;
        }
    }
    else {
        G_TriggerMode--;

        if (G_TriggerMode < TriggerSel_Auto) {
            G_TriggerMode = TriggerSel_Single;
        }
    }
}

void Change_TriggerPosOffset(bit ifNext) {
    if (ifNext) {
        G_TriggerPosOffset--;
        if (G_TriggerPosOffset + G_TriggerPos < ((CHART_H_MAX - CHART_H_MIN) >> 1)) {
            G_TriggerPosOffset = ((CHART_H_MAX - CHART_H_MIN) >> 1) - G_TriggerPos;
        }
    }
    else {
        G_TriggerPosOffset++;
        if (G_TriggerPosOffset + G_TriggerPos > ADC_SAMPLE_BUF_SIZE - ((CHART_H_MAX - CHART_H_MIN) >> 1) - 1) {
            G_TriggerPosOffset = ADC_SAMPLE_BUF_SIZE - ((CHART_H_MAX - CHART_H_MIN) >> 1) - 1 - G_TriggerPos;
        }
    }
}


void Change_OptionInChart(bit ifNext) {
    if (ifNext) {
        G_OptionInChart++;
        if (G_OptionInChart > ChartSel_MeasureWay) {
            G_OptionInChart = ChartSel_ScaleH;
        }
    }
    else {
        G_OptionInChart--;

        if (G_OptionInChart < ChartSel_ScaleH) {
            G_OptionInChart = ChartSel_MeasureWay;
        }
    }
}

void Change_OptionInSetting(bit ifNext) {
    if (ifNext) {
        G_OptionInSettings++;
        if (G_OptionInSettings > SettingSel_OLED_Brightness)
            G_OptionInSettings = SettingSel_PlotMode;
    }
    else {
        G_OptionInSettings--;
        if (G_OptionInSettings < SettingSel_PlotMode)
            G_OptionInSettings = SettingSel_OLED_Brightness;
    }
}

void Change_OLED_Brightness(bit ifNext)
{
    if (ifNext) {
        G_OLED_Brightness++;
        if (G_OLED_Brightness > OLED_BRIGHTNESS_MAX)
            G_OLED_Brightness = OLED_BRIGHTNESS_MAX;
    }
    else {
        G_OLED_Brightness--;
        if (G_OLED_Brightness < OLED_BRIGHTNESS_MIN)
            G_OLED_Brightness = OLED_BRIGHTNESS_MIN;
    }
    G_OLED_BrightnessChanged = SETBIT;
}

void Change_PlotMode(void) {
    G_PlotMode = ~G_PlotMode;
}