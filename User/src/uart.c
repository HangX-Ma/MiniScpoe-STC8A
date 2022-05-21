/**
 * @file uart.c
 * @author MContour (m-contour@qq.com)
 * @brief UART functions realization
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

#include "uart.h"
#include "global_var.h"

void UART1_Init(void) {
    /* TM2 16 bits automatically reloading mode */
    AUXR  &= ~AUXR_T2R;     // Stop TM2
    S1CON  = 0x50;          // Mode1, variable bund rate, 8 bits data
    AUXR  |= AUXR_S1ST2;    // Select TM2 as the bund rate generator
    AUXR  |= AUXR_T2x12;    // 1T Mode, No frequency division
    T2L    = 0x8F;          // Reload Value Low Byte
    T2H    = 0xFD;          // Reload Value High Byte
    IE2   &= ~IE2_ET2;      // Forbidden TM2 interrupt
    AUXR  |= AUXR_T2R;      // Start TM2
    S1TI   = CLRBIT;        // Clear Sender interrupt flag
    S1RI   = CLRBIT;        // Clear Receiver interrupt flag
    S1REN  = SETBIT;        // Enable UART1 receiver
    PS     = SETBIT;        // Interrupt Level 1
    ES     = SETBIT;        // Enable UART1 interrupt
    P_SW1  = (~P_SW1_S1_S) | P_SW1_S1_S_GROUP1; // P3.0 RxD, P3.1 TxD
}

void UARTx_Send(int8_t chr) {
    while (!S1TI) {
        ;
    } // wait until the serial port has sent the data in S1BUF
    S1TI = CLRBIT;
    S1BUF = chr;                    // Add data to S1BUF
}

void UARTx_Send_String(uint8_t *str) {
    while (*str) {
        UARTx_Send(*str++);
    }
}

/**
 * @brief MCU UART1 interrupt service 
 */
void UART1_ISR_Handler(void) interrupt(TM2_VECTOR) using(1) {
    ;
}

