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
    uint16_t ADC_REFV;
    uint8_t Pstate;

    GPIO_DeInit(); // Set all Px mode to quasi bidirectional mode 

    /* Set OLED port P2.3, P2.4, P2.5, P2.6 as quasi bidirectional */
    P2M1 &= ~0xE8;
    P2M0 &= ~0xE8;
    /* Set ADC port P0.4, P0.6 as high-impedance */
    P0M1 |= 0x50;
    P0M0 &= ~0x50;
    /* Set indicator port P1.1 as push-pull output */
    P0M1 &= ~0x02;
    P0M0 |= 0x02;
    /* Set Encoder port P3.2, P3.3, P3.4 as quasi bidirectional */
    P3M1 &= ~0x1C;
    P3M0 &= ~0x1C;
    /* Set P0.0, P0.1 as high impedance */
    P0M1 |= 0x03;
    P0M0 &= ~0x03;


    TM0_Init();
    TM1_Init();
    UART1_Init();
    Read_Options();
    GlobalVarInit();
    delay_nus(20);

    OLED_Init();
    OLED_SetFontWidth(6);
    PlotChart();
    OLED_Display();

    /* Enable global interrupt */
    EA = SETBIT;


    while (1) {
        ADC_Sample_Ready_LED = CLRBIT;
        if (G_UpdateVBAT_FLAG) {
            G_UpdateVBAT_FLAG = CLRBIT; // Prepare for next VBAT info interrupt
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL4, VBAT_RATIO);
        }

        GetWaveData(); // Sample waveform

        if (G_OptionInSettings) {
            runWhenInSettrings();
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



void MainInit(void) {
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
    G_EC11PressWithRotate_FLAG  = CLRBIT;                   // Reset encoder rotation with movement flag
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

    if (G_ADC_Running_FLAG && G_WaveScroll_FLAG) {
        EX0 = 0;
    } // Disable external interrupt 0(Encoder rotation) in waveform scroll mode when sampling
}

void Encoder_Init(void) {
    /* Interrupt for rotating of Encoder */
    TCON_IT0    = CLRBIT; // External interrupt 0 trigger way, rasing edge or falling edge.
    /* External interrupt 0 high priority, which can interrupt the key pressing,
       used to identified a state that encoder is pressed and simultaneously rotated. */
    IPH        &= IPH_PX0H; 
    PX0         = SETBIT; 
    EX0         = SETBIT; // Start external interrupt 0

    /* Interrupt for clicking of Encoder */
    TCON_IT1    = SETBIT; // External interrupt 1 trigger way, falling edge.
    /* External interrupt 1 low priority */
    IPH        &= IPH_PX1H; 
    PX1         = CLRBIT;
    EX1         = SETBIT; // Start external interrupt 1
}

void TM0_Init(void) {
    /* Timer 0, for updating voltage of battery */
    AUXR     &= ~AUXR_T0x12; // Timer clock is 12T mode
    TMOD     &= ~(TMOD_T0_CT | TMOD_T0_SEL_16B_AUTO_RELOAD | TMOD_T0_GATE); // Set timer work mode
    TL0      = 0xB0;         // Initial timer value LOW BYTE
    TH0      = 0x3C;         // Initial timer value HIGH BYTE
    TCON_TF0 = CLRBIT;       // Clear TF0 interrupt flag
    TCON_TR0 = SETBIT;       // Timer0 start running
    ET0      = SETBIT;       // Enable TM0 interrupt
} // 25ms@24.000MHz

void TM1_Init(void) {
    AUXR    &= ~AUXR_T1x12;  //Timer clock is 12T mode
    TMOD    &= ~(TMOD_T1_CT | TMOD_T1_SEL_16B_AUTO_RELOAD | TMOD_T1_GATE);  //Set timer work mode
    TL1      = 0xC0;         // Initial timer value LOW BYTE
    TH1      = 0x63;         // Initial timer value HIGH BYTE
    TCON_TF1 = CLRBIT;       // Clear TF1 flag
    TCON_TR1 = SETBIT;       // Timer1 start run
    ET1      = SETBIT;       // Enable TM1 interrupt
} //20ms@24.000MHz


/**
 * @brief Analyse Rotation of Encoder 
 */
void Scan_EC11(void) {
    /* Delay to remove jitter */
    delay_nus(500);

    if (EC11_A != EC11_B) {
        Select_Option(1);
    } // Clockwise

    else if (EC11_A == EC11_B) {
       Select_Option(0);
    } // Anticlockwise
}

/**
 * @brief Interrput for Encoder Rotated 
 */
void INT0_ISR_Handler(void) interrupt(EXTI0_VECTOR) {
    /* Delay to remove jitter */
    delay_nus(500);

    /* Whether the Encoder is pressed or not */
    if (!EC11_KEY) {
        G_EC11PressWithRotate_FLAG = SETBIT;
    }
    else {
        G_EC11PressWithRotate_FLAG = CLRBIT;
    }

    Scan_EC11();
    G_ADC_Interrupt_FLAG = SETBIT;
    G_DisplayUpdate_FLAG = SETBIT;

    TCON_IE1 = CLRBIT; // Clear external interrupt 1 flag 
    TCON_IE0 = CLRBIT; // Clear external interrupt 0 flag 
}


/**
 * @brief Interrupt for Encoder Pressed
 */
void INT1_ISR_Handler(void) interrupt(EXTI1_VECTOR) {
    delay_nms(50);
    if (!EC11_KEY) {
        G_EC11PressWithRotate_FLAG = CLRBIT;
        if (G_EC11PressWithRotate_FLAG) {
            ;/* Operations Performed by Interrupt of Encoder Rotation */
        } // Rotate Encoder while pressing
        else if (!G_State_Settings_FLAG) {
            EX0 = CLRBIT;
            G_ADC_Running_FLAG = ~G_ADC_Running_FLAG;
            if (G_ADC_Running_FLAG) {
                G_WaveUpdate_FLAG       = SETBIT;
                G_ClearWave_FLAG        = SETBIT;
            }
            else {
                G_DisplayUpdate_FLAG    = SETBIT;
                G_WaveUpdate_FLAG       = SETBIT;
            }
            G_ADC_Interrupt_FLAG = SETBIT;
            TCON_IE0 = CLRBIT;
            TCON_IE1 = CLRBIT;
        } // Single click Encoder - Switch Run/Stop in main interface
    }
    TCON_IE1 = CLRBIT;  // Clear external interrupt 1 request flag
}


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
}


void TM1_ISR_Handler(void) interrupt(TM1_VECTOR) {
    static uint8_t Trg1, read1, cont1;
    static uint8_t Trg2, read2, cont2;
    static uint8_t Trg3, read3, cont3;

    /* RST(P5.4) SETTING(P2.2) OptionsSel(P2.1), these pins are pull up high when idle.
       Therefore, we need to check if these pins are still high, otherwise the keys are pressed. */

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
    read3 = (P5 & 0x10) ^ 0x10;
    Trg3  = read3 & (read3 ^ cont3); 
    cont3 = read3;

    if(Trg1) {
        /* Main Interface */
        if (!G_State_Settings_FLAG) {
            G_WaveScroll_FLAG           = ~G_WaveScroll_FLAG;
            G_State_OptionChanged_FLAG  = SETBIT;   // Option has been changed
            G_ADC_Interrupt_FLAG        = SETBIT;   // ADC sampling interrupt
            G_DisplayUpdate_FLAG        = SETBIT;   // Update Screen
            G_ClearWave_FLAG            = CLRBIT;   // Clear waveform
        } // Options changed
        Trg1 = 0;
    }

    if(Trg2) {
            G_State_Settings_FLAG = ~G_State_Settings_FLAG;
            // Enter setting interface
            if (G_State_Settings_FLAG) {
                G_DisplayUpdate_FLAG    = SETBIT;   // Update Screen
                G_UpdateVBAT_FLAG       = SETBIT;   // Update VBAT information
                TCON_TF0                = CLRBIT;   // Clear TM0 overflow flag
                TCON_TR0                = SETBIT;   // TM0 start, preparing for VBAT information updating.
                TCON_IE0                = CLRBIT;   // Clear external interrupt flag
                EX0                     = SETBIT;   // Start external interrupt 0 (Encoder rotation)
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
        SP   = 0;
        Trg3 = 0;
    } // RESET
}


void runWhenInSettings(void)
{
    ADC_Sample_Ready_LED = CLRBIT;
    G_ClearDisplay_FLAG  = SETBIT;
    while (G_State_Settings_FLAG) {
        /* Update battery voltage information */
        if (G_UpdateVBAT_FLAG) {
            G_UpdateVBAT_FLAG = CLRBIT;
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL4, VBAT_RATIO);
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
    EA = 0;
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
    TCON_IE0 = CLRBIT;
    TCON_IE1 = CLRBIT;
    EA = 1;
}

/*
    ADCRuning=1
    |   WaveUpdate=1
    |   |   ClearWave=1
    |   |   ClearWave=0
    |   WaveUpdate=0
    ADCRuning=0
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
        EX0 = CLRBIT;

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

        /* Not to enable external interrupt 0(Encoder Rotation) when both of ADCRunning and WaveScroll set. */
        if (!G_WaveScroll_FLAG) {
            TCON_IE0    = CLRBIT;
            EX0         = SETBIT;
        }
    } // ADC Sampling Running
    else {
        while (!G_ADC_Running_FLAG && !G_State_Settings_FLAG) {
            if (G_DisplayUpdate_FLAG) {
                EX0 = CLRBIT;
                G_DisplayUpdate_FLAG = CLRBIT;
                if (G_WaveUpdate_FLAG) {
                    G_WaveUpdate_FLAG = CLRBIT;
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
                TCON_IE0    = CLRBIT;
                EX0         = SETBIT;
            }
        }
    } // ADC Sampling Stopped 
}

void runWhenADCComplete() {
    if (G_TriggerMode == TriggerSel_Single)
    {
        EX0                     = CLRBIT;
        G_ADC_Interrupt_FLAG    = SETBIT;
        G_DisplayUpdate_FLAG    = CLRBIT;
        G_WaveUpdate_FLAG       = CLRBIT;
        G_ADC_Running_FLAG      = CLRBIT; //清零ADC运行标志，停止采样
        AnalyzeData();
        G_ClearDisplay_FLAG     = SETBIT;
        PlotChart();
        PlotWave();
        OLED_Display();
        TCON_IE0 = CLRBIT;
        EX0 = SETBIT;
    } // ADC Sampling Complete - Single Trigger Mode
    else {
        //EX0 = 0;
        G_ClearDisplay_FLAG     = SETBIT;   // Reset screen clearance flag
        AnalyzeData();                      // Analyze sampled data
        PlotChart();                        // Draw main interface
        PlotWave();                         // Draw waveform
        OLED_Display();
        //IE0 = 0;
        //EX0 = 1;
    } // ADC Sampling Complete - Auto or Normal Trigger Mode
}
