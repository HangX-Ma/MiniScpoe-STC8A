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

/* TM0: VBAT info updating rate */
/* TM1: Keys press types recognition, short or long */
/* TM3: UART1 Rx out of time */

/*
    -----------------------------------------  [INTRODUCTION]  -----------------------------------------
    OPERATION INTRO:
        Main Interface - Options Selection Mode: 
            SW Button:              Switch Stop/Run
            Short Press SETTING:    Switch to [Waveform Horizontal Scroll Mode]
            SEL:                    Select or deselect this option.
            NEXT/INC:               Jump to next option or increase value.
            LAST/DEC:               Jump to last option or decrease value.


        Main Interface - Waveform Horizontal Scroll Mode:
            SW Button:              Switch Stop/Run
            Short Press SETTING:    Switch to [Options Selection Mode]
            [SEL] SET(Horizontal waveform scroll, Valid in suspended sampling state):
                NEXT/INC:           Jump to next option or increase value.
                LAST/DEC:           Jump to last option or decrease value.
            [SEL] RESET(Vertical waveform scroll, Valid in suspended sampling state):
                NEXT/INC:           Jump to next option or increase value.
                LAST/DEC:           Jump to last option or decrease value.

        Setting Interface: (SETTING)
            Long Press SETTING: Return to main interface or enter setting interface from main interface.
            SEL:                    Select or de-select this option.
            NEXT/INC:               Jump to next option or increase value.
            LAST/DEC:               Jump to last option or decrease value.
    ----------------------------------------------------------------------------------------------------
    MAIN INTERFACE INTRO:
        Vertical Time Range:     "500ms", "200ms", "100ms", "50ms", "20ms", "10ms","5ms", "2ms", "1ms", "500us", "200us", "100us"(100us only used in AUTO Mode)
        Horizontal Voltage Range: Set voltage upper range, whose maximum value is 30V. If the voltage upper range is set to '0', system run into AUTO mode.
        Trigger Value:            Set trigger voltage value. The trigger position is set to the screen center.
        Trigger Direction:        Use up or down arrow indicator, separately representing rising edge trigger and falling edge trigger.
        Trigger Mode:
            AUTO:   Sample continuously, whose indicator led is always turned on. SW Button is used to select RUN or STOP;
                    If trigger process is successful, trigger position is set to the screen center, otherwise the waveform scrolling irregularly. FAIL will be shown.
            NORMAL: Wait for prerequisite sampling. Indicator led is turned on, indicating the prerequisite sampling finished. Signal can be entered.
                    After successfully triggering, display the waveform, and continue to wait for the next trigger;
                    If a new trigger is successful, the new waveform will be displayed automatically. If no new trigger is successful, the screen waveform will remain and wait for the next trigger;
                    (Be careful to set the correct trigger value, otherwise has been waiting for the trigger state, the screen will not display the waveform)
            SINGLE: Wait for prerequisite sampling. Indicator led is turned on, indicating the prerequisite sampling finished. Signal can be entered.
                    After triggering successfully, display the waveform and stop sampling, the user needs to click on the encoder to start the next sampling.
                    (Be careful to set the correct trigger value, otherwise has been waiting for the trigger state, the screen will not display the waveform)
    ----------------------------------------------------------------------------------------------------
    OPTIONS SETTING SAVING:
        Long press SETTING key to leave the setting interface. If options have been changed, the program will save the setting to EEPROM.
    LVD EVENT:
        If LVD event happens, system will not allow user to operate EEPROM to save data.
    ----------------------------------------------------------------------------------------------------
    MAIN INTERFACE STATUS: 
        RUN:        Sampling in progress 
        STOP:       Stop Sampling 
        FAIL:       In AUTO mode, trigger value exceeds waveform range causing trigger failure 
        SUCC:       In automatic mode, the trigger value displays the trigger success in the waveform range.
        AUTO:       Auto range.
    ----------------------------------------------------------------------------------------------------
    SETTING INTERFACE STATUS: 
        PMODE (drawing mode) : 
            Vector: waveform display using vector 
            Dots:   waveform display using dots
        LSB (sampling factor) : 100 times the partial voltage factor, (8.2K+2K)/2K*100 = 510. Adjust this value subtly in accordance with actual register values.
        BRT:    OLED brightness
    ----------------------------------------------------------------------------------------------------
 */
#include "main.h"
#include "STDIO.H"
#include "STDLIB.H"
#include "STRING.H"
#include "stc8x_sysclk.h"


bit ifKeyPressed;
static uint8_t keyPressedTimeCount;

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
    /* Set P1.7 as PWM output pin, quasi bidirectional */
    P1M1 &= ~0x80;
    P1M0 &= ~0x80;
    /* Set OLED port P2.3, P2.4, P2.5, P2.6, P2.7 as quasi bidirectional */
    P2M1 &= ~0xF8;
    P2M0 &= ~0xF8;
    /* Set ADC port P0.4, P0.6 as high-impedance */
    P0M1 |= 0x50;
    P0M0 &= ~0x50;
    /* P2.0(SEL), P2.1(LAST/DEC), P2.2(EX) as high-impedance */
    P2M1 |= 0x07;
    P2M0 &= ~0x07;
    /* Set P4.1(SETTING), P4.2(NEXT/INC) as high-impedance */
    P4M1 |= 0x06;
    P4M0 &= ~0x06;
    /* Set Button P3.2 as high-impedance */
    P3M1 &= ~0x04;
    P3M0 &= ~0x04;
    /* Set indicator port P3.5 as push-pull output */
    P3M1 &= ~0x20;
    P3M0 |= 0x20;

    P_SW2  |= P_SW2_EAXFR;
    /* Falling edge interrupt */
    P2IM0  &= ~0x07;
    P2IM1  &= ~0x07;
    P4IM0  &= ~0x06;
    P4IM1  &= ~0x06;
    /* Lowest Priority */
    PINIPL &= ~0x14;
    PINIPH &= ~0x14;
    /* Enable I/O interrupt */
    P2INTE |= 0x07;
    P4INTE |= 0x06;
    P_SW2 &= ~P_SW2_EAXFR;
    
    RST_Init();
    PWM_config();
    Switch_Init();
    TM0_Init();
    UART1_Init();
    TM1_Init();
    Read_Options();
    GlobalVar_Init();
    delay_nus(50);

    /* OLED initialization */
    OLED_Init();
    OLED_SetFontWidth(6);
    PlotChart();
    OLED_Display();

    EA = SETBIT;


    while (1) {
        /* Update voltage infor of battery */
        if (G_UpdateVBAT_FLAG) {
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL12, VBAT_RATIO);
            G_UpdateVBAT_FLAG = CLRBIT;
        }

        GetWaveData(); // Sample waveform

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

void runWhenInSettings(void) {
    ADC_Sample_Ready_LED = CLRBIT;
    G_ClearDisplay_FLAG  = SETBIT;

    while (G_State_Settings_FLAG) {
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
        P_SW2  |= P_SW2_EAXFR;
        /* Disable I/O interrupt P4.2, P2.1 */
        P2INTE &= ~0x02;
        P4INTE &= ~0x04;
        P_SW2 &= ~P_SW2_EAXFR;

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
            else {
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
            P_SW2  |= P_SW2_EAXFR;
            /* Enable I/O interrupt P4.2, P2.1 */
            P2INTE |= 0x02;
            P4INTE |= 0x04;
            P_SW2 &= ~P_SW2_EAXFR;
        }
    } // ADC Sampling Running
    else {
        while (!G_ADC_Running_FLAG && !G_State_Settings_FLAG) {
            if (G_DisplayUpdate_FLAG) {
                 P_SW2  |= P_SW2_EAXFR;
                /* Disable I/O interrupt P4.2, P2.1 */
                P2INTE &= ~0x02;
                P4INTE &= ~0x04;

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

                /* Enable I/O interrupt P4.2, P2.1 */
                P2INTE |= 0x02;
                P4INTE |= 0x04;
                P_SW2 &= ~P_SW2_EAXFR;
            }
        } // ADC Sampling Stopped 
    }
}


void runWhenADCComplete() {
    if (G_TriggerMode == TriggerSel_Single)
    {
        P_SW2  |= P_SW2_EAXFR;
        /* Disable I/O interrupt P4.2, P2.1 */
        P2INTE &= ~0x02;
        P4INTE &= ~0x04;
        G_ADC_Interrupt_FLAG    = SETBIT;
        G_DisplayUpdate_FLAG    = CLRBIT;
        G_WaveUpdate_FLAG       = CLRBIT;
        G_ADC_Running_FLAG      = CLRBIT; // ADC Stop running 
        AnalyzeData();
        G_ClearDisplay_FLAG     = SETBIT;
        PlotChart();
        PlotWave();
        OLED_Display();
        TCON_IE0                = CLRBIT;
        /* Enable I/O interrupt P4.2, P2.1 */
        P2INTE |= 0x02;
        P4INTE |= 0x04;
        P_SW2 &= ~P_SW2_EAXFR;
    } // ADC Sampling Complete - Single Trigger Mode
    else {
        G_ClearDisplay_FLAG     = SETBIT;   // Reset screen clearance flag
        AnalyzeData();                      // Analyze sampled data
        PlotChart();                        // Draw main interface
        PlotWave();                         // Draw waveform
        OLED_Display();
    } // ADC Sampling Complete - Auto or Normal Trigger Mode
}



void GlobalVar_Init(void) {
    G_State_Settings_FLAG         = CLRBIT;                 // Not enter setting interface
    G_WaveScroll_FLAG             = CLRBIT;                 // Not enable waveform scroll

    G_UpdateVBAT_FLAG             = CLRBIT;                 // Reset battery update flag
    G_OLED_BrightnessChanged_FLAG = CLRBIT;                 // Reset OLED brightness update flag
    G_State_OptionChanged_FLAG    = CLRBIT;                 // Reset option changed flag

    /* Default ADC functions state */
    G_ADC_Running_FLAG            = SETBIT;                 // Set ADC running flag
    G_ADC_Interrupt_FLAG          = CLRBIT;                   // Reset ADC interrupt flag
    G_ADC_Complete_FLAG           = CLRBIT;                   // Reset ADC conversion complete flag

    /* Default Option for [setting interface] and [main interface]*/
    G_OptionInSettings            = SettingSel_PlotMode;    // default: PlotMode
    G_OptionInChart               = ChartSel_ScaleH;        // default: ScaleH
    G_MeasureWaySel               = MeasureWay_AC;          // default: AC measure way
    G_PlotModeSel                 = PlotMode_Vector;        // default: Vector

    /* Display setting */
    G_ClearDisplay_FLAG           = SETBIT;                 // Set clearing display flag
    G_ClearWave_FLAG              = CLRBIT;                 // Reset waveform erasing flag
    G_WaveUpdate_FLAG             = CLRBIT;                 // Reset waveform update flag

    /* Indicator */
    ADC_Sample_Ready_LED          = CLRBIT;                 // ADC sampling indicator off
    ifKeyPressed                  = CLRBIT;                 // Initially, keys are not pressed

    /* Basic Config */
    G_VMax_Modified             = 0;
    G_VMin_Modified             = 0;
    G_Voltage_Modified          = 0;
    G_VolV_Max_Modified         = 0;

    G_Bias_Voltage              = 8900;                     // Bias voltage weight

    /* Waveform Config */
    G_VMax                      = 0;                        // Clear maximum waveform voltage
    G_VMin                      = 0;                        // Clear minimum waveform voltage
    G_WaveAvgLengthSumNum       = 0;                        // Clear average waveform length sum counts
    G_WaveAvgLengthSum          = 0;                        // Clear average waveform length sum
    G_WaveFreq                  = 0;                        // Clear waveform frequency

    /* Trigger Config */
    G_TriggerPos                = 50;                       // Set initial trigger position
    G_TriggerPosOffset          = 0;                        // Set initial trigger offset
    G_TriggerFail_FLAG          = CLRBIT;                   // Reset trigger failure flag
    G_TriggerLevel_mV_Modified  = 2000;

    /* Disable external key interrupt [NEXT/INC] and [LAST/DEC] in waveform scroll mode when sampling */
    if (G_ADC_Running_FLAG && G_WaveScroll_FLAG) {
        P_SW2  |= P_SW2_EAXFR;
        /* Disable I/O interrupt P4.2, P2.1 */
        P2INTE &= ~0x02;
        P4INTE &= ~0x04;
        P_SW2 &= ~P_SW2_EAXFR;
    } 

    BGV_RAM                       = Get_RAM_REFV();
}

void RST_Init(void) {
    RSTCFG  = RSTCFG_P54RST | RSTCFG_LVDS_3V0; // Select P5.4 as the RST pin. LVD threshold voltage is 3.0V.
    RSTCFG &= ~RSTCFG_ENLVR; // Disable LVD reset
    PCON   &= ~PCON_LVDF;    // Clear interrupt flag
    delay_nus(500);
    ELVD    = SETBIT;        // Enable LVD interrupt
}


void Switch_Init(void) {
    TCON_IT0 = CLRBIT;   // Enable INT0, rising edge and falling edge interrupt

    /* External interrupt 0 level 1 priority, which can interrupt the key pressing, 
    used to identified a state that switch button is pressed or not. */
    PX0      = SETBIT; 
    EX0      = SETBIT; // Start external interrupt 0
}

void TM0_Init(void) {
    /* Timer 0, for updating voltage of battery */
    AUXR     &= ~AUXR_T0x12; // Timer clock is 12T mode
    TMOD     &= ~(TMOD_T0_CT | TMOD_T0_SEL | TMOD_T0_GATE); // Set timer work mode
    TL0      = 0xB0;         // Initial timer value LOW BYTE
    TH0      = 0x3C;         // Initial timer value HIGH BYTE
    TCON_TF0 = CLRBIT;       // Clear TF0 interrupt flag
    ET0      = SETBIT;       // Enable TM0 interrupt
    TCON_TR0 = CLRBIT;       // Timer0 stop running
} // 25ms@24.000MHz


void TM1_Init(void) {
    AUXR    &= ~AUXR_T1x12;  //Timer clock is 12T mode
    TMOD    &= ~(TMOD_T1_CT | TMOD_T1_SEL | TMOD_T1_GATE);  //Set timer work mode
    TL1      = 0xE0;         // Initial timer value LOW BYTE
    TH1      = 0xB1;         // Initial timer value HIGH BYTE
    TCON_TF1 = CLRBIT;       // Clear TF1 flag
    ET1      = SETBIT;       // Enable TM1 interrupt
    TCON_TR1 = CLRBIT;       // Timer1 stop running
} //10ms@24.000MHz

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
    if (keyPressedTimeCount >= 0 && keyPressedTimeCount < 255) {
        keyPressedTimeCount++;
    } 
    else {
        keyPressedTimeCount = 255;
    } // Prevent data overflow
    TCON_TF1 = CLRBIT; // Clear TF1 interrupt flag
}

/**
 * @brief INT0 interrupt used to determine the state of the switch button.
 */
void INT0_ISR_Handler(void) interrupt(EXTI0_VECTOR) {
    delay_nms(10); // Jitters elimination

    EX0      = CLRBIT;

    /* Main Interface: Switch Stop/Run state */
    if (!G_State_Settings_FLAG) {
        /* Disable [NEXT/INC] and [LAST/DEC]*/
        P_SW2  |= P_SW2_EAXFR;
        /* Disable I/O interrupt P4.2[NEXT/INC], P2.1[LAST/DEC] */
        P2INTE &= ~0x02;
        P4INTE &= ~0x04;
        P_SW2 &= ~P_SW2_EAXFR;

        if (SW_Button) {
            G_ADC_Running_FLAG = SETBIT;
        } 
        else {
            G_ADC_Running_FLAG = CLRBIT;
        } // Button state detection

        if (G_ADC_Running_FLAG) {
            G_WaveUpdate_FLAG    = SETBIT;
            G_ClearWave_FLAG     = SETBIT;
        }
        else {
            G_DisplayUpdate_FLAG = SETBIT;
            G_WaveUpdate_FLAG    = SETBIT;
        }
        G_ADC_Interrupt_FLAG     = SETBIT;
    }
    EX0      = SETBIT;
    TCON_IE0 = CLRBIT;
}


/**
 * @brief Long pressed key functions
 */
void LPK_functions(void) {
    /* Determine witch key is pressed */
    if (P2INTF) {
        if (P2INTF & 0x01) {
            ADC_Sample_Ready_LED = SETBIT;
        } // P2.0(SEL)

        if (P2INTF & 0x04) {
            ADC_Sample_Ready_LED = SETBIT;
        } // P2.2(EX)
    }

    if (P4INTF) {
        if (P4INTF & 0x02) {
            G_State_Settings_FLAG = ~G_State_Settings_FLAG; // Switch 
            // Enter setting interface
            if (G_State_Settings_FLAG) {
                G_DisplayUpdate_FLAG    = SETBIT;   // Update Screen
                G_UpdateVBAT_FLAG       = SETBIT;   // Update VBAT information
                TCON_TF0                = CLRBIT;   // Clear TM0 overflow flag
                TCON_TR0                = SETBIT;   // TM0 start, preparing for VBAT information updating.
                /* Enable I/O interrupt P4.2[NEXT/INC], P2.1[LAST/DEC] */
                P2INTE |= 0x02;
                P4INTE |= 0x04;
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
        } // P4.1(SETTING) [Enter/Exit Setting Interface]
    }
}

/**
 * @brief Short pressed key functions
 */
void SPK_functions(void) {
    /* Determine witch key is pressed */
    if (P2INTF) {
        if (P2INTF & 0x01) {
            G_SEL_CONFIRM_FLAG = ~G_SEL_CONFIRM_FLAG;
        } // P2.0(SEL)

        if (P2INTF & 0x02) {
            G_SELOption_Next        = CLRBIT;   // Last value or increase value
            if (!G_State_Settings_FLAG) {
                SelectInChart(G_SELOption_Next);
            } 
            else {
                SelectInSettings(G_SELOption_Next);
            }
            G_ADC_Interrupt_FLAG    = SETBIT;   // ADC sampling process is interrupted by key
            G_DisplayUpdate_FLAG    = SETBIT;   // Update OLED display
        } // P2.1(LAST/DEC)

        if (P2INTF & 0x04) {
            ADC_Sample_Ready_LED = CLRBIT;
        } // P2.2(EX)
    }

    if (P4INTF) {
        if (P4INTF & 0x02) {
            /* In Main Interface */
            if (!G_State_Settings_FLAG)
            {
                G_WaveScroll_FLAG           = ~G_WaveScroll_FLAG; // Change Main Interface Mode
                G_State_OptionChanged_FLAG  = SETBIT;
                G_ADC_Interrupt_FLAG        = SETBIT;
                G_DisplayUpdate_FLAG        = SETBIT;
                G_ClearWave_FLAG            = CLRBIT;
            }
        } // P4.1(SETTING)

        if (P4INTF & 0x04) {
            G_SELOption_Next        = SETBIT;   // Next value or decrease value
            if (!G_State_Settings_FLAG) {
                SelectInChart(G_SELOption_Next);
            } 
            else {
                SelectInSettings(G_SELOption_Next);
            }
            G_ADC_Interrupt_FLAG    = SETBIT;   // ADC sampling process is interrupted by key
            G_DisplayUpdate_FLAG    = SETBIT;   // Update OLED display
        } // P4.2(NEXT/INC)
    }
}


/**
 * @brief Keys I/O interrupt, falling edge detection
 */
void Common_ISR_Handler(void) interrupt(13) {
    delay_nms(20); // Jitters elimination

    P_SW2  |= P_SW2_EAXFR;

    if (!ifKeyPressed) {
        ifKeyPressed        = SETBIT; // Key has been pressed, change I/O trigger method.
        keyPressedTimeCount = 0;      // Clear previous value
        /* Rising edge interrupt */
        P2IM0   |= 0x07;
        P2IM1   &= ~0x07;
        P4IM0   |= 0x06;
        P4IM1   &= ~0x06;
        /* Start TM1 to record the pressing time */
        TCON_TR1 = SETBIT;
    } // Waiting for rising edge (Key released)
    else {
        TCON_TR1     = CLRBIT; // Stop TM1
        ifKeyPressed = CLRBIT; // recover state
        /* Falling edge interrupt */
        P2IM0  &= ~0x07;
        P2IM1  &= ~0x07;
        P4IM0  &= ~0x06;
        P4IM1  &= ~0x06;

        if (keyPressedTimeCount >=50) {
            LPK_functions();
        } // Duration time over 500~600ms, long press
        else {
            SPK_functions();
        } // Duration time within 500~600ms, short press
        keyPressedTimeCount = 0; // Clear previous value
    } // Key released, recover state.
    /* Clear interrupt flag */
    P2INTF &= ~0x07;
    P4INTF &= ~0x06;

    P_SW2 &= ~P_SW2_EAXFR;
}

void RST_ISR_Handler(void) interrupt(LVD_VECTOR) {
    G_LVD_EVENT_FLAG = SETBIT;      // Set LVD event flag 
    PCON            &= ~PCON_LVDF;
}