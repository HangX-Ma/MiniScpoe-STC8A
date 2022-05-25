/**
 * @file fifo.h
 * @author MContour (m-contour@qq.com)
 * @brief FIFO functions and critical definitions
 * @version 0.1
 * @date 2022-05-24
 * 
 * @copyright Copyright (c) 2022
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

#ifndef __FIFO__H__
#define __FIFO__H__

//* ------------      head files      ------------
#include "base_stc8x.h"

//* ------------ developer definitions ------------
#define FIFO_SIZE 16

//* ------------   GLOBAL variables   ------------
/**
 * @brief Ring FIFO basic structure
 * @note \c volatile is used to declare \b frontPtr and \b rearPtr, 
 * because both of them will be accessed from application and interrupt context.
 */
typedef struct {
    volatile uint8_t frontPtr;
    volatile uint8_t rearPtr;
    uint8_t FIFOBuf[FIFO_SIZE]; // actual used size is FIFO_SIZE-1
} FIFO_struct;

typedef enum {
    FIFO_succ = 0x00,
    FIFO_full,
    FIFO_empty,
} stateFIFO;

typedef enum {
    FIFO_Threshold_2  = 2,
    FIFO_Threshold_4  = 4,
    FIFO_Threshold_8  = 8,
    FIFO_Threshold_12 = 12,
    FIFO_Threshold_14 = 14,
    FIFO_Threshold_15 = 15,
} FIFO_Rx_Tx_Threshold_TypeDef;

extern FIFO_Rx_Tx_Threshold_TypeDef FIFO_Threshold;
//* ------------     functions     ------------

/**
 * @brief Initialize FIFO struct
 * 
 * @param UARTx_FIFO FIFO_struct
 * @return stateFIFO 
 */
stateFIFO FIFO_Init(FIFO_struct *_FIFOStruct);

/**
 * @brief FIFO read buffer
 * @details FIFO queue read FIFO_struct buffer data to address content where _Buf points to
 * @param UARTx_FIFO FIFO_struct
 * @param _Buf ouput data position
 * @return stateFIFO 
 */
stateFIFO FIFO_Out(FIFO_struct *_FIFOStruct, uint8_t* _Buf);

/**
 * @brief FIFO write buffer
 * @details FIFO queue store input data to FIFO_struct
 * @param UARTx_FIFO FIFO_struct
 * @param _dat input data
 * @return stateFIFO 
 */
stateFIFO FIFO_In(FIFO_struct *_FIFOStruct, const uint8_t _dat);

/**
 * @brief Get the used buffer space of FIFO
 * 
 * @param _FIFOStruct FIFO_struct
 * @return uint8_t Used space info
 */
uint8_t Get_FIFO_UsedSpace(FIFO_struct *_FIFOStruct);


#endif  //!__FIFO__H__