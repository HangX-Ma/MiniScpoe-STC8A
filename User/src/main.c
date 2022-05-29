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
    if (c == '\n') {
        c = 0x0D;
    }
    UARTx_Send(c);

    return c;
}

int main (void) {
    uint16_t ADC_REFV;

    GPIO_DeInit();
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
        if (G_UpdateVBAT_FLAG) {
            G_UpdateVBAT_FLAG = CLRBIT; // Prepare for next VBAT info interrupt
            VBAT = Get_BATV(ADC_CONTR_ADC_CHS_VAL4, VBAT_RATIO);
        }

        GetWaveData();

        if (G_OptionInSettings) {
            // runWhenInSettrings();
        }

    }




    return 0;
}



void MainInit(void) {
    G_State_Settings_FLAG       = CLRBIT;                   // Not enter setting interface
    G_OptionInSettings          = SettingSel_PlotMode;      // PlotMode
    G_OptionInChart             = ChartSel_ScaleH;          // ScaleH
    G_WaveScroll_FLAG           = CLRBIT;                   // Not enable wave scroll
    G_State_OptionChanged_FLAG  = CLRBIT;                   // Reset option changed flag
    G_ADC_Running_FLAG          = SETBIT;                   // Set ADC running flag
    G_ADC_Interrupt_FLAG        = CLRBIT;                   // Reset ADC interrupt flag
    G_ADC_Complete_FLAG         = CLRBIT;                   // Reset ADC conversion complete flag
    G_WaveUpdate_FLAG           = CLRBIT;                   // Reset wave update flag
    G_ClearWave_FLAG            = CLRBIT;                   // Reset wave erasing flag
    G_ClearDisplay_FLAG         = SETBIT;                   // Set clearing display flag
    G_EC11PressWithRotate_FLAG  = CLRBIT;                   // Reset encoder rotation with movement flag
    G_UpdateVBAT_FLAG           = CLRBIT;                   // Reset battery update flag
    G_WaveFreq                  = 0;                        // Clear wave frequency
    G_TriggerPos                = 50;                       // Set initial trigger position
    G_TriggerPosOffset          = 0;                        // Set initial trigger offset
    G_TriggerFail_FLAG          = CLRBIT;                   // Reset trigger failure flag
    G_VMax                      = 0;                        // Clear maximum wave voltage
    G_VMin                      = 0;                        // Clear minimum wave voltage
    G_WaveAvgLengthSumNum       = 0;                        // Clear average wave length sum counts
    G_WaveAvgLengthSum          = 0;                        // Clear average wave length sum
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

    read1 = (P2 & 0x02) ^ 0x02;
    Trg1  = (read1 ^ cont1)  & read1; 
    cont1 = read1;
        
    read2 = (P2 & 0x04) ^ 0x04;
    Trg2  = (read2 ^ cont2) & read2; 
    cont2 = read2;

    read3 = P5 ^ 0xFF;
    Trg3  = (read3 ^ cont3) & read3; 
    cont3 = read3;

    if(Trg1) {
        /* Main Interface */
        if (!G_State_Settings_FLAG) {
            G_WaveScroll_FLAG           = ~G_WaveScroll_FLAG;
            G_State_OptionChanged_FLAG  = SETBIT;
            G_ADC_Interrupt_FLAG        = SETBIT;
            G_DisplayUpdate_FLAG        = SETBIT;
            G_ClearWave_FLAG            = CLRBIT;
        }

        Trg1 = 0;
    }

    if(Trg2) {
            G_State_Settings_FLAG = ~G_State_Settings_FLAG;
            if (G_State_Settings_FLAG) {
                G_DisplayUpdate_FLAG = 1;
                G_UpdateVBAT_FLAG = 1;
                TCON_TF0 = CLRBIT; //清零定时器0溢出标志
                TCON_TR0 = SETBIT; //定时器0开始计时，开始电池电压信息更新计时
                TCON_IE0 = CLRBIT; //清零外部中断0中断标志
                EX0      = SETBIT; //开启外部中断0（编码器旋转）
            } // Enter Settings
            else {
                TCON_TR0 = CLRBIT; //清零定时器0溢出标志
                TCON_TF0 = CLRBIT; //定时器0停止计时，停止电池电压信息更新计时
                G_WaveFreq = 0;
                G_TriggerFail_FLAG = CLRBIT;
                G_VMax = 0;
                G_VMin = 0;
                G_DisplayUpdate_FLAG = SETBIT;
                G_WaveUpdate_FLAG = SETBIT;
                G_ClearWave_FLAG = CLRBIT;
            } // Retrurn to main interface
             
            G_ADC_Interrupt_FLAG = SETBIT;
        Trg2 = 0;
    }

    if(Trg3) {
        SP   = 0;
        Trg3 = 0;
    }
}