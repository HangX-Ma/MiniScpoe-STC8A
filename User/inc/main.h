/**
 * @file main.h
 * @author MContour (m-contour@qq.com)
 * @brief main program definition
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

#ifndef __MAIN__H__
#define __MAIN__H__

#include "uart.h"
#include "adc.h"
#include "chart.h"
#include "ssd1306.h"
#include "setting.h"
#include "stc8x_gpio.h"
#include "stc8x_delay.h"
#include "global_var.h"

void Encoder_Init(void);
void TM0_Init(void);
void TM1_Init(void);
void GlobalVar_Init(void);

void runWhenInSettings(void);
void runWhenADCInterrupt(void);
void runWhenADCComplete(void);

#endif  //!__MAIN__H__
