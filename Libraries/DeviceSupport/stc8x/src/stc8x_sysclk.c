/**
 * @file stc8x_sysclk.c
 * @author MContour (m-contour@qq.com)
 * @brief STC8x system clock basic function realization 
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Apache 2.0 LICENSE
 * 
 *****************************************************************************
 * Copyright (c) [2022-04-27] [MContour m-contour@qq.com]
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
#include "stc8x_sysclk.h"

void RCC_DeInit(void) {
    /* Enable access to the internal extended RAM area */
    P_SW2 |= P_SW2_EAXFR;
    /* clear CKSEL */
    SYSCLK->RCC_CKSEL &= ~(0xFF);
    /* select 24MHz IRC */
    SYSCLK->RCC_CKSEL |= CKSEL_MCKSEL_IRC24MCR;
    /* MCLK no output */
    SYSCLK->RCC_CKSEL |= CKSEL_MCLKODIV_DIV0;
    /* SYSCLK no division */
    SYSCLK->RCC_CLKDIV = 0x00;
    /* disable unused clock registers */
    SYSCLK->RCC_IRC32KCR = 0x7E;
    SYSCLK->RCC_XOSCCR   = 0x7E;
    /* enable 24MHz IRC */
    SYSCLK->RCC_IRC24MCR = 0xFE;
    /* wait for oscillator to be stable */
    while (!(SYSCLK->RCC_IRC24MCR & IRC24MCR_IRC24MST)) {
        ;
    }
    /* set internal 24MHz IRC to 27MHz actually */
    IRCTRIM  = 200;
    /* internal clock IRC no trim */
    LIRCTRIM = LIRTRIM_BITS_FT000;
}
