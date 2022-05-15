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
#include "stc8x_sysclk.h"


void RCC_DeInit(void) {
    /* Enable access to the internal extended RAM area */
    P_SW2 |= P_SW2_EAXFR;
    /* disable unused clock registers */
    SYSCLK->RCC_IRC32KCR = 0x7E;
    SYSCLK->RCC_XOSCCR   = 0x7E;
    /* enable 24MHz IRC */
    SYSCLK->RCC_IRC24MCR = 0xFE;
    /* wait for oscillator to be stable */
    while (!(SYSCLK->RCC_IRC24MCR & IRC24MCR_IRC24MST)) {
        ;
    }
    /* SYSCLK no division */
    SYSCLK->RCC_CLKDIV = 0x00;
    /* clear CKSEL */
    SYSCLK->RCC_CKSEL &= ~(0xFF);
    #if (LIB_MCU_MODULE == STC8Ax)
        IRTRIM = T24M_RAM;
        /* MCLK no output */
        SYSCLK->RCC_CKSEL |= CKSEL_MCLKODIV_DIV0;
    #elif (LIB_MCU_MODULE == STC8Hx)
        SYSCLK->RCC_MCLKOCR = 0x00;
        IRCBAND = IRCBAND_SEL_27MHz;
        IRTRIM  = T24M_CHIPID;
        VRTRIM  = VRT27M_CHIPID;

    #endif
    /* select 24MHz IRC */
    SYSCLK->RCC_CKSEL |= CKSEL_MCKSEL_IRC24MCR;
}


#if  (LIB_MCU_MODULE == STC8Ax) 

    void STC8Ax_RCC_UserIRC(uint8_t _irtrim, uint8_t _clkdiv, uint8_t _mclk_div_val, uint8_t _mclk_output_pin) {
        /* enable external special registers */
        P_SW2 |= P_SW2_EAXFR;
        /* set value for irtrim */
        IRTRIM = _irtrim;
        /* disable unused clock registers */
        SYSCLK->RCC_IRC32KCR = 0x7E;
        SYSCLK->RCC_XOSCCR   = 0x7E;
        /* enable 24MHz IRC */
        SYSCLK->RCC_IRC24MCR = 0xFE;
        /* wait for oscillator to be stable */
        while (!(SYSCLK->RCC_IRC24MCR & IRC24MCR_IRC24MST)) {
            ;
        }
        /* clear CKSEL */
        SYSCLK->RCC_CKSEL &= ~(0xFF);

        /* no clock division */
        SYSCLK->RCC_CLKDIV = _clkdiv;
        /* MCLK division value */
        SYSCLK->RCC_CKSEL |= _mclk_div_val;
        /* MCLK output pin */
        SYSCLK->RCC_CKSEL  = _mclk_output_pin;
        /* select 24MHz IRC */
        SYSCLK->RCC_CKSEL |= CKSEL_MCKSEL_IRC24MCR;
        /* disable external special registers */
        P_SW2 &= ~P_SW2_EAXFR;
    }

#endif


#if  (LIB_MCU_MODULE == STC8Hx) 

    void STC8Hx_RCC_UserIRC(uint8_t _irtrim, uint8_t _vrtrim, uint8_t _ircband,
                             uint8_t _clkdiv, uint8_t _mclk_div_val, uint8_t _mclk_output_pin) {
        /* enable external special registers */
        P_SW2 |= P_SW2_EAXFR;
        /* set value for IRTRIM */
        IRTRIM  = _irtrim;
        /* set value for VRTRIM */
        VRTRIM  = _vrtrim;
        /* select IRC band */
        IRCBAND = _ircband;
        /* disable unused clock registers */
        SYSCLK->RCC_IRC32KCR = 0x7E;
        SYSCLK->RCC_XOSCCR   = 0x7E;
        /* enable 24MHz IRC */
        SYSCLK->RCC_IRC24MCR = 0xFE;

        /* wait for oscillator to be stable */
        while (!(SYSCLK->RCC_IRC24MCR & IRC24MCR_IRC24MST)) {
            ;
        }
        /* clear CKSEL */
        SYSCLK->RCC_CKSEL   &= ~(0xFF);

        /* no clock division */
        SYSCLK->RCC_CLKDIV   = _clkdiv;
        /* MCLK division value */
        SYSCLK->RCC_MCLKOCR |= _mclk_div_val;
        /* clear previous definition of MCLK output pin */
        SYSCLK->RCC_MCLKOCR &= ~MCLKOCR_MCLKO_S;
        SYSCLK->RCC_MCLKOCR |= _mclk_output_pin;
        /* select 24MHz IRC */
        SYSCLK->RCC_CKSEL   |= CKSEL_MCKSEL_IRC24MCR;
        /* disable external special registers */
        P_SW2 &= ~P_SW2_EAXFR;
    }

#endif
