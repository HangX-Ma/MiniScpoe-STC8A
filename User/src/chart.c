/**
 * @file chart.c
 * @author MContour (m-contour@qq.com)
 * @brief Wave diplay functions realization
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

#include "global_var.h"
#include "setting.h"
#include "chart.h"
#include "adc.h"
#include "MATH.H"

uint16_t *ADCSamplingPtr;               // Real time ADC sampling pointer
int32_t waveBuf[ADC_SAMPLE_BUF_SIZE];   // Analyzed waveform data buffer
int32_t TriggerPosFEqPoint;             // Trigger position forward equivalent point
int32_t TriggerPosBEqPoint;             // Trigger position backward equivalent point
int32_t plotADCMax, plotADCMin;

void GetWaveData() {
    ADCSamplingPtr = GetWaveADC(ADC_CONTR_ADC_CHS_VAL14, G_ScaleH);
}

/**
 * @brief Get the Vertical Voltage Range Automatically
 * @details Auto range, according to sampled point maximum and minimum value,
 * expanding range by 500 mv per step.
 */
void GetVoltageVRangeAuto(void) {
    if (G_Scale_Auto_FLAG) {
        if (G_VMax / 100 % 10 >= 5) {
            G_VolV_Max = (G_VMax + 500) / 1000 * 1000;
        }
        else {
            G_VolV_Max = G_VMax / 1000 * 1000 + 500;
        }

        if (G_Voltage_Modified / 100 % 10 >= 5) {
            G_VolV_Max_Modified = (G_Voltage_Modified + 500) / 1000 * 1000;
        }
        else {
            G_VolV_Max_Modified = G_Voltage_Modified / 1000 * 1000 + 500;
        }

        if (G_VMin / 100 % 10 >= 5) {
            G_VolV_Min = G_VMin / 1000 * 1000 + 500;
        }
        else {
            G_VolV_Min = G_VMin / 1000 * 1000;
        }

        if (G_VolV_Max > MAX_VOLTAGE_V) {
            G_VolV_Max = MAX_VOLTAGE_V;
        }

        if (G_VolV_Min < MIN_VOLTAGE_V) {
            G_VolV_Min = MIN_VOLTAGE_V;
        }
    }
}

/**
 * @brief Get the Wave Length
 * 
 * @param triggerLevel 
 * @param ifRightSide Search right(1) side or left(0) side
 * @return 
 */
bit GetWaveLength(int32_t triggerLevel, bit ifRightSide)
{
    int16_t i;
    uint8_t triggerForward  = 255;
    uint8_t triggerBackward = 255;
    uint8_t triggerPosTmp;
    bit triggerSlopeTmp;
    bit triggerFail = SETBIT;
    /* Search the trigger point closest to the center of chart */
    if (ifRightSide) {
        for (i = G_TriggerPos + G_TriggerPosOffset; i < ADC_SAMPLE_BUF_SIZE - 2; i++) {
            if (GetTriggerPos(*(ADCbuf + i), *(ADCbuf + i + 1), triggerLevel, (bit)RisingEdgeTrigger)) {
                triggerPosTmp   = i;
                triggerSlopeTmp = SETBIT;
                triggerFail     = CLRBIT;
                break;
            } // Search on rising edge
            else if (GetTriggerPos(*(ADCbuf + i), *(ADCbuf + i + 1), triggerLevel, (bit)FallingEdgeTrigger)) {
                triggerPosTmp   = i;
                triggerSlopeTmp = CLRBIT;
                triggerFail     = CLRBIT;
                break;
            } // Search on falling edge
        } 
    } // Search right side
    else {
        for (i = G_TriggerPos + G_TriggerPosOffset; i > 0; i--) {
            if (GetTriggerPos(*(ADCbuf + i), *(ADCbuf + i + 1), triggerLevel, RisingEdgeTrigger)) {
                triggerPosTmp   = i;
                triggerSlopeTmp = SETBIT;
                triggerFail     = CLRBIT;
                break;
            } // Search on rising edge
            else if (GetTriggerPos(*(ADCbuf + i), *(ADCbuf + i + 1), triggerLevel, FallingEdgeTrigger)) {
                triggerPosTmp   = i;
                triggerSlopeTmp = CLRBIT;
                triggerFail     = CLRBIT;
                break;
            } // Search on falling edge
        }
    } //Search left side

    if (!triggerFail)
    {
        for (i = triggerPosTmp; i >= 0; i--) {
            if (triggerSlopeTmp) {
                if (*(ADCbuf + i) >= triggerLevel && *(ADCbuf + i + 1) <= triggerLevel) {
                    triggerForward = i;
                    break;
                }
            } // Trigger on rising edge, searching on falling edge
            else {
                if (*(ADCbuf + i) <= triggerLevel && *(ADCbuf + i + 1) >= triggerLevel) {
                    triggerForward = i;
                    break;
                }
            } // Trigger on falling edge, searching on rising edge
        } // The equivalence point to the left of the trigger position of the query

        for (i = triggerPosTmp; i < ADC_SAMPLE_BUF_SIZE - 1; i++)
        {
            if (triggerSlopeTmp) {
                if (*(ADCbuf + i) >= triggerLevel && *(ADCbuf + i + 1) <= triggerLevel) {
                    triggerBackward = i;
                    break;
                }
            } // Trigger on rising edge, searching on falling edge
            else {
                if (*(ADCbuf + i) <= triggerLevel && *(ADCbuf + i + 1) >= triggerLevel) {
                    triggerBackward = i;
                    break;
                }
            } // Trigger on falling edge, searching on rising edge
        } // The equivalence point to the right of the trigger position of the query
    }

    if (triggerForward == 255 || triggerBackward == 255 || (triggerForward == triggerBackward)) {
        G_WaveLength = 0;
        return 0;
    }
    else {
        G_WaveLength = triggerBackward - triggerForward;
        return 1;
    }
}

/**
 * @brief Get the Waveform Frequency
 */
void GetWaveFreq(void) {
    uint16_t sumNum; // Sum number
    uint8_t  avgRSNum;   //Average right shift number

    if (!G_ADC_Running_FLAG) {
        G_WaveAvgLengthSumNum = 0;
        G_WaveAvgLengthSum    = 0;
    } // When the ADC stops sampling, the waveform frequency is displayed for each action, so the average value is not calculated
    else if (G_TriggerMode == 0 && G_ScaleH > Scale_100ms) {
        switch (G_ScaleH) {
            case Scale_50ms: sumNum = 2; avgRSNum = 1; break;
            case Scale_20ms: sumNum = 4; avgRSNum = 2; break;
            default: sumNum = 8; avgRSNum = 3; break; //<10ms
        }

        G_WaveAvgLengthSum += G_WaveLength;

        if (++G_WaveAvgLengthSumNum >= sumNum) {
            G_WaveLength          = G_WaveAvgLengthSum >> avgRSNum; // Get average waveform length
            G_WaveAvgLengthSumNum = 0;                              // Clear waveform length summation times
            G_WaveAvgLengthSum    = 0;                              // Clear waveform length summation 
        }  // Reach the threshold of summation number
        else {
            return;
        } // If not reach the threshold of summation number, return and keep WaveFreq stable.
    } // In automatic mode, in order to keep the frequency from changing frequently, 
      // take the average value, and don't take the average value for the interval 
      // greater than or equal to 100 ms, one reason is that the 100 ms waveform 
      // changes slowly, so the frequency beats infrequently, on the other hand, 
      // the longer the time interval is, the higher the delay of average value is

    switch (G_ScaleH) {
        case Scale_500ms: G_WaveFreq = 50.0f / (float)G_WaveLength + 0.5; break; // G_WaveFreq=25000/(500*G_WaveLength);
        case Scale_200ms: G_WaveFreq = (125.0f / (float)G_WaveLength);    break; // G_WaveFreq=25000/(200*G_WaveLength);
        case Scale_100ms: G_WaveFreq = (250.0f / (float)G_WaveLength);    break; // G_WaveFreq=25000/(100*G_WaveLength);
        case Scale_50ms:  G_WaveFreq = (500.0f / (float)G_WaveLength);    break; // G_WaveFreq=25000/(50*G_WaveLength);
        case Scale_20ms:  G_WaveFreq = (1250.0f / (float)G_WaveLength);   break; // G_WaveFreq=25000/(20*G_WaveLength);
        case Scale_10ms:  G_WaveFreq = (2500.0f / (float)G_WaveLength);   break; // G_WaveFreq=25000/(10*G_WaveLength);
        case Scale_5ms:   G_WaveFreq = (5000.0f / (float)G_WaveLength);   break; // G_WaveFreq=25000/(5*G_WaveLength);
        case Scale_2ms:   G_WaveFreq = (12500.0f / (float)G_WaveLength);  break; // G_WaveFreq=25000/(2*G_WaveLength);
        case Scale_1ms:   G_WaveFreq = (25000.0f / (float)G_WaveLength);  break; // G_WaveFreq=25000/(1*G_WaveLength);
        case Scale_500us: G_WaveFreq = (50000.0f / (float)G_WaveLength);  break; // G_WaveFreq=25000000/(500*G_WaveLength);
        case Scale_200us: G_WaveFreq = (125000.0f / (float)G_WaveLength); break; // G_WaveFreq=25000000/(200*G_WaveLength);
        case Scale_100us: G_WaveFreq = (250000.0f / (float)G_WaveLength); break; // G_WaveFreq=25000000/(100*G_WaveLength);
    }

    G_WaveFreq = (float)G_WaveFreq /  (0.0162f * (log10(G_WaveFreq) /log10(2.7) ) + 0.9398f);

}

/**
 * @brief Convert voltage in mV to string
 * 
 * @param mv mV
 * @return string pointer 
 */
uint8_t* Convert_mV_Str(int32_t mv)
{
    static uint8_t str[6];

     if (mv > -10000l && mv < 0 ) {
        str[0] = '-';
        str[1] = mv / -1000l + '0';
        str[2] = '.';
        str[3] = mv / -100l % -10l + '0';
        str[4] = mv / -10l % -10l + '0';
        str[5] = '\0';
    }
    else if( mv <= -10000l) {
        str[0] = '-';
        str[1] = mv / -10000l + '0';
        str[2] = mv / -1000l % -10l + '0';
        str[3] = '.';
        str[4] = mv / -100l % -10l + '0';
        str[5] = '\0';
    }
    else if ( mv >= 0 && mv < 10000l) {
        str[0] = '+';
        str[1] = mv / 1000l + '0';
        str[2] = '.';
        str[3] = mv / 100l % 10 + '0';
        str[4] = mv / 10l % 10 + '0';
        str[5] = '\0';
    }
    else {
        str[0] = '+';
        str[1] = mv / 10000l + '0';
        str[2] = mv / 1000l % 10l + '0';
        str[3] = '.';
        str[4] = mv / 100l % 10l + '0';
        str[5] = '\0';
    }

    return str;
}

/**
 * @brief Convert frequency to string
 * 
 * @return string
 */
uint8_t* Convert_WaveFreq_Str(void) {
    static uint8_t str[5];
    if (G_WaveFreq == 0) {
        str[0] = '-';
        str[1] = '-';
        str[2] = '-';
        str[3] = '-';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 10000000) {
        str[0] = ' ';
        str[1] = G_WaveFreq / 10000000 + '0';
        str[2] = ((G_WaveFreq) / 1000000) % 10 + '0';
        str[3] = 'M';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 1000000) {
        str[0] = G_WaveFreq / 1000000 + '0';
        str[1] = '.';
        str[2] = ((G_WaveFreq) / 100000) % 10 + '0';
        str[3] = 'M';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 100000) {
        str[0] = G_WaveFreq / 100000 + '0';
        str[1] = (G_WaveFreq / 10000) % 10 + '0';
        str[2] = ((G_WaveFreq) / 1000) % 10 + '0';
        str[3] = 'k';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 10000) {
        str[0] = ' ';
        str[1] = G_WaveFreq / 10000 + '0';
        str[2] = ((G_WaveFreq) / 1000) % 10 + '0';
        str[3] = 'k';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 1000) {
        str[0] = G_WaveFreq / 1000 + '0';
        str[1] = '.';
        str[2] = ((G_WaveFreq) / 100) % 10 + '0';
        str[3] = 'k';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 100) {
        str[0] = ' ';
        str[1] = G_WaveFreq / 100 + '0';
        str[2] = (G_WaveFreq / 10) % 10 + '0';
        str[3] = (G_WaveFreq) % 10 + '0';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 10) {
        str[0] = ' ';
        str[1] = ' ';
        str[2] = G_WaveFreq / 10 + '0';
        str[3] = (G_WaveFreq) % 10 + '0';
        str[4] = '\0';
    }
    else if (G_WaveFreq >= 1) {
        str[0] = ' ';
        str[1] = ' ';
        str[2] = ' ';
        str[3] = (G_WaveFreq) % 10 + '0';
        str[4] = '\0';
    }

    return str;
}

/**
 * @brief Remap sampling data to display range and inverse it 
 * 
 * @param val 
 * @param rangeMax 
 * @param rangeMin 
 * @param rangeMaxNew 
 * @param rangeMinNew 
 * @return uint16 
 */
uint16_t remap(uint16_t val, uint16_t rangeMax, uint16_t rangeMin, uint16_t rangeMaxNew, uint16_t rangeMinNew)
{
    if (val > rangeMax) {
        val = rangeMax;
    }
    else if (val < rangeMin) {
        val = rangeMin;
    }
    val = rangeMinNew + (uint32_t)(rangeMax - val) * (rangeMaxNew - rangeMinNew) / (rangeMax - rangeMin);

    return val;
}

int32_t Max = 0;
int32_t Min = 4095;

void AnalyzeData()
{
    int16_t i;
    int32_t tmp;
    int32_t adcMax     = 0;
    int32_t adcMin     = 4095;
    int32_t adcMid     = 0;
    int32_t plotADCMid = 0;
    static int32_t Avg_Modified_Voltage    = 0;
    static int32_t Modified_Voltage_Buffer = 0;
    static int8_t  Avg_Cnt = 0;
    // uint16_t ADC_Sampled_Bandgap, ADC_RAM_Bandgap;
    /* get internal 1.344V REFV average value by ADC */

    /* read internal 1.344V REFV */

    if (G_ADC_Complete_FLAG) {
        G_ScaleH_tmp = G_ScaleH; 
        //record the sampling time interval, because the number of sampling points is small, 
        // it is not supported to scale the waveform according to the time interval in real time, 
        // and to clear the waveform when the time interval changes. Copy the sample point to
        // another array to avoid data clutter caused by sample interruption. 
        // If the sampling is interrupted, the waveform is displayed using the old 
        //sampling point in the cache.        
        for (i = 0; i < ADC_SAMPLE_BUF_SIZE; i++) {
            *(ADCbuf + i) = *(ADCSamplingPtr + i);
        }

        /* Calculate the trigger position */
        /* When ADC sampling stops, `G_TriggerPos` remains the same, so the following calculations are not performed */
        G_TriggerPos       = ADC_SAMPLE_BUF_SIZE / 2;
        G_TriggerFail_FLAG = SETBIT;
        for (i = ((CHART_H_MAX - CHART_H_MIN) >> 1); i < ADC_SAMPLE_BUF_SIZE - ((CHART_H_MAX - CHART_H_MIN) >> 1); i++)
        {
            if (GetTriggerPos(*(ADCbuf + i), *(ADCbuf + i + 1), G_TriggerADCx, G_TriggerSlope_FLAG)) {
                G_TriggerPos       = i;
                G_TriggerFail_FLAG = CLRBIT;
                break;
            }
        }
        G_TriggerPosOffset = 0;
    }

    /* Get the MAX and MIN value of waveform on screen */
    for (i = 0; i <= 100; i++) {
        tmp = *(ADCbuf + G_TriggerPos + G_TriggerPosOffset - 50 + i);
        if (tmp > adcMax) {
            adcMax = tmp;
        } 
        else if (tmp < adcMin) {
            adcMin = tmp;
        } 
    }

    Max = adcMax;
    Min = adcMin;

    // The maximum and minimum sampling values of the sampling points are converted into voltage values mV
    G_VMax = ConvertUnit_ADC2mV(adcMax, BGV, ADCbg, SVin_ratio);
    G_VMin = ConvertUnit_ADC2mV(adcMin, BGV, ADCbg, SVin_ratio);

    G_VMax_Modified = ConvertUnit_ADC2mV(adcMax - G_Bias_Voltage, BGV, ADCbg, SVin_ratio);
    G_VMin_Modified = ConvertUnit_ADC2mV(adcMin - G_Bias_Voltage, BGV, ADCbg, SVin_ratio);

    if(G_VMax_Modified > 0  && G_VMin_Modified > 0)
    {
        G_VMax_Modified = G_VMax_Modified * (0.0003f * (double)G_VMax_Modified + 0.2085f);
        G_VMin_Modified = G_VMin_Modified * (0.0003f * (double)G_VMin_Modified + 0.2085f);
    }
    else if(G_VMax_Modified < 0  && G_VMin_Modified < 0)
    {
        G_VMax_Modified = G_VMax_Modified * (0.0002f * (0-(double)G_VMax_Modified) + 0.2065f);
        G_VMin_Modified = G_VMin_Modified * (0.0002f * (0-(double)G_VMin_Modified) + 0.2065f);		
    }
    else if(G_VMax_Modified > 0  && G_VMin_Modified < 0)
    {
        G_VMax_Modified = G_VMax_Modified * (0.0003f * (double)G_VMax_Modified + 0.2085f);
        G_VMin_Modified = G_VMin_Modified * (0.0002f * (0-(double)G_VMin_Modified) + 0.2065f);
    }

    if(G_MeasureWaySel == MeasureWay_DC)
    {
        Avg_Modified_Voltage += (G_VMax_Modified + G_VMin_Modified) >> 1;
        
        if(++Avg_Cnt >= 8) {
            Avg_Cnt = 0;
            
            G_Voltage_Modified = (float)Avg_Modified_Voltage / 8.0f;
            Avg_Modified_Voltage = 0;
        }		
        
    }
    else if(G_MeasureWaySel == MeasureWay_AC) {
        Modified_Voltage_Buffer = (G_VMax_Modified - G_VMin_Modified);
        
        Modified_Voltage_Buffer = Modified_Voltage_Buffer / (0.3722f*(log10(Modified_Voltage_Buffer)/log10(2.728f)) - 2.0842f );
        
        Avg_Modified_Voltage   += Modified_Voltage_Buffer;
        
        if(++Avg_Cnt >= 8) {
            Avg_Cnt = 0;
            
            G_Voltage_Modified   = (float)Avg_Modified_Voltage / 8.0f;
            Avg_Modified_Voltage = 0;
        }
    }

    // Get the upper and lower limits of the vertical voltage range automatically
    GetVoltageVRangeAuto();

    // Use the vertical ruler mV range to invert the ADC range as the upper and lower limits of the chart
    plotADCMax = ConvertUnit_mV2ADC(G_VolV_Max, BGV, ADCbg, SVin_ratio);
    plotADCMin = ConvertUnit_mV2ADC(G_VolV_Min, BGV, ADCbg, SVin_ratio);

    // Calculate waveform frequency
    //如果当前的时间区间和采样数据的时间间隔一致则进行频率计算
    //由于STC8A8K在高频采样时，ADC值经常莫名其妙地跳变到满量程4095，咨询STC工程师无果
    //为避免ADC采样出错时频率跳变厉害，计算波长时使用电压标尺的中点和波幅中点的较小值
    //如果遇到频率跳变无法读取，将自动量程切换至合适的手动量程即可
    adcMid = (adcMax + adcMin) >> 1;
    plotADCMid = (plotADCMax + plotADCMin) >> 1;
    if (GetWaveLength(adcMid < plotADCMid ? adcMid : plotADCMid, 1) || GetWaveLength(adcMid < plotADCMid ? adcMid : plotADCMid, 0)) {
        GetWaveFreq();
    }
    else {
        G_WaveFreq = 0;
    }

    // Remap the sample values to the display area of the screen
    for (i = 0; i < ADC_SAMPLE_BUF_SIZE; i++) {
        waveBuf[i] = remap(*(ADCbuf + i), plotADCMax, plotADCMin, CHART_V_MAX, CHART_V_MIN);
    }
}


void PlotChart(void)
{
    uint8_t i;
    uint8_t *str;

    if (G_ClearDisplay_FLAG) {
        G_ClearDisplay_FLAG = CLRBIT;
        OLED_ClearBuffer();

        /* Chart border
           Waveform horizontal plotting area: 101 grids, 26~126
           Waveform vertical plotting area: 45 grids, ,8~52
           Frame of chart 45x101*/
        OLED_ShowHorizontalLine(CHART_H_MIN, CHART_V_MIN, 4);
        OLED_ShowHorizontalLine(CHART_H_MIN, CHART_V_MAX, 4);
        OLED_ShowHorizontalLine(CHART_H_MAX - 3, CHART_V_MIN, 4);
        OLED_ShowHorizontalLine(CHART_H_MAX - 3, CHART_V_MAX, 4);
        OLED_ShowHorizontalLine(CHART_H_MIN + 25 - 2, CHART_V_MIN, 5);
        OLED_ShowHorizontalLine(CHART_H_MIN + 25 - 2, CHART_V_MAX, 5);
        OLED_ShowHorizontalLine(CHART_H_MIN + 50 - 2, CHART_V_MIN, 5);
        OLED_ShowHorizontalLine(CHART_H_MIN + 50 - 2, CHART_V_MAX, 5);
        OLED_ShowHorizontalLine(CHART_H_MIN + 75 - 2, CHART_V_MIN, 5);
        OLED_ShowHorizontalLine(CHART_H_MIN + 75 - 2, CHART_V_MAX, 5);
        OLED_ShowVerticalLine(CHART_H_MIN - 1, CHART_V_MIN, CHART_V_MAX - CHART_V_MIN + 1);
        OLED_ShowVerticalLine(CHART_H_MAX + 1, CHART_V_MIN, CHART_V_MAX - CHART_V_MIN + 1);

        /* Grid of chart */
        for (i = 0; i < 15; i++) {
            OLED_ShowHorizontalLine(CHART_H_MIN + 7 * i, CHART_V_MIN + ((CHART_V_MAX - CHART_V_MIN) >> 1), 3);
        }
        for (i = 0; i < 6; i++) {
            OLED_ShowVerticalLine(CHART_H_MIN + 25, CHART_V_MIN + 1 + i * 8, 3);
            OLED_ShowVerticalLine(CHART_H_MIN + 50, CHART_V_MIN + 1 + i * 8, 3);
            OLED_ShowVerticalLine(CHART_H_MIN + 75, CHART_V_MIN + 1 + i * 8, 3);
        }

        /* Voltage range of waveform*/
        OLED_SetPos(18, 56);
        str = Convert_mV_Str(G_VMin_Modified);
        OLED_ShowString(str);

        /* Ruler for waveform position*/
        OLED_ShowHorizontalLine(50, 66, 17);
        OLED_ShowVerticalLine(50, 60, 3);
        OLED_ShowVerticalLine(66, 60, 3);
        OLED_ShowVerticalLine((G_TriggerPos + G_TriggerPosOffset - 80) * 0.5f / 4 + 55, 58, 5);

        OLED_SetPos(67, 56);
        str = Convert_mV_Str(G_VMax_Modified);
        OLED_ShowString(str);
    }

    OLED_Overlap(0); // Set drawing mode to overwrite

    /* Flag for Auto Range*/
    if (G_Scale_Auto_FLAG == SETBIT) {
        OLED_SetPos(0, 0);
        OLED_ShowString("Auto");
    }
    else {
        OLED_SetPos(0, 0);
        OLED_ShowString("    ");
    }

    /* Trigger Level */
    if (G_OptionInChart == ChartSel_TriggerLevel && !G_WaveScroll_FLAG)
    {
        OLED_ShowHorizontalLine(28, 0, 44);
        OLED_ShowVerticalLine(34, 1, 8);
        OLED_ShowVerticalLine(71, 1, 8);
        OLED_Reverse(1);
    }
    else {
        OLED_Reverse(1);
        OLED_ShowHorizontalLine(28, 0, 44);
        OLED_ShowVerticalLine(34, 1, 8);
        OLED_ShowVerticalLine(71, 1, 8);
        OLED_Reverse(0);
    }

    OLED_SetPos(28, 1);
    OLED_ShowString("T");
    str = Convert_mV_Str(G_TriggerLevel_mV_Modified);
    OLED_SetPos(35, 1);
    OLED_ShowString(str);
    OLED_SetPos(65, 1);
    OLED_ShowString("V");

    OLED_Reverse(0);


    /* Trigger Slope */
    if (G_OptionInChart == ChartSel_TriggerSlope && !G_WaveScroll_FLAG) {
        OLED_ShowVerticalLine(73, 1, 8);
        OLED_ShowVerticalLine(80, 1, 8);
        OLED_Reverse(1);
    }
    else {
        OLED_Reverse(1);
        OLED_ShowVerticalLine(73, 1, 8);
        OLED_ShowVerticalLine(80, 1, 8);
        OLED_Reverse(0);
    }
	
    if (G_TriggerSlope_FLAG) {
        OLED_ShowChar(75, 1, 123); //123 up arrow，rising edge trigger
    }
    else {
        OLED_ShowChar(75, 1, 124); //124 down arrow，falling edge trigger
    }
    OLED_Reverse(0);

    /* Trigger Mode */
    if (G_OptionInChart == ChartSel_TriggerMode && !G_WaveScroll_FLAG) {
        OLED_ShowVerticalLine(88, 1, 8);
        OLED_Reverse(1);
    }
    else {
        OLED_Reverse(1);
        OLED_ShowVerticalLine(88, 1, 8);
        OLED_Reverse(0);
    }
	
    OLED_SetPos(82, 1);
    if (G_TriggerMode == TriggerSel_Auto) {
        OLED_ShowString("A");
    }
    else if (G_TriggerMode == TriggerSel_Normal) {
        OLED_ShowString("N");
    }
    else if (G_TriggerMode == TriggerSel_Single) {
        OLED_ShowString("S");
    }
    OLED_Reverse(0);

    /* Frequency */
    OLED_SetPos(92, 1);
    OLED_ShowString("          ");
    OLED_SetPos(92, 1);
    str = Convert_WaveFreq_Str();
    OLED_ShowString(str);
    OLED_SetPos(117, 1);
    OLED_ShowString("Hz");

    // Draw run/stop flags
    OLED_SetPos(0, 8);
    if (G_ADC_Running_FLAG) {
        OLED_ShowString("Run ");
    }
    else {
        OLED_ShowString("Stop");
    }

    /* Trigger Fail Flag*/
    OLED_SetPos(0, 17);
    if (G_TriggerFail_FLAG) {
        OLED_ShowString("Fail");
    }
    else {
        OLED_ShowString("Succ");
    }

    /* Ruler for Voltage */
    OLED_SetPos(0, 26);
    OLED_ShowString("    ");
    if (G_OptionInChart == ChartSel_ScaleV && !G_WaveScroll_FLAG) {
        OLED_ShowHorizontalLine(0, 25, 30);  // Because the top line doesn't cover it, so just draw it out and fill it in.
        OLED_Reverse(1);
    }
    else {
        OLED_Reverse(1);
        OLED_ShowHorizontalLine(0, 25, 30);  // Because the top line doesn't cover it, so just draw it out and fill it in.
        OLED_Reverse(0);
    }

    str = Convert_mV_Str(G_VolV_Max_Modified);
    OLED_SetPos(0, 26);
    OLED_ShowString(str);
    OLED_Reverse(0);

     /* Correct the calculated waveform voltage value */   
    if (G_OptionInChart == ChartSel_MeasureWay && !G_WaveScroll_FLAG) {
        OLED_ShowVerticalLine(25, 35, 9);    // Since the rightmost column is not covered, draw it directly to complete it
        OLED_ShowVerticalLine(24, 35, 9);    // Since the rightmost column is not covered, draw it directly to complete it
        OLED_ShowHorizontalLine(0, 35, 25);  // Because the top line doesn't cover it, so just draw it out and fill it in.
        OLED_Reverse(1);
    }
    else
    {
        OLED_Reverse(1);
        OLED_ShowVerticalLine(25, 35, 9);    // Since the rightmost column is not covered, draw it directly to complete it
        OLED_ShowVerticalLine(24, 35, 9);    // Since the rightmost column is not covered, draw it directly to complete it
        OLED_ShowHorizontalLine(0, 35, 25);  //
        OLED_Reverse(0);
    }
	
    OLED_SetPos(1, 36);
    if(G_MeasureWaySel == MeasureWay_DC) {
        OLED_ShowString("M-DC");
    }
    else if (G_MeasureWaySel == MeasureWay_AC) {
        OLED_ShowString("M-AC");
    } 
    OLED_Reverse(0);
    
    /* Correct the calculated waveform voltage value */   
	str = Convert_mV_Str(G_Voltage_Modified);
    OLED_SetPos(0, 46);
    OLED_ShowString(str);
    OLED_Reverse(0);
	

    /* Seconds per division */
    OLED_SetPos(99, 56);
    OLED_ShowString("     ");
    if (G_OptionInChart == ChartSel_ScaleH && !G_WaveScroll_FLAG) {
        OLED_ShowHorizontalLine(99, 55, 30);  // Because the top line doesn't cover it, so just draw it out and fill it in.
        OLED_Reverse(1);
    }
    else {
        OLED_Reverse(1);
        OLED_ShowHorizontalLine(99, 55, 30);  // Because the top line doesn't cover it, so just draw it out and fill it in.
        OLED_Reverse(0);		
    }
    OLED_SetPos(99, 56);
    OLED_ShowString(ScaleHTxt[G_ScaleH]);
    OLED_Reverse(0);

    /* display the quantity of electricity */
    OLED_ShowHorizontalLine(1, 56, 14);
    OLED_ShowHorizontalLine(14, 57, 2);

    /* left shell */
    OLED_ShowVerticalLine(1, 56, 7);
    /* First frame */
    OLED_ShowVerticalLine(3, 56, 7);
    /* Second frame */
    OLED_ShowVerticalLine(5, 56, 7);
    /* Third frame */
    OLED_ShowVerticalLine(7, 56, 7);

    if(VBAT >=3500 && VBAT < 4000) {
        /* Forth frame */
        OLED_ShowVerticalLine(9, 56, 7);
    }

    if(VBAT >=4000 && VBAT < 4500) {
        /* Forth frame */
        OLED_ShowVerticalLine(9, 56, 7);
        /* Fifth frame */
        OLED_ShowVerticalLine(11, 56, 7);
    }
    if(VBAT >=4500) {
        /* Forth frame */
        OLED_ShowVerticalLine(9, 56, 7);
        /* Fifth frame */
        OLED_ShowVerticalLine(11, 56, 7);
        /* head frame */
        OLED_ShowVerticalLine(13, 56, 7);
    }
    // right shell  
    OLED_ShowVerticalLine(15, 57, 5);

    OLED_ShowHorizontalLine(14, 61, 2);
    OLED_ShowHorizontalLine(1, 62, 14);

    //resume drawing mode is overlay
    OLED_Overlap(1);
}

void PlotWave(void)
{
    uint8_t i;
    if (G_PlotModeSel == PlotMode_Vector)
    {
        for (i = 0; i < (CHART_H_MAX - CHART_H_MIN); i++) {
            OLED_ShowLine(
                i + CHART_H_MIN,
                waveBuf[G_TriggerPos + G_TriggerPosOffset - ((CHART_H_MAX - CHART_H_MIN) >> 1) + i],
                i + CHART_H_MIN + 1,
                waveBuf[G_TriggerPos + G_TriggerPosOffset - ((CHART_H_MAX - CHART_H_MIN) >> 1) + i + 1]
                );
        }
    } //Vector Mode

    if (G_PlotModeSel == PlotMode_Dot)
    {
        for (i = 0; i <= (CHART_H_MAX - CHART_H_MIN); i++) {
            OLED_ShowPixel(i + CHART_H_MIN, waveBuf[G_TriggerPos + G_TriggerPosOffset - ((CHART_H_MAX - CHART_H_MIN) >> 1) + i]);
        }
    } // Dots Mode
}

void PlotSettings()
{
    if (G_ClearDisplay_FLAG) {
        G_ClearDisplay_FLAG = CLRBIT;
        OLED_ClearBuffer();
        
        OLED_ShowVerticalLine(48, 0, 11);    // left frame
        OLED_ShowVerticalLine(88, 0, 11);    // right frame
        OLED_ShowHorizontalLine(49, 0, 39);  // up frame
        OLED_ShowHorizontalLine(49, 10, 39); // down frame
        
        OLED_SetPos(50, 2);
        OLED_ShowString("MinDSO");
    }

    OLED_Overlap(0);

    /* Options */
    /* DrawMode */
    OLED_SetPos(38, 15);
    OLED_ShowString("PMode=");
    OLED_ShowString(PlotModeTxt[G_PlotModeSel]);

    /* Sampling port voltage ratio */
    OLED_SetPos(45, 25);
    OLED_ShowString("LSB=");
    OLED_ShowString("   ");
    OLED_SetPos(70, 25);
    OLED_ShowNum(SVin_ratio, 3);

    /* OLED_Brightness */
    OLED_SetPos(45, 35);
    OLED_ShowString("BRT=");
    OLED_ShowString("   ");
    OLED_SetPos(70, 35);
    OLED_ShowNum(G_OLED_Brightness, 3);

    /* VBAT */
    OLED_SetPos(35, 45);
    OLED_ShowString("Vbat=");
    OLED_ShowString(Convert_mV_Str(VBAT));
    OLED_ShowString("V");

    OLED_Overlap(0);
    /* Option indicator, 125 is Left arrow */
    if (G_OptionInSettings == SettingSel_PlotMode) {
        OLED_ShowChar(101, 15, 125);
        OLED_ShowChar(95, 30, ' ');
        OLED_ShowChar(90, 45, ' ');
    } // PlotMode
    else if (G_OptionInSettings == SettingSel_SVin_ratio) {
        OLED_ShowChar(101, 15, ' ');
        OLED_ShowChar(95, 25, 125);
        OLED_ShowChar(90, 35, ' ');
    } // Sampling port voltage ratio
    else if (G_OptionInSettings == SettingSel_OLED_Brightness) {
        OLED_ShowChar(101, 15, ' ');
        OLED_ShowChar(95, 25, ' ');
        OLED_ShowChar(90, 35, 125);
    } // OLED_Brightness 
	
    OLED_Overlap(1);
}

void PlotSaveStatus(bit _saveStatus)
{
	OLED_Overlap(0); // Set drawing mode to overwrite
    OLED_SetPos(30, 55);
    if (_saveStatus) {
        OLED_ShowString("Save Success!");
    }
    else {
        OLED_ShowString("Save Fail!");
    }
    OLED_Overlap(1); // Set drawing mode to overlap
}
