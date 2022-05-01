/**
 * @file base_stc8x.h
 * @author MContour (m-contour@qq.com)
 * @brief basic type definition for stc8x 
 * @version 0.1
 * @date 2022-04-27
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
#ifndef __BASE_STC8X__H__
#define __BASE_STC8X__H__

/** @defgroup STC
 * STCxx chip firmware library
 * @{
 */

/** @defgroup base_stc8x
 * STC8x basic definition for low level system
 * @{
 */

//! integer types
typedef  signed char  int8_t;	//  8 bits 
typedef  signed int  int16_t;	// 16 bits 
typedef  signed long int32_t;	// 32 bits 

//! unsigned integer types
typedef  unsigned char   uint8_t; //  8 bits 
typedef  unsigned int   uint16_t; // 16 bits 
typedef  unsigned long  uint32_t; // 32 bits 

//! volatile signed integer types
typedef volatile int8_t     vint8_t;
typedef volatile int16_t    vint16_t;
typedef volatile int32_t    vint32_t;

//! volatile unsigned integer types
typedef volatile uint8_t     vuint8_t;
typedef volatile uint16_t    vuint16_t;
typedef volatile uint32_t    vuint32_t;

/**
 * @brief IO definitions
 * @details access restrictions to peripheral registers
 */
#define  __I   volatile const /*!< defines 'read only' permissions      */
#define  __O   volatile       /*!< defines 'write only' permissions     */
#define  __IO  volatile       /*!< defines 'read / write' permissions   */


/** @defgroup Exported_macro
  * @{
  */
 
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/** @} */



/** @} */

/** @} */
#endif  //!__BASE_STC8X__H__