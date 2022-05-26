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



#ifndef __VS_CODE_H__
#define __VS_CODE_H__

#ifdef VSCODE
    #define _nop_() (void*)0;
    #define interrupt(x)
    #define using(x)
    #include <stdbool.h>
    #include <stdint.h>
    typedef bool bit;
    class sfr{
    public:
        sfr(int){};
        sfr(){};
        ~sfr(){};
        bool operator ^ (uint16_t _data);
        bool operator ^ (int _data);
        sfr& operator=(const sfr& _other);
        sfr& operator=(const int _other);
        int operator ^= (int _data);
        int operator &= (int _data);
        int operator |= (int _data);
        int operator | (int _data);
        int operator & (int _data);
        operator int(){};
    };
    #define sbit bool
    #define idata
    #define xdata
    #define bdata
    #define data
    #define code
    #define reentrant
#else
    #define interrupt(x) interrupt x
    #define using(x) using x
#endif

#endif

//! integer types
#ifndef int8_t 
    #define int8_t signed char
#endif

#ifndef int16_t 
    #define int16_t signed int
#endif

#ifndef int32_t 
    #define int32_t signed long
#endif

//! unsigned integer types
#ifndef uint8_t 
    #define uint8_t unsigned char
#endif

#ifndef uint16_t 
    #define uint16_t unsigned int
#endif

#ifndef uint32_t 
    #define uint32_t unsigned long
#endif

//! volatile signed integer types
#ifndef vint8_t 
    #define vint8_t volatile int8_t
#endif

#ifndef vint16_t 
    #define vint16_t volatile int16_t
#endif

#ifndef vint32_t 
    #define vint32_t volatile int32_t
#endif

//! volatile unsigned integer types
#ifndef vuint8_t 
    #define vuint8_t volatile uint8_t
#endif

#ifndef vuint16_t 
    #define vuint16_t volatile uint16_t
#endif

#ifndef vuint32_t 
    #define vuint32_t volatile uint32_t
#endif


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