/**
 * @file stc8x_eeprom.h
 * @author MContour (m-contour@qq.com)
 * @brief STC8x EEPROM basic function definitions 
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

#ifndef __STC8X_EEPROM__H__
#define __STC8X_EEPROM__H__

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif


/** @addtogroup STC
 * @{
 */

/** @addtogroup Peripheral
 * @{
 */


void EEPROM_PageErase(uint16_t addr);

void EEPROM_Read(uint8_t *str, char num);

void EEPROM_Write(uint8_t *str, char num);




/** @} */

/** @} */

#endif  //!__STC8X_EEPROM__H__