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
#include "uart.h"
#include "stc8x_gpio.h"
#include "stc8x_delay.h"
#include "STRING.H"

int main (void) {
    uint8_t i = 12;
    uint8_t str[256];

    GPIO_DeInit();
    UART1_Init();

    EA = SETBIT;

    sprintf((char*)str, "STC8A8K64D4 UART1 BMM Test Programme! %bd \r\n", i);
    while (1) {
        UARTx_Send_String(str, strlen((char*)str));
        delay_nms(500);
    }

    return 0;
}
