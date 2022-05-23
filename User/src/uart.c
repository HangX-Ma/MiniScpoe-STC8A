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
#include "STRING.H"
#include "global_var.h"

bit UART1_Tx_Busy;
// UARTx_struct xdata UART1_Tx;
UARTx_struct xdata UART1_Rx;

#if (UARTx_MODE == UARTx_DMA_MODE)
DMABuf_TypeDef CurrFreeBuf;
uint8_t xdata UART1_Tx_DMABuffer[UART1_DMA_BUF_LENGTH];
uint8_t xdata UART1_Rx_DMABuffer[2][UART1_DMA_BUF_LENGTH];
#endif

stateFIFO FIFO_Init(UARTx_struct *UARTx_FIFO) {
    memset(UARTx_FIFO->FIFOBuf, 0, FIFO_SIZE);
    UARTx_FIFO->frontPtr = 0;
    UARTx_FIFO->rearPtr = 0;
    
    return FIFO_succ;
}


stateFIFO FIFO_Out(UARTx_struct *UARTx_FIFO, uint8_t* _Buf) {
    if (UARTx_FIFO->frontPtr == UARTx_FIFO->rearPtr) {
        return FIFO_empty;
    } // if frontPtr touch the rearPtr, the FIFO queue
    else {
        *_Buf = UARTx_FIFO->FIFOBuf[UARTx_FIFO->frontPtr];
        UARTx_FIFO->frontPtr = (UARTx_FIFO->frontPtr+1) % FIFO_SIZE;
    }
    return FIFO_succ;
}

stateFIFO FIFO_In(UARTx_struct *UARTx_FIFO, const uint8_t _dat) {
    if ((UARTx_FIFO->rearPtr+1) % FIFO_SIZE == UARTx_FIFO->frontPtr) {
        return FIFO_full;
    } // Queue full
    else {
        UARTx_FIFO->FIFOBuf[UARTx_FIFO->rearPtr] = _dat;
        UARTx_FIFO->rearPtr = (UARTx_FIFO->rearPtr+1) % FIFO_SIZE;
    }

    return FIFO_succ;
}

void UART1_Init(void) {
    /* TM2 16 bits automatically reloading mode */
    AUXR  |= AUXR_S1ST2;    // Select TM2 as the bund rate generator
    AUXR  &= ~AUXR_T2R;     // Stop TM2
    AUXR  &= ~AUXR_T2CT;    // Select TM2 as Timer
    AUXR  |= AUXR_T2x12;    // 1T Mode, No frequency division
    T2L    = 0x8F;          // Reload Value Low Byte
    T2H    = 0xFD;          // Reload Value High Byte
    IE2   &= ~IE2_ET2;      // Forbidden TM2 interrupt
    AUXR  |= AUXR_T2R;      // Start TM2
    S1CON  = 0x50;          // Mode1, variable bund rate, 8 bits data
    ES     = SETBIT;        // Enable UART1 interrupt
    S1REN  = SETBIT;        // Enable UART1 receiver
    // PS     = SETBIT;        // Interrupt Level 1
    P_SW1  = (~P_SW1_S1_S) | P_SW1_S1_S_GROUP1; // P3.0 RxD, P3.1 TxD
    UART1_Tx_Busy = CLRBIT;
    FIFO_Init(&UART1_Rx);
    // FIFO_Init(&UART1_Tx);
}

void UARTx_Send(int8_t chr) {
    S1BUF = chr;                    // Add data to S1BUF
    UART1_Tx_Busy  = SETBIT;
    while (UART1_Tx_Busy) {
        ;
    } // wait until the serial port has sent the data in S1BUF
}

void UARTx_Send_String(uint8_t *str) {
    while (*str) {
        UARTx_Send(*str++);
    }
}

/**
 * @brief MCU UART1 interrupt service 
 */
void UART1_ISR_Handler(void) interrupt(UART1_VECTOR) using(1) {
    if(S1TI) {
        S1TI = CLRBIT; // prepare for next interrupt
        UART1_Tx_Busy = CLRBIT; // data has been sent
    }

    if (S1RI) {
        S1RI = CLRBIT; // prepare for next interrupt
        FIFO_In(&UART1_Rx, S1BUF); // store data
    }


}

#if (UARTx_MODE == UARTx_DMA_MODE)
/**
 * @brief UART1 BMM
 * @note Because the interrupt vector is greater than 31, 
 * you can not compile directly in Keil and must borrow 
 * the interrupt entry address No.13.
 */
void UART1_BMM_Interrupt(void) interrupt(13) {
    if (UR1T_BMM->STA & BMM_UR1T_STA_UR1TIF) {
        UR1T_BMM->STA &= ~BMM_UR1T_STA_UR1TIF; // Clear DMA interrupt flag
        UR1T_BMM->CR   = 0x00;
        memset(UART1_Tx_DMABuffer, (uint8_t)0, sizeof(UART1_Tx_DMABuffer)/sizeof(uint8_t));
    } // Sending complete

    if (UR1T_BMM->STA & BMM_UR1T_STA_TXOVW) {
        UR1T_BMM->STA &= ~BMM_UR1T_STA_TXOVW;
    } // Data overwrite. Sending process continues.

    if (UR1R_BMM->STA & BMM_UR1R_STA_UR1RIF) {
        UR1R_BMM->STA &= ~BMM_UR1R_STA_UR1RIF;

        P_SW2 |= P_SW2_EAXFR;
        if (CurrFreeBuf == Sel_Rx_DMABuf1) {
            memset(UART1_Rx_DMABuffer[CurrFreeBuf], (uint8_t)0, sizeof(UART1_Rx_DMABuffer[CurrFreeBuf])/sizeof(uint8_t));
            UR1R_BMM->TXA = (uint16_t)UART1_Rx_DMABuffer[Sel_Rx_DMABuf1];
            CurrFreeBuf = Sel_Rx_DMABuf2;
        }
        else if  (CurrFreeBuf == Sel_Rx_DMABuf2) {
            memset(UART1_Rx_DMABuffer[CurrFreeBuf], (uint8_t)0, sizeof(UART1_Rx_DMABuffer[CurrFreeBuf])/sizeof(uint8_t));
            UR1R_BMM->TXA = (uint16_t)UART1_Rx_DMABuffer[Sel_Rx_DMABuf2];
            CurrFreeBuf = Sel_Rx_DMABuf1;
        }
        P_SW2 &= ~P_SW2_EAXFR;
    } // Receiving complete

    if (UR1R_BMM->STA & BMM_UR1R_STA_RXLOSS) {
        UR1R_BMM->STA &= ~BMM_UR1R_STA_RXLOSS;
    } // Data discarded. Receiving process continues.
}

void UART1_BMM_Config(void) {
    P_SW2 |= P_SW2_EAXFR;
    UR1T_BMM->CFG |= BMM_UR1T_CFG_UR1TIE;                           // Enable BMM interrupt
    UR1T_BMM->CR  &= ~(BMM_UR1T_CR_ENUR1T | BMM_UR1T_CR_TRIG);      // Disable UR1T_BMM function and UR1T_BMM 
    UR1T_BMM->STA  = 0x00;                                          // Clear STA previous value
    UR1T_BMM->AMT  = (uint8_t)(UART1_DMA_BUF_LENGTH - 1);           // Set total transferred bytes: n+1
    UR1T_BMM->TXA  = (uint16_t)UART1_Tx_DMABuffer;                  // Get buffer Address

    UR1R_BMM->CFG |= BMM_UR1R_CFG_UR1RIE;                           // Enable BMM interrupt
    UR1R_BMM->CR  |= (BMM_UR1R_CR_ENUR1R | BMM_UR1R_CR_TRIG | BMM_UR1R_CR_CLRFIFO); // Enable UR1R_BMM function, UR1R_BMM and CLRFIFO 
    UR1R_BMM->STA  = 0x00;                                          // Clear STA previous value
    UR1R_BMM->AMT  = (uint8_t)(UART1_DMA_BUF_LENGTH - 1);           // Set total transferred bytes: n+1
    UR1R_BMM->TXA  = (uint16_t)UART1_Rx_DMABuffer[Sel_Rx_DMABuf1];  // Get buffer Address
    /* Select Rx DMA Buffer1 first, so current Free Buffer is Buffer2 */
    CurrFreeBuf    = Sel_Rx_DMABuf2;
    P_SW2 &= ~P_SW2_EAXFR;
}

void UARTx_DMA_Send_String(const uint8_t *str) {
    uint8_t UART1_DMA_Cnt = 0;
    /* disable DMA interrupt while preparing data */
    UR1T_BMM->CFG &= BMM_UR1T_CFG_UR1TIE;
    UR1R_BMM->CFG &= BMM_UR1R_CFG_UR1RIE;

    /* data preparing */
    while (*str != '\0') {
        UART1_Tx_DMABuffer[UART1_DMA_Cnt++] = *str;
        if (UART1_DMA_Cnt >= UART1_DMA_BUF_LENGTH) {
            UART1_DMA_Cnt = 0;
        } // overflow protection
    }

    /* enable DMA interrupt */
    UR1T_BMM->CFG &= BMM_UR1T_CFG_UR1TIE;
    UR1R_BMM->CFG &= BMM_UR1R_CFG_UR1RIE;
    // start DMA
    UR1T_BMM->CR  |= BMM_UR1T_CR_ENUR1T | BMM_UR1T_CR_TRIG;
}

#endif

