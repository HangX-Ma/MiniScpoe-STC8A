/**
 * @file uart.h
 * @author MContour (m-contour@qq.com)
 * @brief UART basic function definitions
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

#ifndef __UART__H__
#define __UART__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif

//* ------------ developer definitions ------------

#define UARTx_INTERRUPT_MODE (0)
#define UARTx_DMA_MODE       (1)

// #define  UARTx_MODE UARTx_DMA_MODE

#ifndef UARTx_MODE
    #define UARTx_MODE UARTx_INTERRUPT_MODE
#endif

#define FIFO_SIZE 128
typedef struct {
    uint8_t frontPtr;
    uint8_t rearPtr;
    uint8_t FIFOBuf[FIFO_SIZE];
} UARTx_struct;

typedef enum {
    FIFO_succ = 0x00,
    FIFO_full,
    FIFO_empty,
} stateFIFO;

#if (UARTx_MODE == UARTx_DMA_MODE)
#define UART1_DMA_BUF_LENGTH 256

typedef enum {
    Sel_Rx_DMABuf1 = 0x00,
    Sel_Rx_DMABuf2 = 0x01,
} DMABuf_TypeDef;

//* ------------   GLOBAL variables   ------------
extern uint8_t xdata UART1_Tx_DMABuffer[UART1_DMA_BUF_LENGTH];
extern uint8_t xdata UART1_Rx_DMABuffer[2][UART1_DMA_BUF_LENGTH];

extern DMABuf_TypeDef CurrFreeBuf;
#endif
//* ------------     functions     ------------

/**
 * @brief UART1, Mode1, Timer2, 1T, 9600bps@24.000MHz,Reload Value: 65536-(SYSclk)/(4*bund_rate)=0xFD8FH
 */
void UART1_Init(void);

void UARTx_Send(int8_t chr);

void UARTx_Send_String(uint8_t *str);

#if (UARTx_MODE == UARTx_DMA_MODE)
/**
 * @brief UART1 DMA configuration
 */
void UART1_BMM_Config(void);

void UARTx_DMA_Send_String(const uint8_t *str);

#endif

#endif  //!__UART__H__