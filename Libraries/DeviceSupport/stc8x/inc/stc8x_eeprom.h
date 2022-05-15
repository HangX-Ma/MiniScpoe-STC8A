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

#ifndef __STC8X_EEPROM__H__
#define __STC8X_EEPROM__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

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

//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------

//* ------------     functions     ------------

#define EEPROM_START_ADDR   ((uint16_t)0x0000)  //! EEPROM starting address
#define EEPROM_PAGE_SIZE    ((uint16_t)0x0200)  //! EEPROM page size

/**
 * @brief Erase a Page 
 * 
 * @param[in] addr page heading address
 * @note A page size is 512 bytes and EEPROM erasure operation 
 * takes effect only in page. For instance, \em STC8A8K16S4A12 EEPROM
 * space size is 48K(0x0000H~0xBFFFH). 0x0234H/0x0200H/0x0300H/0x03FFH 
 * address setting always performs a specifc action that erase a page 
 * 0x0200H~0x03FFH. So \c addr value set to 
 * \code EEPROM_START_ADDR + n*EEPROM_PAGE_SIZE \endcode.
 * @warning Don't try to erase the page region that surpasses the maximum EEPROM
 * page region.
 */
void EEPROM_PageErase(uint16_t addr);

/**
 * @brief Read N bytes starting at specifc address from EEPROM
 * 
 * @param[in,out] str data container
 * @param[in] N \c N bytes
 * @param[in] addr starting address
 */
void EEPROM_Read_N_Bytes(uint8_t *str, uint8_t N, uint16_t addr);

/**
 * @brief Write N bytes starting at specifc address to EEPROM
 * 
 * @param[in] str data container
 * @param[in] N \c N bytes
 * @param[in] addr starting address
 * 
 * @return Returned value indicates writing state.
 */
ErrorStatus EEPROM_Write_N_Bytes(uint8_t *str, uint8_t N, uint16_t addr);




/** @} */

/** @} */

#endif  //!__STC8X_EEPROM__H__