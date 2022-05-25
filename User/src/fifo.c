/**
 * @file fifo.c
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
#include "fifo.h"
#include "STRING.H"

FIFO_Rx_Tx_Threshold_TypeDef FIFO_Threshold;

stateFIFO FIFO_Init(FIFO_struct *_FIFOStruct) {
    memset(_FIFOStruct->FIFOBuf, 0, FIFO_SIZE);
    _FIFOStruct->frontPtr = 0;
    _FIFOStruct->rearPtr = 0;
    
    return FIFO_succ;
}

stateFIFO FIFO_Out(FIFO_struct *_FIFOStruct, uint8_t* _RBuf) {
    if (_FIFOStruct->frontPtr == _FIFOStruct->rearPtr) {
        return FIFO_empty;
    } // if frontPtr touch the rearPtr, the FIFO queue is EMPTY
    else {
        *_RBuf = _FIFOStruct->FIFOBuf[_FIFOStruct->frontPtr];
        _FIFOStruct->frontPtr = (_FIFOStruct->frontPtr+1) % FIFO_SIZE; // avoid index out of range
    }

    return FIFO_succ;
}

stateFIFO FIFO_In(FIFO_struct *_FIFOStruct, const uint8_t _dat) {
    if ((_FIFOStruct->rearPtr+1) % FIFO_SIZE == _FIFOStruct->frontPtr) {
        return FIFO_full;
    } // Queue FULL
    else {
        _FIFOStruct->FIFOBuf[_FIFOStruct->rearPtr] = _dat;
        _FIFOStruct->rearPtr = (_FIFOStruct->rearPtr+1) % FIFO_SIZE;
    }

    return FIFO_succ;
}

uint8_t Get_FIFO_UsedSpace(FIFO_struct *_FIFOStruct) {
    return (_FIFOStruct->rearPtr - _FIFOStruct->frontPtr + FIFO_SIZE) % FIFO_SIZE;
}