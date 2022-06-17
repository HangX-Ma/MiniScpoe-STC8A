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
    ----------------------------------  [INTRODUCTION]  ----------------------------------
    OPERATION INTRO:
        Main Interface - Options Selection Mode: 
    

        Setting Interface: (SETTING)
            Long Press SETTING: Return to main interface or enter setting interface from main interface.
            SEL:                Select or de-select this option.
            NEXT/INC:           Jump to next option or increase value.
            LAST/DEC:           Jump to last option or decrease value.
            

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

        if (G_State_Settings_FLAG) {
            runWhenInSettings();
        } // Enter Settings Interface

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
    TCON_IE0 = CLRBIT;
    EA       = SETBIT;
}







void GlobalVar_Init(void) {
    G_State_Settings_FLAG       = CLRBIT;                   // Not enter setting interface
    G_WaveScroll_FLAG           = CLRBIT;                   // Not enable waveform scroll

    G_UpdateVBAT_FLAG           = CLRBIT;                   // Reset battery update flag
    G_State_OptionChanged_FLAG  = CLRBIT;                   // Reset option changed flag

    /* Default function state */
    G_ADC_Running_FLAG          = SETBIT;                   // Set ADC running flag


    /* Default Option for [setting interface] and [main interface]*/
    G_OptionInSettings          = SettingSel_PlotMode;      // default: PlotMode
    G_OptionInChart             = ChartSel_ScaleH;          // default: ScaleH
    G_MeasureWaySel             = MeasureWay_AC;            // default: AC measure way
    G_PlotModeSel               = PlotMode_Vector;          // default: Vector

    /* Display setting */
    G_ClearDisplay_FLAG         = SETBIT;                   // Set clearing display flag
    G_ClearWave_FLAG            = CLRBIT;                   // Reset waveform erasing flag
    G_WaveUpdate_FLAG           = CLRBIT;                   // Reset waveform update flag

    /* Indicator */
    ADC_Sample_Ready_LED        = CLRBIT;                   // ADC sampling indicator off
    ifKeyPressed                = CLRBIT;                   // Initially, keys are not pressed

    BGV_RAM                     = Get_RAM_REFV();
}


void Switch_Init(void) {
    TCON_IT0 = CLRBIT;   // Enable INT0, rising edge and falling edge interrupt
    /* External interrupt 0 high priority, which can interrupt the key pressing, 
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

        if (SW_Button) {
            G_ADC_Running_FLAG = SETBIT;
        } 
        else {
            G_ADC_Running_FLAG = CLRBIT;
        }

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
            ADC_Sample_Ready_LED = CLRBIT;
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
    /* Disable I/O interrupt */
    P2INTE &= ~0x07;
    P4INTE &= ~0x06;

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
    /* Enable I/O interrupt */
    P2INTE |= 0x07;
    P4INTE |= 0x06;
    P_SW2 &= ~P_SW2_EAXFR;
}

