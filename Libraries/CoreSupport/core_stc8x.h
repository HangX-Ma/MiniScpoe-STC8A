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
#ifndef __CORE_STC8X__H__
#define __CORE_STC8X__H__

/** @addtogroup STC
 * @{
 */

/** @defgroup core_stc8x
 * STC8x core definition for low level system
 * @{
 */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/**
 * @brief Peripheral I/O group selection type
 */
typedef enum
{
	SW_G1 = (uint8_t)0x00,  //!< switch to the group 1
	SW_G2 = (uint8_t)0x01,  //!< switch to the group 2
	SW_G3 = (uint8_t)0x02,  //!< switch to the group 3
	SW_G4 = (uint8_t)0x03   //!< switch to the group 4
} GPIO_SWG;


/**
 * @brief Interrupt management priority enum.
 */
typedef enum
{
  ISR_PR0 = (uint8_t)0x00,  //!< Lowest  priority
  ISR_PR1 = (uint8_t)0x01,  //!< Lower   priority
  ISR_PR2 = (uint8_t)0x02,  //!< Higher  priority
  ISR_PR3 = (uint8_t)0x03   //!< Highest priority
} ISR_PRx;


// low voltage detection interrupt
#define LVD_ISRQ_Handler(void)  LVD_ISR(void) interrupt  6

// UART interrupt
#define UART1_ISRQ_Handler(void)  UART1_ISR(void) interrupt  4
#define UART2_ISRQ_Handler(void)  UART2_ISR(void) interrupt  8
#define UART3_ISRQ_Handler(void)  UART3_ISR(void) interrupt  17
#define UART4_ISRQ_Handler(void)  UART4_ISR(void) interrupt  18

// external interrupt 
#define EXTI0_ISRQ_Handler(void)  EXTI0_ISR(void) interrupt  0
#define EXTI1_ISRQ_Handler(void)  EXTI1_ISR(void) interrupt  2
#define EXTI2_ISRQ_Handler(void)  EXTI2_ISR(void) interrupt  10
#define EXTI3_ISRQ_Handler(void)  EXTI3_ISR(void) interrupt  11
#define EXTI4_ISRQ_Handler(void)  EXTI4_ISR(void) interrupt  16

// timer interrupt
#define TIMER0_ISRQ_Handler(void)  TIMER0_ISR(void) interrupt  1
#define TIMER1_ISRQ_Handler(void)  TIMER1_ISR(void) interrupt  3
#define TIMER2_ISRQ_Handler(void)  TIMER2_ISR(void) interrupt  12
#define TIMER3_ISRQ_Handler(void)  TIMER3_ISR(void) interrupt  19
#define TIMER4_ISRQ_Handler(void)  TIMER4_ISR(void) interrupt  20

// ADC interrupt
#define ADC_ISRQ_Handler(void)  ADC_ISR(void) interrupt  5

// PCA interrupt
#define PCA_ISRQ_Handler(void)  PCA_ISR(void) interrupt  7

// PWM interrupt
#define PWM_ISRQ_Handler(void)  PWM_ISR(void) interrupt  22
#define PWMFD_ISRQ_Handler(void)  PWMFD_ISR(void) interrupt  23

// comparator interrupt
#define COMP_ISRQ_Handler(void)  COMP_ISR(void) interrupt  21

// SPI interrupt
#define SPI_ISRQ_Handler(void)  SPI_ISR(void) interrupt  9

// I2C interrupt
#define I2C_ISRQ_Handler(void)  I2C_ISR(void) interrupt  24


/** @} */


#endif  //!__CORE_STC8X__H__