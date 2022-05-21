/**
 * @file core_stc8x.h
 * @author MContour (m-contour@qq.com)
 * @brief STC8x Core Peripheral Access Layer Header File.
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
#ifndef __CORE_STC8X__H__
#define __CORE_STC8X__H__

#include "procfg_stc8x.h"

/** @addtogroup STC
 * @{
 */

/** @defgroup core_stc8x
 * STC8x core definition for low level system
 * @{
 */

#define SETBIT 1
#define CLRBIT 0

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/**
 * @brief Peripheral I/O group selection type
 */
typedef enum
{
	SW_G1 = 0x00,  //!< switch to the group 1
	SW_G2 = 0x01,  //!< switch to the group 2
	SW_G3 = 0x02,  //!< switch to the group 3
	SW_G4 = 0x03   //!< switch to the group 4
} GPIO_SWG;


/**
 * @brief Interrupt management priority enum.
 */
typedef enum
{
  ISR_PR0 = 0x00,  //!< Lowest  priority
  ISR_PR1 = 0x01,  //!< Lower   priority
  ISR_PR2 = 0x02,  //!< Higher  priority
  ISR_PR3 = 0x03   //!< Highest priority
} ISR_PRx;

/** @defgroup NVIC
 * @{
 */

// low voltage detection interrupt
#define     EXTI0_VECTOR            0       //0003H
#define     TM0_VECTOR              1       //000BH
#define     EXTI1_VECTOR            2       //0013H
#define     TM1_VECTOR              3       //001BH
#define     UART1_VECTOR            4       //0023H
#define     ADC_VECTOR              5       //002BH
#define     LVD_VECTOR              6       //0033H
#define     PCA_VECTOR              7       //003BH
#define     UART2_VECTOR            8       //0043H
#define     SPI_VECTOR              9       //004BH
#define     EXTI2_VECTOR            10      //0053H
#define     EXTI3_VECTOR            11      //005BH
#define     TM2_VECTOR              12      //0063H
#define     EXTI4_VECTOR            16      //0083H
#define     UART3_VECTOR            17      //008BH
#define     UART4_VECTOR            18      //0093H
#define     TM3_VECTOR              19      //009BH
#define     TM4_VECTOR              20      //00A3H
#define     COMP_VECTOR             21      //00ABH
#define     PWM_VECTOR              22      //00B3H
#define     PWMFD_VECTOR            23      //00BBH
#define     I2C_VECTOR              24      //00C3H

#if (LIB_MCU_MODULE == STC8Hx)
#define     P0INT_VECTOR            37      //012BH
#define     P1INT_VECTOR            38      //0133H
#define     P2INT_VECTOR            39      //013BH
#define     P3INT_VECTOR            40      //0143H
#define     P4INT_VECTOR            41      //014BH
#define     P5INT_VECTOR            42      //0153H
#define     P6INT_VECTOR            43      //015BH
#define     P7INT_VECTOR            44      //0163H
#define     M2MDMA_VECTOR           47      //017BH
#define     ADCDMA_VECTOR           48      //0183H
#define     SPIDMA_VECTOR           49      //018BH
#define     U1TXDMA_VECTOR          50      //0193H
#define     U1RXDMA_VECTOR          51      //019BH
#define     U2TXDMA_VECTOR          52      //01A3H
#define     U2RXDMA_VECTOR          53      //01ABH
#define     U3TXDMA_VECTOR          54      //01B3H
#define     U3RXDMA_VECTOR          55      //01BBH
#define     U4TXDMA_VECTOR          56      //01C3H
#define     U4RXDMA_VECTOR          57      //01CBH
#define     LCMDMA_VECTOR           58      //01D3H
#define     LCM_VECTOR              59      //01DBH

/** @} */

#endif

/** @} */


#endif  //!__CORE_STC8X__H__