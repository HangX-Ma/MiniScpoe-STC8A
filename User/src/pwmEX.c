/**
 * @file pwmEX.c
 * @author MContour (m-contour@qq.com)
 * @brief Enhanced PWM module
 * @version 0.1
 * @date 2022-06-20
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

#include "pwmEX.h"

void PWM_config(void) {
    /* Only if you enable enhanced PWM module, can you set the options  */
    PWMSET |= PWMSET_ENPWM;
    /* Enable external registers */
    P_SW2  |= P_SW2_EAXFR;
    /* Select SYSCLK and the preset clock division parament is 1 */
    PWMCKS &= ~(PWMCKS_SELT2);
    PWMCKS |= PWMCKS_PWM_PS_SCLK_DIV12;
    /* PWM_frequency = SYSCLK/(PWM_PS+1)/(PWMC+1) */
    PWMC    = 0x0100;    // PWM frequency setting [PWMCH(7 bits), PWMCL(8 bits)]
    PWM7T1  = 0x0080;    // Low level output position
    PWM7T2  = 0x0000;    // High level output position
    PWM7CR  = 0x88;      // Enable PWMi channel but disable PWMi interrupt and Select P1.7 as the output pin and initial output is low.
    /* Disable external registers */
    P_SW2  &= ~P_SW2_EAXFR;

    /* PWM start counting */
    PWMCFG |= PWMCFG_PWMCEN;
}

void PWM_DutySet(uint8_t PWM_id, uint16_t dutyL, uint16_t dutyH) {
    P_SW2 |= P_SW2_EAXFR;

    switch(PWM_id) { 
        case 0: PWM0T1 = dutyL & 0x7FFF; PWM0T2 = dutyH & 0x7FFF; break;
        case 1: PWM1T1 = dutyL & 0x7FFF; PWM1T2 = dutyH & 0x7FFF; break;
        case 2: PWM2T1 = dutyL & 0x7FFF; PWM2T2 = dutyH & 0x7FFF; break;
        case 3: PWM3T1 = dutyL & 0x7FFF; PWM3T2 = dutyH & 0x7FFF; break;
        case 4: PWM4T1 = dutyL & 0x7FFF; PWM4T2 = dutyH & 0x7FFF; break;
        case 5: PWM5T1 = dutyL & 0x7FFF; PWM5T2 = dutyH & 0x7FFF; break;
        case 6: PWM6T1 = dutyL & 0x7FFF; PWM6T2 = dutyH & 0x7FFF; break;
        case 7: PWM7T1 = dutyL & 0x7FFF; PWM7T2 = dutyH & 0x7FFF; break;
    }
    P_SW2 &= ~P_SW2_EAXFR;
}

ErrorStatus PWM_ForcedLevelSet(uint8_t PWM_id, uint8_t pwm_hldl, uint8_t pwm_hldh)
{
    if(PWM_id > 7)   return ERROR;  // id ERROR
    if(pwm_hldh > 1) return ERROR;  // Low level control bit
    if(pwm_hldl > 1) return ERROR;  // High level control bit

    P_SW2 |= P_SW2_EAXFR;
    switch(PWM_id) {
        case 0: PWM0HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 1: PWM1HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 2: PWM2HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 3: PWM3HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 4: PWM4HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 5: PWM5HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 6: PWM6HLD = (pwm_hldh << 1) | pwm_hldl; break;
        case 7: PWM7HLD = (pwm_hldh << 1) | pwm_hldl; break;
    }
    P_SW2 &= ~P_SW2_EAXFR;
    return SUCCESS;
}
