/**
 * @file stc8x_delay.c
 * @author MContour (m-contour@qq.com)
 * @brief STC8x delay basic function realization 
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
#include "stc8x_delay.h"

void delay_nms(uint16_t nms) {
    uint16_t i;
    for(; nms > 0; nms--) {
        i = LIB_CLK_FREQ / LIB_DELAY_COUNT;
        while(--i);
    }
}

void delay_nus(uint16_t nus) {
    uint16_t i;
    for(; nus > 0; nus--) {
        i = LIB_CLK_FREQ / LIB_DELAY_COUNT / 1000;
        while(--i);
    }
}