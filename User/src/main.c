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
    UARTx_Send(c);

    return c;
}

int main (void) {
    uint16_t ADC_REFV;

    GPIO_DeInit();
    UART1_Init();
    Wave_ADC_Init(Scale_500ms);

    Px_M1(5) &= ~(0x01); Px_M1(5) &= ~(0x01); // P5.0 port weak pull
    P_SW2 |= P_SW2_EAXFR;
    Px_PU(5) |= 0x01;
    P_SW2 &= ~P_SW2_EAXFR;

    EA = SETBIT;

    ADC_REFV = Get_RAM_REFV();

    while (1) {
        // printf("STC8A8K64D4 UART1 BMM Test Programme! %bd \r\n", i);
        P50 = ~P50;
        printf("STC8A8K64D4 ADC Bandgap value: %d\r\n", ADC_REFV);

        delay_nms(200);

    }

    return 0;
}
