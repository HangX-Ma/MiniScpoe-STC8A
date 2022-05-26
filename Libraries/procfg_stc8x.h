/**
 * @file procfg_stc8x.h
 * @author MContour (m-contour@qq.com)
 * @brief project configuration file
 * @version 0.1
 * @date 2022-05-13
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

#ifndef __PROCFG_STC8X__H__
#define __PROCFG_STC8X__H__

//* MCU MODULE Definitions
#define STC8Ax          (0)
#define STC8Hx          (1)

// #define LIB_MCU_MODULE  STC8Ax
#define LIB_MCU_MODULE  STC8Hx

#ifndef LIB_MCU_MODULE
    #define LIB_MCU_MODULE STC8Ax
#endif

//* Clock frequency Definition beforehand
#define LIB_CLK_FREQ    ((uint32_t)24000000)    // clock frequency acquirement
#define LIB_DELAY_COUNT ((uint16_t)9980)       // accurate delay base

#endif  //!__PROCFG_STC8X__H__
