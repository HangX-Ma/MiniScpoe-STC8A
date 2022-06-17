/**
 * @file stc8x_eeprom.c
 * @author MContour (m-contour@qq.com)
 * @brief STC8x EEPROM basic function realization 
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

#include "stc8x_eeprom.h"
#include "INTRINS.H"

/**
 * @brief IAP/EEPROM IDLE mode trigger function
 */
void IAP_IDLE(void) {
    /* disable IAP/EEPROM */
    IAP_CONTR = 0;
    /* clear command */
    IAP_CMD   = 0;
    /* clear trigger */
    IAP_TRIG  = 0;
    /* change value in address regiseter to non-eeprom address area */
    IAP_ADDRH  = 0x80;
    IAP_ADDRL  = 0x00;
}


/**
 * @brief Read a byte from specific address
 * 
 * @param addr given address where to retrive data
 * @return uint8_t returned 1 byte data
 */
uint8_t IAP_ReadByte(uint16_t addr) {
    uint8_t idata IOdata;
    /* enable IAP/EEPROM */
    IAP_CONTR |= IAP_CONTR_IAPEN;
    #if (LIB_MCU_MODULE == STC8Ax)
        /* set waiting time for IAP/EEPROM */
        IAP_CONTR &= ~(IAP_CONTR_IAP_WT);
        /* select corresponding register value */
        switch (LIB_CLK_FREQ) {
            case 1:  IAP_CONTR |= IAP_CONTR_IAP_WT_1MHz;  break;
            case 2:  IAP_CONTR |= IAP_CONTR_IAP_WT_2MHz;  break;
            case 3:  IAP_CONTR |= IAP_CONTR_IAP_WT_3MHz;  break;
            case 6:  IAP_CONTR |= IAP_CONTR_IAP_WT_6MHz;  break;
            case 12: IAP_CONTR |= IAP_CONTR_IAP_WT_12MHz; break;
            case 20: IAP_CONTR |= IAP_CONTR_IAP_WT_20MHz; break;
            case 24: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
            case 30: IAP_CONTR |= IAP_CONTR_IAP_WT_30MHz; break;
            default: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
        }
    #elif (LIB_MCU_MODULE == STC8Hx) 
        /* set waiting time for IAP/EEPROM */
        IAP_TPS = (uint32_t)LIB_CLK_FREQ/1000000;
    #endif
    /* set EEPROM read command */
    IAP_CMD   = IAP_CMD_CMD_READ;
    /* set address */
    IAP_ADDRH = addr >> 8;
    IAP_ADDRL = addr;
    /* trigger command */
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    /* wait for EEPROM operation */
    _nop_();
    /* get data retrived from EEPROM */
    IOdata = IAP_DATA;
    /* change IAP/EEPROM to IDLE mode */
    IAP_IDLE();

    return IOdata;
}

/**
 * @brief Write a byte to specific address
 * 
 * @param addr given address
 * @param IOdata data needs to be written
 */
void IAP_WriteByte(uint16_t addr, uint8_t IOdata) {
    /* enable IAP/EEPROM */
    IAP_CONTR |= IAP_CONTR_IAPEN;
    #if (LIB_MCU_MODULE == STC8Ax)
        /* set waiting time for IAP/EEPROM */
        IAP_CONTR &= ~(IAP_CONTR_IAP_WT);
        /* select corresponding register value */
        switch (LIB_CLK_FREQ) {
            case 1:  IAP_CONTR |= IAP_CONTR_IAP_WT_1MHz;  break;
            case 2:  IAP_CONTR |= IAP_CONTR_IAP_WT_2MHz;  break;
            case 3:  IAP_CONTR |= IAP_CONTR_IAP_WT_3MHz;  break;
            case 6:  IAP_CONTR |= IAP_CONTR_IAP_WT_6MHz;  break;
            case 12: IAP_CONTR |= IAP_CONTR_IAP_WT_12MHz; break;
            case 20: IAP_CONTR |= IAP_CONTR_IAP_WT_20MHz; break;
            case 24: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
            case 30: IAP_CONTR |= IAP_CONTR_IAP_WT_30MHz; break;
            default: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
        }
    #elif (LIB_MCU_MODULE == STC8Hx)
        /* set waiting time for IAP/EEPROM */
        IAP_TPS = (uint32_t)LIB_CLK_FREQ/1000000;
    #endif
    /* set EEPROM read command */
    IAP_CMD   = IAP_CMD_CMD_WRITE;
    /* set address */
    IAP_ADDRH = addr >> 8;
    IAP_ADDRL = addr;
    /* get data retrived from EEPROM */
    IAP_DATA  = IOdata;
    /* trigger command */
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    /* wait for EEPROM operation */
    _nop_();
    /* change IAP/EEPROM to IDLE mode */
    IAP_IDLE();
}


void EEPROM_PageErase(uint16_t addr) {
    /* enable IAP/EEPROM */
    IAP_CONTR |= IAP_CONTR_IAPEN;
    #if (LIB_MCU_MODULE == STC8Ax)
        /* set waiting time for IAP/EEPROM */
        IAP_CONTR &= ~(IAP_CONTR_IAP_WT); // Clear previous value
        /* select corresponding register value */
        switch (LIB_CLK_FREQ/1000000) {
            case 1:  IAP_CONTR |= IAP_CONTR_IAP_WT_1MHz;  break;
            case 2:  IAP_CONTR |= IAP_CONTR_IAP_WT_2MHz;  break;
            case 3:  IAP_CONTR |= IAP_CONTR_IAP_WT_3MHz;  break;
            case 6:  IAP_CONTR |= IAP_CONTR_IAP_WT_6MHz;  break;
            case 12: IAP_CONTR |= IAP_CONTR_IAP_WT_12MHz; break;
            case 20: IAP_CONTR |= IAP_CONTR_IAP_WT_20MHz; break;
            case 24: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
            case 30: IAP_CONTR |= IAP_CONTR_IAP_WT_30MHz; break;
            default: IAP_CONTR |= IAP_CONTR_IAP_WT_24MHz; break;
        }
    #elif (LIB_MCU_MODULE == STC8Hx)
        /* set waiting time for IAP/EEPROM */
        IAP_TPS = (uint32_t)LIB_CLK_FREQ/1000000;
    #endif
    /* set EEPROM read command */
    IAP_CMD   = IAP_CMD_CMD_ERASE;
    /* set address */
    IAP_ADDRH = addr >> 8;
    IAP_ADDRL = addr;
    /* trigger command */
    IAP_TRIG  = 0x5A;
    IAP_TRIG  = 0xA5;
    /* wait for EEPROM operation */
    _nop_();
    /* change IAP/EEPROM to IDLE mode */
    IAP_IDLE();
}

void EEPROM_Read_N_Bytes(uint8_t *str, uint8_t N, uint16_t addr) {
    int16_t i;
    for (i = 0; i < N; i++) {
        *(str+i) = IAP_ReadByte(i+addr);
    } // read a byte at EEPROM_ADDRESS:(addr+i) and save this byte at RAM_ADDRESS:(str+i)
}

ErrorStatus EEPROM_Write_N_Bytes(uint8_t *str, uint8_t N, uint16_t addr) {
    int8_t i;
    uint16_t start_page_addr = (addr/EEPROM_PAGE_SIZE)*EEPROM_PAGE_SIZE + EEPROM_START_ADDR;
    EEPROM_PageErase(start_page_addr);

    for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
        if (IAP_ReadByte(start_page_addr + i) != 0xFF) {
            return ERROR; 
        }
    } // check if the erasure operation success

    for (i = 0; i < N; i++) {
        IAP_WriteByte(start_page_addr + i, *(str + i));
    } // write value to EEPROM

    for (i = 0; i < N; i++) {
        if (IAP_ReadByte(start_page_addr + i) != *(str + i)) {
            return ERROR;
        }
    } // check value in EEPROM

    return SUCCESS;
}
