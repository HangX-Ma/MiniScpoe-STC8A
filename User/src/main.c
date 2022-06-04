/**
 * @file main.c
 * @author MContour (m-contour@qq.com)
 * @brief main program
 * @version 0.1
 * @date 2022-05-06
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

#include "main.h"
#include "STDIO.H"
#include "STDLIB.H"
#include "STRING.H"
#include "stc8x_sysclk.h"

/**
 * @brief Rewrite 'putchar' function, remapped to UARTx output
 * 
 * @param c input character
 * @return char 
 */
char putchar (char c) {
    UARTx_Send(c);

    return c;
}

int main (void) {
    /* Set OLED port P2.3, P2.4, P2.5, P2.6, P2.7 as quasi bidirectional */
    P2M1 &= ~0xF8;
    P2M0 &= ~0xF8;
    /* Set ADC port P0.4, P0.6 as high-impedance */
    P0M1 |= 0x50;
    P0M0 &= ~0x50;
    /* Set indicator port P1.7 as push-pull output */
    P0M1 &= ~0x80;
    P0M0 |= 0x80;
    /* Set P2.0, P2.1, P2.2, P5.4 as high-impedance */
    P2M1 |= 0x07;
    P2M0 &= ~0x07;
    P5M1 |= 0x10;
    P5M0 &= ~0x10;
    /* Set Encoder port P3.2, P3.3, P3.4 as quasi bidirectional */
    P3M1 &= ~0x1C;
    P3M0 &= ~0x1C;

    P_SW2 |= P_SW2_EAXFR;
    P3PU  |= 0x1C;
    P_SW2 &= ~P_SW2_EAXFR;

    EC11_A   = CLRBIT;
    EC11_B   = CLRBIT;
    EC11_KEY = CLRBIT;

    TM0_Init();
    UART1_Init();
    TM1_Init();
    Read_Options();

    GlobalVar_Init();
    delay_nus(50);

    OLED_Init();
    OLED_SetFontWidth(6);
    PlotChart();
    OLED_Display();

    /* Enable global interrupt */
    EA = SETBIT;

    while (1) {
        /* Update voltage infor of battery */
        if (G_UpdateVBAT_FLAG) {
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL12, VBAT_RATIO);
            G_UpdateVBAT_FLAG = CLRBIT;
        }

        GetWaveData(); // Sample waveform

        if (G_SELOption_FLAG) {
            SelectInChart(G_SELOption_Next);
            G_SELOption_FLAG = CLRBIT;
        }

        if (G_State_Settings_FLAG) {
            runWhenInSettings();
        } // Enter Settings Interface
        else if (G_ADC_Interrupt_FLAG) {
            runWhenADCInterrupt();
        } // ADC Sampling Interrupt
        else {
            runWhenADCComplete();
        } // ADC Sampling Complete
    }

    return 0;
}

void GlobalVar_Init(void) {
    G_State_Settings_FLAG       = CLRBIT;                   // Not enter setting interface
    G_OptionInSettings          = SettingSel_PlotMode;      // PlotMode
    G_OptionInChart             = ChartSel_ScaleH;          // ScaleH
    G_WaveScroll_FLAG           = CLRBIT;                   // Not enable waveform scroll
    G_State_OptionChanged_FLAG  = CLRBIT;                   // Reset option changed flag
    G_ADC_Running_FLAG          = SETBIT;                   // Set ADC running flag
    G_ADC_Interrupt_FLAG        = CLRBIT;                   // Reset ADC interrupt flag
    G_ADC_Complete_FLAG         = CLRBIT;                   // Reset ADC conversion complete flag
    G_WaveUpdate_FLAG           = CLRBIT;                   // Reset waveform update flag
    G_ClearWave_FLAG            = CLRBIT;                   // Reset waveform erasing flag
    G_ClearDisplay_FLAG         = SETBIT;                   // Set clearing display flag
    G_SEL_CONFIRM_FLAG          = CLRBIT;                   // Reset encoder rotation with movement flag
    G_UpdateVBAT_FLAG           = CLRBIT;                   // Reset battery update flag
    G_WaveFreq                  = 0;                        // Clear waveform frequency
    G_TriggerPos                = 50;                       // Set initial trigger position
    G_TriggerPosOffset          = 0;                        // Set initial trigger offset
    G_TriggerFail_FLAG          = CLRBIT;                   // Reset trigger failure flag
    G_VMax                      = 0;                        // Clear maximum waveform voltage
    G_VMin                      = 0;                        // Clear minimum waveform voltage
    G_WaveAvgLengthSumNum       = 0;                        // Clear average waveform length sum counts
    G_WaveAvgLengthSum          = 0;                        // Clear average waveform length sum
    G_PlotModeSel               = PlotMode_Vector;          // Vector
    G_Bias_Voltage              = 8900;                     // Bias voltage weight

    G_TriggerLevel_mV_Modified  = 2000;

    G_VMax_Modified             = 0;
    G_VMin_Modified             = 0;
    G_Voltage_Modified          = 0;
    G_VolV_Max_Modified         = 0;

    G_MeasureWaySel             = MeasureWay_AC;

    G_SELSwitcher_FLAG          = SETBIT;
    /* Disable switcher interrupt in waveform scroll mode when sampling*/
    if (G_ADC_Running_FLAG && G_WaveScroll_FLAG) {
        G_SELSwitcher_FLAG = CLRBIT;
    }

    BGV = Get_RAM_REFV();
}

void runWhenInSettings(void)
{
    ADC_Sample_Ready_LED = CLRBIT;
    G_ClearDisplay_FLAG  = SETBIT;
    while (G_State_Settings_FLAG) {
        if (G_SELOption_FLAG) {
            SelectInSettings (G_SELOption_Next);
            G_SELOption_FLAG = CLRBIT;
        }
        /* Update battery voltage information */
        if (G_UpdateVBAT_FLAG) {
            G_UpdateVBAT_FLAG = CLRBIT;
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL12, VBAT_RATIO);
        }
        /* Update OLED Brightness */
        if (G_OLED_BrightnessChanged_FLAG) {
            G_OLED_BrightnessChanged_FLAG = CLRBIT;
            OLED_WR_CMD(0x81);
            OLED_WR_CMD(G_OLED_Brightness * 10);
        }
        /* Update parameters */
        if (G_DisplayUpdate_FLAG) {
            G_DisplayUpdate_FLAG = CLRBIT;
            PlotSettings();
            OLED_Display();
        }
    }

    /* Save Settings */
    EA = CLRBIT;
    if (G_State_OptionChanged_FLAG){
        G_State_OptionChanged_FLAG  = CLRBIT;
        G_ClearDisplay_FLAG         = CLRBIT;
        /* Save settings and display saving status*/
        PlotSaveStatus(Save_Options());
        OLED_Display();
        delay_nms(1000);
    }

    /* Refresh screen after exiting settings */
    G_ClearDisplay_FLAG = SETBIT;
    PlotChart();
    OLED_Display();
    EA = SETBIT;
}

/*
    ADCRunning=1
    |   WaveUpdate=1
    |   |   ClearWave=1
    |   |   ClearWave=0
    |   WaveUpdate=0
    ADCRunning=0
        DisplayUpdate=1
            WaveUpdate=1
            |   ClearWave=1
            |   ClearWave=0
            WaveUpdate=0
*/
void runWhenADCInterrupt(void) {
    G_ADC_Interrupt_FLAG = CLRBIT;
    ADC_Sample_Ready_LED = CLRBIT;

    if (G_ADC_Running_FLAG) {
        G_SELSwitcher_FLAG      = CLRBIT; // Avoid Switcher Interrupt

        if (G_WaveUpdate_FLAG) {
            G_WaveUpdate_FLAG   = CLRBIT;
            G_ClearDisplay_FLAG = SETBIT;

            if (G_ClearWave_FLAG) {
                G_ClearWave_FLAG = CLRBIT;
                memset(ADCbuf, 0x00, ADC_SAMPLE_BUF_SIZE * 2);
                G_WaveFreq          = 0;
                G_TriggerFail_FLAG  = CLRBIT;
                G_VMax              = 0;
                G_VMin              = 0;
                PlotChart();
            } // Clear waveform in buffer
            else
            {
                AnalyzeData();
                PlotChart();
                PlotWave();
            } // Display waveform in buffer
        }
        else {
            G_ClearDisplay_FLAG = CLRBIT;
            PlotChart();
        }
        OLED_Display();

        /* Not to enable Switcher interrupt when both of ADCRunning and WaveScroll set. */
        if (!G_WaveScroll_FLAG) {
            G_SELSwitcher_FLAG      = CLRBIT; // Avoid Switcher Interrupt
        }
    } // ADC Sampling Running
    else {
        while (!G_ADC_Running_FLAG && !G_State_Settings_FLAG) {
            if (G_DisplayUpdate_FLAG) {
                G_SELSwitcher_FLAG      = CLRBIT; // Avoid Switcher Interrupt
                G_DisplayUpdate_FLAG    = CLRBIT;
                if (G_WaveUpdate_FLAG) {
                    G_WaveUpdate_FLAG   = CLRBIT;
                    /* Analyze completed sampling data in buffer */
                    AnalyzeData();
                    G_ClearDisplay_FLAG = SETBIT;

                    /* Clear display and draw waveform */
                    if (G_ScaleH == G_ScaleH_tmp) {
                        /* Since the sampling points limitation, scaling waveform along horizontal is not support.
                           Show waveform only when time scale is same as that when sampling completed */
                        PlotChart();
                        PlotWave();
                    }
                    else {
                        G_WaveFreq = 0; //Clear G_WaveFreq，display '****Hz'
                        PlotChart();
                    }
                } // Clear display and draw no waveform
                else
                {
                    G_ClearDisplay_FLAG = CLRBIT;
                    PlotChart();
                } // Update parameters on display only and maintain waveform

                OLED_Display();
                G_SELSwitcher_FLAG      = SETBIT; // Recover Switcher Interrupt
            }
        }
    } // ADC Sampling Stopped 
}

void runWhenADCComplete() {
    if (G_TriggerMode == TriggerSel_Single) {
        G_SELSwitcher_FLAG      = CLRBIT; // Avoid Switcher Interrupt
        G_ADC_Interrupt_FLAG    = SETBIT;
        G_DisplayUpdate_FLAG    = CLRBIT;
        G_WaveUpdate_FLAG       = CLRBIT;
        G_ADC_Running_FLAG      = CLRBIT; // Clear ADC running flag, stopping waveform sampling.
        AnalyzeData();
        G_ClearDisplay_FLAG     = SETBIT;
        PlotChart();
        PlotWave();
        OLED_Display();
        G_SELSwitcher_FLAG      = SETBIT; // Recover previous state
    } // ADC Sampling Complete - Single Trigger Mode
    else {
        G_ClearDisplay_FLAG     = SETBIT;   // Reset screen clearance flag
        AnalyzeData();                      // Analyze sampled data
        PlotChart();                        // Draw main interface
        PlotWave();                         // Draw waveform
        OLED_Display();
    } // ADC Sampling Complete - Auto or Normal Trigger Mode
}



void TM0_Init(void) {
    /* Timer 0, for updating voltage of battery */
    AUXR     &= ~AUXR_T0x12; // Timer clock is 12T mode
    TMOD     &= ~(TMOD_T0_CT | TMOD_T0_SEL | TMOD_T0_GATE); // Set timer work mode
    TL0      = 0xB0;         // Initial timer value LOW BYTE
    TH0      = 0x3C;         // Initial timer value HIGH BYTE
    TCON_TF0 = CLRBIT;       // Clear TF0 interrupt flag
    // TCON_TR0 = SETBIT;       // Timer0 start running
    ET0 = SETBIT; // Enable TM0 interrupt
} // 25ms@24.000MHz

void TM1_Init(void) {
    AUXR    &= ~AUXR_T1x12;  //Timer clock is 12T mode
    TMOD    &= ~(TMOD_T1_CT | TMOD_T1_SEL | TMOD_T1_GATE);  //Set timer work mode
    TL1      = 0xC0;         // Initial timer value LOW BYTE
    TH1      = 0x63;         // Initial timer value HIGH BYTE
    TCON_TF1 = CLRBIT;       // Clear TF1 flag
    TCON_TR1 = SETBIT;       // Timer1 start run
    ET1      = SETBIT;       // Enable TM1 interrupt
} //20ms@24.000MHz


/**
 * @brief TM0 interrupt for updating battery voltage infomation on screen.
 */
void TM0_ISR_Handler(void) interrupt(TM0_VECTOR) {
    static uint8_t n;

    if (++n >= VBAT_UPDATE_FREQ) {
        n = 0;
        G_UpdateVBAT_FLAG    = SETBIT; // Update VBAT info
        G_DisplayUpdate_FLAG = SETBIT; // Refresh VBAT info display
    }
    // printf("TM0 ISR\r\n");
}


bit MainInterface_FLAG = 0;
bit WaveScroll_FUNC_SEL_FLAG = 0;

void TM1_ISR_Handler(void) interrupt(TM1_VECTOR) {
    static uint8_t Trg0, read0, cont0;
    static uint8_t Trg1, read1, cont1;
    static uint8_t Trg2, read2, cont2;
    static uint8_t Trg3, read3, cont3;
    /* Next Option(P5.4) Last Option(P2.0) SETTING(P2.2) OptionsSel(P2.1), these pins are pull up high when idle.
       Therefore, we need to check if these pins are still high, otherwise the keys are pressed. */

    // P2.0
    read0 = (P2 & 0x01) ^ 0x01;
    Trg0  = read0 & (read0 ^ cont0) ; 
    cont0 = read0;

    // P2.1
    read1 = (P2 & 0x02) ^ 0x02;         // Read P2.2 and check if key is pressed. Was key pressed, read1 would be 1.
    Trg1  = read1 & (read1 ^ cont1);    /* If key is pressed, read1 will be 1. Compared with previous read value, 
                                           if key is pressed continuously, cont1=read1=1, Trg1 will be 0. Otherwise, if key 
                                           is pressed differing from previous one, Trg1=1. */
    cont1 = read1;                      // cont1 stores the previous read1 value
    
    // P2.2
    read2 = (P2 & 0x04) ^ 0x04;
    Trg2  = read2 & (read2 ^ cont2) ; 
    cont2 = read2;

    // P5.4
    read3 = P5 ^ 0xFF;
    Trg3  = read3 & (read3 ^ cont3); 
    cont3 = read3;

    if(Trg1) {
        if (!G_State_Settings_FLAG) {
            if (G_WaveScroll_FLAG) {
                G_WaveScroll_FLAG = ~G_WaveScroll_FLAG;
            } // WaveScroll Mode
            else {
                if (!G_SEL_CONFIRM_FLAG && !MainInterface_FLAG) {
                    G_SEL_CONFIRM_FLAG = SETBIT;
                } // Enter OPTION SELECTION Mode
                else if (G_SEL_CONFIRM_FLAG && !MainInterface_FLAG) {
                    G_SEL_CONFIRM_FLAG = CLRBIT;
                } // Exit OPTION SELECTION mode and then enter FUNCTION SELECTION mode
                else if (MainInterface_FLAG) {
                    G_WaveScroll_FLAG = ~G_WaveScroll_FLAG;
                    if (WaveScroll_FUNC_SEL_FLAG) {
                        G_SEL_CONFIRM_FLAG       = SETBIT;
                        WaveScroll_FUNC_SEL_FLAG = CLRBIT;
                    } 
                    else {
                        G_SEL_CONFIRM_FLAG = CLRBIT;
                    }
                    MainInterface_FLAG = CLRBIT;
                } // Exit FUNCTION SELECTION mode and then enter WaveScroll mode
            } // Chart Mode
            G_State_OptionChanged_FLAG  = SETBIT;   // Option has been changed
            G_ADC_Interrupt_FLAG        = SETBIT;   // ADC sampling interrupt
            G_DisplayUpdate_FLAG        = SETBIT;   // Update Screen
            G_ClearWave_FLAG            = CLRBIT;   // Clear waveform
        }

        if (G_State_Settings_FLAG) {
            G_SEL_CONFIRM_FLAG = ~G_SEL_CONFIRM_FLAG;
        }

        Trg1 = 0;
    } 

    if(Trg2) {
            if (!G_State_Settings_FLAG && G_WaveScroll_FLAG) {
                G_State_Settings_FLAG = ~G_State_Settings_FLAG;
            } // In MAIN interface and 'G_WaveScroll' is set, current state can enter SETTING interface
            else if (!G_State_Settings_FLAG && !G_WaveScroll_FLAG && !MainInterface_FLAG) {
                MainInterface_FLAG = SETBIT;
            } // In MAIN interface and 'G_WaveScroll' is unset, current state can not enter SETTING interface,
              // but 'Trg2' pressed will lock the LAST/NEXT option selection and tell the MCU to exit FUNCTION
              // SELECTION mode.
            else if (!G_State_Settings_FLAG && !G_WaveScroll_FLAG && MainInterface_FLAG) {
                WaveScroll_FUNC_SEL_FLAG = SETBIT;
            }
            else {
                G_State_Settings_FLAG = ~G_State_Settings_FLAG;
            }
            // Enter setting interface
            if (G_State_Settings_FLAG) {
                G_DisplayUpdate_FLAG    = SETBIT;   // Update Screen
                G_UpdateVBAT_FLAG       = SETBIT;   // Update VBAT information
                TCON_TF0                = CLRBIT;   // Clear TM0 overflow flag
                TCON_TR0                = SETBIT;   // TM0 start, preparing for VBAT information updating.
                G_SELSwitcher_FLAG      = SETBIT;   
            } // Enter Settings
            else {
                TCON_TR0                = CLRBIT;   // Clear TM0 overflow flag
                TCON_TF0                = CLRBIT;   // TM0 is stopped, suspending the VBAT infomation updating.
                G_WaveFreq              = 0;        // Reset waveform frequency 
                G_TriggerFail_FLAG      = CLRBIT;   // Clear trigger failure flag
                G_VMax                  = 0;        // Reset maximum waveform voltage
                G_VMin                  = 0;        // Reset minimum waveform voltage
                G_DisplayUpdate_FLAG    = SETBIT;   // Update screen display
                G_WaveUpdate_FLAG       = SETBIT;   // Update waveform information
                G_ClearWave_FLAG        = CLRBIT;   // Don't clear waveform on screen
            } // Return to main interface
             
            G_ADC_Interrupt_FLAG = SETBIT; // Stop sampling ADC information and returned sampled value.
        Trg2 = 0;
    }

    if(Trg3) {
        if (MainInterface_FLAG && G_SELSwitcher_FLAG) {
            G_SELOption_FLAG        = CLRBIT;
        }
        else {
            G_SELOption_FLAG        = SETBIT;
            G_SELOption_Next        = CLRBIT;
            G_ADC_Interrupt_FLAG    = SETBIT;
            G_DisplayUpdate_FLAG    = SETBIT;
        }
        Trg3 = 0;
    } // Next Option

    if (Trg0) {
        if (MainInterface_FLAG && G_SELSwitcher_FLAG) {
            G_SELOption_FLAG        = CLRBIT;
        }
        else {
            G_SELOption_FLAG        = SETBIT;
            G_SELOption_Next        = SETBIT;
            G_ADC_Interrupt_FLAG    = SETBIT;
            G_DisplayUpdate_FLAG    = SETBIT;
        }
        Trg0 = 0;
    } // Last Option
}



/* Need to add a new key */
void Trg4_STOP_RUN(void) {
    if (!G_State_Settings_FLAG)
    {
        G_SELSwitcher_FLAG = CLRBIT;
        G_ADC_Running_FLAG = ~G_ADC_Running_FLAG;
        if (G_ADC_Running_FLAG)
        {
            G_WaveUpdate_FLAG   = SETBIT;
            G_ClearWave_FLAG    = SETBIT;
        }
        else
        {
            G_DisplayUpdate_FLAG = SETBIT;
            G_WaveUpdate_FLAG    = SETBIT;
        }
        G_ADC_Interrupt_FLAG    = SETBIT;
        G_SELSwitcher_FLAG      = SETBIT;
    }
}