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
/* voltage division ratio = [for instance] (2K+8K)/(2K)*100 = 500 
   Algorithm limitation: STC8A8K maximum integer is 2^32, 
   SVin_ratio maximum value is (2^32/2^12/1344 = 780)
   [12 bits ADC:2^12][BGV_RAM:1344mV]  */
#define SVin_ratio_MAX          750
#define SVin_ratio_MIN          10

#define CHX_Vin_ratio_MAX       750
#define CHX_Vin_ratio_MIN       10

#define OLED_BRIGHTNESS_MAX     25      //!< Maximum OLED brightness degree
#define OLED_BRIGHTNESS_MIN     1       //!< Minimum OLED brightness degree

#define MAX_VOLTAGE_V           30000   //!< Vertical axis maximum voltage (mV)
#define MIN_VOLTAGE_V           -11000  //!< Vertical axis minimum voltage (mV)
//* ------------     functions     ------------

/**
 * @brief interval validation checker
 * @details MCU speed limitation result in the Single Sweep 
 * and Normal Sweep calculations could not complete in 100us time interval.
 */
void CheckMinTimeInterval(void);

/**
 * @brief Change interval for wave time scale (Horizontal Grid)
 * @details Time scale: 500ms, 200ms, 100ms, 50ms, 20ms, 10ms, 5ms, 2ms, 1ms, 500us, 200us, 100us 
 * @param ifNext Next scale or last scale
 */
void Change_ScaleH(bit ifNext);

/**
 * @brief Change wave sampling port voltage division ratio
 * @details ratio value has maximum and minimum limitation
 * @param ifNext Next or last ratio value
 */
void Change_SVin_ratio(bit ifNext);

/**
 * @brief Change trigger value
 * 
 * @param ifNext Increase or decrease
 */
void Change_TriggerVal(bit ifNext);

/**
 * @brief Select suitable trigger mode (loop)
 * @details 2: Single(stop sampling after triggering, manually waiting for next triggering), 
 * 1: Normal(stop sampling after triggering, automatically waiting for next triggering), 
 * 0: Auto(continuously sample, manually stop)
 * @param ifNext Next or last trigger mode
 */
void Change_TriggerMode(bit ifNext);

/**
 * @brief Change trigger position offset
 * 
 * @param ifNext Increate or decrease position offset
 */
void Change_TriggerPosOffset(bit ifNext);

/**
 * @brief Change chart options
 * @details 0:ScaleH, 1:ScaleV, 2:TriggerLevel, 3:TriggerSlope, 4:TriggerMode 5 MeasureWay
 * @param ifNext Next or last option
 */
void Change_OptionInChart(bit ifNext);

/**
 * @brief Change setting options
 * @details 0:PlotMode, 1:SVin_ratio, 2:OLED_Brightness
 * @param ifNext Next or last option
 */
void Change_OptionInSetting(bit ifNext);

/**
 * @brief Change OLED brightness
 * @details 1~254 but divided artificially to ten degrees,
 * numerically increasing or decreasing 10 per step.
 * @param ifNext Next or last brightness value
 */
void Change_OLED_Brightness(bit ifNext);

/**
 * @brief Change Plot Mode
 * @details 0: vector, 1: dot
 */
void Change_PlotMode(void);

void Change_VoltageV_Max(bit ifNext);

void Change_VoltageV(bit ifNext);

void SelectInSettings(bit ifNext);

void SelectInChart(bit ifNext);

/**
 * @brief Read options from EEPROM
 */
void Read_Options(void);

/**
 * @brief Check options read from EEPROM
 */
void Check_Options(void);


/**
 * @brief Save options to EEPROM and return the saving result
 * @return ErrorStatus, indicate whether saving process success
 */
ErrorStatus Save_Options(void);


#endif  //!__SETTING__H__