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

bit interrupt_num;
bit Encoder_Flag;
uint16_t count;

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
    // uint16_t ADC_Sampled_REFV;
    // uint16_t *ADC_REFV;
    // uint16_t Vin;
    // uint16_t VSampled;
    interrupt_num = 0;
    count = 0;
    /* Set indicator port P1.7 as push-pull output */
    P0M1 &= ~0x80;
    P0M0 |= 0x80;

    /* Set P2.1, P2.2, P5.4 as high-impedance */
    P2M1 |= 0x06;
    P2M0 &= ~0x06;
    P5M1 |= 0x10;
    P5M0 &= ~0x10;
    ADC_CHx_SEL(ADC_CONTR_ADC_CHS_VAL12);
    ADC_CHx_SEL(ADC_CONTR_ADC_CHS_VAL14);
    UART1_Init();
    delay_nus(20);
    TM0_Init();
    ET0 = SETBIT; // Enable TM0 interrupt
    TM1_Init();
    ET1 = SETBIT; // Enable TM1 interrupt
    Encoder_Init();

    EA = SETBIT;

    while (1) {
        if (G_ADC_Interrupt_FLAG) {
            if (G_UpdateVBAT_FLAG) {
                VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL12, VBAT_RATIO);
            }
            printf("Count: %d VBAT: %d\r\n", count, VBAT);
            G_ADC_Interrupt_FLAG = CLRBIT;
        }
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
    BGV = Get_RAM_REFV();
}

void Encoder_Init(void) {
    /* Interrupt for rotating of Encoder */
    TCON_IT0    = CLRBIT; // External interrupt 0 trigger way, rising edge or falling edge.
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
    TMOD     &= ~(TMOD_T0_CT | TMOD_T0_SEL | TMOD_T0_GATE); // Set timer work mode
    TL0      = 0xB0;         // Initial timer value LOW BYTE
    TH0      = 0x3C;         // Initial timer value HIGH BYTE
    TCON_TF0 = CLRBIT;       // Clear TF0 interrupt flag
    TCON_TR0 = SETBIT;       // Timer0 start running
} // 25ms@24.000MHz

void TM1_Init(void) {
    AUXR    &= ~AUXR_T1x12;  //Timer clock is 12T mode
    TMOD    &= ~(TMOD_T1_CT | TMOD_T1_SEL | TMOD_T1_GATE);  //Set timer work mode
    TL1      = 0xC0;         // Initial timer value LOW BYTE
    TH1      = 0x63;         // Initial timer value HIGH BYTE
    TCON_TF1 = CLRBIT;       // Clear TF1 flag
    TCON_TR1 = SETBIT;       // Timer1 start run
} //20ms@24.000MHz

void TM4_Init(void) {
    T4T3M &= ~(T4T3M_T4x12 | T4T3M_T4_CT);  //Timer clock is 12T mode
    T4L = 0x90;                             //Initial timer value LOW BYTE
    T4H = 0xE8;                             //Initial timer value HIGH BYTE
    T4T3M |= T4T3M_T4R;                     //Timer4 start run
} //3ms@24.000MHz

/**
 * @brief Interrupt for Encoder Rotated 
 */
void INT0_ISR_Handler(void) interrupt(EXTI0_VECTOR) using(2) {
    /* Whether the Encoder is pressed or not */
    if (!EC11_KEY) {
        G_EC11PressWithRotate_FLAG = SETBIT;
    }
    else {
        G_EC11PressWithRotate_FLAG = CLRBIT;
    }

    if (interrupt_num == 0 && EC11_A == 0) {
        Encoder_Flag = CLRBIT;
        if (EC11_B) {
            Encoder_Flag = SETBIT;
        }
        interrupt_num = 1;
    } // First interrupt, EC11_A is falling edge

    if (interrupt_num == 1 && EC11_A == 1) {
        if (EC11_B == 0 && Encoder_Flag == 1) {
            count = 1;
        }

        if (EC11_B == 1 && Encoder_Flag == 0) {
            count = 2;
        }
        interrupt_num = 0;
        
        G_ADC_Interrupt_FLAG = SETBIT;
        G_DisplayUpdate_FLAG = SETBIT;
    } // Second interrupt, EC11_A is rising edge
}

/**
 * @brief TM0 interrupt for updating battery voltage infomation on screen.
 */
void TM0_ISR_Handler(void) interrupt(TM0_VECTOR) using(3){
    static uint8_t n;

    if (++n >= VBAT_UPDATE_FREQ) {
        n = 0;
        G_UpdateVBAT_FLAG    = SETBIT; // Update VBAT info
    }
    // printf("TM0 ISR\r\n");
}


void TM1_ISR_Handler(void) interrupt(TM1_VECTOR) using(3){
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
    read3 = P5 ^ 0xFF;
    Trg3  = read3 & (read3 ^ cont3); 
    cont3 = read3;

    if (Trg1) {
        P17 = ~P17;
    }

    if (Trg2) {
        P17 = ~P17;
    }

    if (Trg3) {
        P17 = ~P17;
    }
}