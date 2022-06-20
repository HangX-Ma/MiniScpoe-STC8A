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
#include "stc8x_eeprom.h"

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
        SVin_ratio = SVin_ratio + 10;
        if (SVin_ratio > SVin_ratio_MAX) {
            SVin_ratio = SVin_ratio_MAX;
        }
    }
    else {
        SVin_ratio = SVin_ratio - 10;

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

void Change_TriggerVal(bit ifNext) {
    if (ifNext) {
        if (G_TriggerLevel_mV_Modified >= 5000) {
            G_TriggerLevel_mV_Modified += 500;
        }
        else {
            G_TriggerLevel_mV_Modified += 100;
        }

        if (G_TriggerLevel_mV_Modified > MAX_VOLTAGE_V) {
            G_TriggerLevel_mV_Modified = MAX_VOLTAGE_V;
        }
    }
    else {
        if (G_TriggerLevel_mV_Modified <= 5000) {
            G_TriggerLevel_mV_Modified -= 100;
        }
        else {
            G_TriggerLevel_mV_Modified -= 500;
        }

        if (G_TriggerLevel_mV_Modified < MIN_VOLTAGE_V) {
            G_TriggerLevel_mV_Modified = MIN_VOLTAGE_V;
        }
    }
    G_TriggerLevel_mV = G_TriggerLevel_mV_Modified + 11100;
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
    G_OLED_BrightnessChanged_FLAG = SETBIT;
}

void Change_PlotMode(void) {
    G_PlotModeSel = ~G_PlotModeSel;
}


void Change_VoltageV_Max(bit ifNext) {
    if (ifNext) {
        if (G_VolV_Max <= 1000) {
            G_VolV_Max          -= 100;
            G_VolV_Max_Modified -= 100;
        }
        else {
            G_VolV_Max          -= 500;
            G_VolV_Max_Modified -= 500;
        }

        if (G_VolV_Max <= G_VolV_Min) {
            G_Scale_Auto_FLAG = SETBIT;
        }
    }
    else {
        G_Scale_Auto_FLAG = CLRBIT;
        if (G_VolV_Max >= 1000) {
            G_VolV_Max          += 500;
            G_VolV_Max_Modified += 500;
        }
        else {
            G_VolV_Max          += 100;
            G_VolV_Max_Modified += 100;
        }

        if (G_VolV_Max > MAX_VOLTAGE_V) {
            G_VolV_Max          = MAX_VOLTAGE_V;
            G_VolV_Max_Modified = MAX_VOLTAGE_V;
        }
    }
}

void Change_VoltageV(bit ifNext)
{
    G_Scale_Auto_FLAG = CLRBIT;
    if (ifNext) {
        G_VolV_Max -= 100;
        G_VolV_Min -= 100;
        G_VolV_Max_Modified -= 100;
        if (G_VolV_Min < MIN_VOLTAGE_V + 100) {
            G_VolV_Max += 100;
            G_VolV_Min = MIN_VOLTAGE_V;
        }
    }
    else {
        G_VolV_Max_Modified += 100;
        G_VolV_Max          += 100;
        G_VolV_Min          += 100;
        if (G_VolV_Max > MAX_VOLTAGE_V) {
            G_VolV_Max          = MAX_VOLTAGE_V;
            G_VolV_Max_Modified = MAX_VOLTAGE_V;
            G_VolV_Min          -= 100;
        }
    }
}

void SelectInSettings(bit ifNext) {
    if (G_SEL_CONFIRM_FLAG) {
        Change_OptionInSetting(ifNext);
    }
    else if (G_OptionInSettings == SettingSel_PlotMode) {
        Change_PlotMode();
    } // Switch plot mode
    else if (G_OptionInSettings == SettingSel_SVin_ratio) {
        Change_SVin_ratio(ifNext);
    } // Adjust sampling port voltage division ratio
    else if (G_OptionInSettings == SettingSel_OLED_Brightness) {
        Change_OLED_Brightness(ifNext);
    } // Adjust OLED Brightness

    /* Options need to be saved */
    G_State_OptionChanged_FLAG = SETBIT;
}

void SelectInChart(bit ifNext) {
    if (G_SEL_CONFIRM_FLAG) {
        if (G_WaveScroll_FLAG) {
            Change_VoltageV(ifNext);
            G_WaveUpdate_FLAG = SETBIT;
        } // waveform scroll mode
        else {
            Change_OptionInChart(ifNext);
            /* Not clear waveform when switch options */
            G_ClearWave_FLAG = CLRBIT;
        } // Out of waveform scroll mode
    }
    else if (G_WaveScroll_FLAG) {
        Change_TriggerPosOffset(ifNext);
        G_WaveUpdate_FLAG = SETBIT; 
    } 
    else {
        if (G_OptionInChart == ChartSel_ScaleH) {
            Change_ScaleH(ifNext);
            G_WaveUpdate_FLAG     = SETBIT;
            G_ClearWave_FLAG      = SETBIT;
            G_WaveAvgLengthSumNum = 0; 
            G_WaveAvgLengthSum    = 0;   
        } // Adjust time scale 
        else if (G_OptionInChart == ChartSel_ScaleV) {
            Change_VoltageV_Max(ifNext);
            G_WaveUpdate_FLAG = SETBIT;
            G_ClearWave_FLAG  = CLRBIT;
        } // Adjust measuring range
        else if (G_OptionInChart == ChartSel_TriggerLevel) {
            Change_TriggerVal(ifNext);
            G_WaveUpdate_FLAG = CLRBIT;
            G_ClearWave_FLAG  = CLRBIT;
        } // Adjust Trigger level
        else if (G_OptionInChart == ChartSel_TriggerSlope) {
            G_TriggerSlope_FLAG = ~G_TriggerSlope_FLAG;
            G_WaveUpdate_FLAG = CLRBIT;
            G_ClearWave_FLAG  = CLRBIT;
        } // Adjust Trigger Slope
        else if (G_OptionInChart == ChartSel_TriggerMode) {
            Change_TriggerMode(ifNext);
            CheckMinTimeInterval(); //检查时间区间的合法性
            G_WaveUpdate_FLAG     = SETBIT;
            G_ClearWave_FLAG      = SETBIT;
            G_WaveAvgLengthSumNum = 0; 
            G_WaveAvgLengthSum    = 0;   
        } // Switch Trigger Mode
        else if (G_OptionInChart == ChartSel_MeasureWay) {
            G_MeasureWaySel = !G_MeasureWaySel;
            G_WaveUpdate_FLAG = CLRBIT;
            G_ClearWave_FLAG  = CLRBIT;
        } // Switch measurement way
        
        /* Options need to be saved */
        G_State_OptionChanged_FLAG = SETBIT;
    } // Rotate Encoder in main interface
}


void Read_Options(void) {
    uint8_t options[16];
    uint8_t *optionsPtr;
    optionsPtr = options;

    EEPROM_Read_N_Bytes(options, sizeof(options) / sizeof(uint8_t), EEPROM_START_ADDR);

    /* SVin_ratio [uint16_t] */
    SVin_ratio    = *optionsPtr++;   // Read sampling port voltage ratio high 8 bits
    SVin_ratio  <<= 8;               // Move SVin_ratio low 8 bits to high bits, preparing for reading 8 high bits
    SVin_ratio   |= *optionsPtr++;   // Read sampling port voltage ratio high 8 bits

    /* Waveform measurement way selection [bit] */
	G_MeasureWaySel = *optionsPtr++;

    /* Plot mode selection [bit] */
    G_PlotModeSel = *optionsPtr++;

    /* Time scale (horizontal) [int8_t]  */
    G_ScaleH = *optionsPtr++;        // Read time scale [char]

    /* Auto time scale flag [bit] */
    G_Scale_Auto_FLAG = *optionsPtr++;   // Auto range flag [bit]

    /* G_VolV_Max [int16_t] */
    G_VolV_Max = *optionsPtr++;     // Read vertical axis maximum voltage high 8 bits
    G_VolV_Max <<= 8;               // Move G_VolV_Max low 8 bits to high bits, preparing for reading 8 high bits
    G_VolV_Max |= *optionsPtr++;    // Read vertical axis maximum voltage high 8 bits
    
    /* G_VolV_Min [int16_t] */
    G_VolV_Min = *optionsPtr++;     // Read vertical axis minimum voltage high 8 bits
    G_VolV_Min <<= 8;               // Move G_VolV_Min low 8 bits to high bits, preparing for reading 8 high bits
    G_VolV_Min |= *optionsPtr++;    // Read vertical axis minimum voltage high 8 bits

    G_TriggerLevel_mV_Modified = *optionsPtr++;  // Read trigger level (mV) [modified] high 8 bits
    G_TriggerLevel_mV_Modified <<= 8;            // Move low 8 bits to high bits, preparing for reading 8 high bits
    G_TriggerLevel_mV_Modified |= *optionsPtr++; // Read trigger level (mV) [modified] high 8 bits

    /* Read trigger mode [int8_t] */
    G_TriggerMode = *optionsPtr++;
    
    /* Read trigger direction flag [bit] */
    G_TriggerSlope_FLAG = *optionsPtr++;

    /* Read wave scrolling flag [bit] */
    G_WaveScroll_FLAG = *optionsPtr++;

    /* Read OLED brightness degree [int8_t] */
    G_OLED_Brightness = *optionsPtr;

    /* Check validation */
    Check_Options();
}

void Check_Options(void) {
    if (SVin_ratio > CHX_Vin_ratio_MAX || SVin_ratio < CHX_Vin_ratio_MIN) {
        SVin_ratio = CHX_VBAT_RATIO;
    }

    if (G_ScaleH > SVin_ratio_MAX || G_ScaleH < SVin_ratio_MIN) {
        G_ScaleH = 0;
    }

    if (G_VolV_Max > MAX_VOLTAGE_V || G_VolV_Max < MIN_VOLTAGE_V) {
        G_VolV_Max = 0;
    }

    if (G_VolV_Min > MAX_VOLTAGE_V || G_VolV_Min < MIN_VOLTAGE_V) {
        G_VolV_Min = 0;
    }

    if (G_VolV_Max <= G_VolV_Min) {
        G_VolV_Max = G_VolV_Min;
        G_Scale_Auto_FLAG = SETBIT;
    }

    if (G_TriggerLevel_mV > MAX_VOLTAGE_V || G_TriggerLevel_mV < MIN_VOLTAGE_V) {
        G_TriggerLevel_mV = 2000;
    }

    if (G_TriggerMode > TriggerSel_Single || G_TriggerMode < TriggerSel_Auto) {
        G_TriggerMode = TriggerSel_Auto;
    }

    if (G_OLED_Brightness > OLED_BRIGHTNESS_MAX || G_OLED_Brightness < OLED_BRIGHTNESS_MIN) {
        G_OLED_Brightness = 10;
    }

    CheckMinTimeInterval();
}

ErrorStatus Save_Options(void) {
    uint8_t option[16];
    uint8_t *optionPtr;

    if (G_LVD_EVENT_FLAG) {
        G_LVD_EVENT_FLAG = CLRBIT;
        return ERROR;
    }

    Check_Options();
    
    optionPtr    = option;
    *optionPtr++ = SVin_ratio >> 8;
    *optionPtr++ = SVin_ratio;
	*optionPtr++ = G_MeasureWaySel;
    *optionPtr++ = G_PlotModeSel;
    *optionPtr++ = G_ScaleH;
    *optionPtr++ = G_Scale_Auto_FLAG;
    *optionPtr++ = G_VolV_Max >> 8;
    *optionPtr++ = G_VolV_Max;
    *optionPtr++ = G_VolV_Min >> 8;
    *optionPtr++ = G_VolV_Min;
    *optionPtr++ = G_TriggerLevel_mV_Modified >> 8;
    *optionPtr++ = G_TriggerLevel_mV_Modified;
    *optionPtr++ = G_TriggerMode;
    *optionPtr++ = G_TriggerSlope_FLAG;
    *optionPtr++ = G_WaveScroll_FLAG;
    *optionPtr   = G_OLED_Brightness;

    return EEPROM_Write_N_Bytes(option, sizeof(option)/sizeof(uint8_t), EEPROM_START_ADDR);
}