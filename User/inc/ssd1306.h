/**
 * @file ssd1306.h
 * @author MContour (m-contour@qq.com)
 * @brief SSD1306 OLED driver basic function definitions
 * @version 0.1
 * @date 2022-05-10
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

#ifndef __SSD1306__H__
#define __SSD1306__H__

//* ------------      head files      ------------
#include "core_stc8x.h"

#if (LIB_MCU_MODULE == STC8Ax)
    #include "config_stc8ax.h"
#elif (LIB_MCU_MODULE == STC8Hx)
    #include "config_stc8hx.h"
#endif


//* ------------   GLOBAL variables   ------------

//* ------------ developer definitions ------------
/*  OLED Pixel */
#define WIDTH  128
#define HEIGHT 64
#define PAGES  8

#define Max_Column 128
#define Max_Row 64

/*  OLED Pins */
sbit OLED_SCLK = P2^7;   // clock(D0/SCLK)
sbit OLED_SDIN = P2^6;   // data(D1/MOSI)
sbit OLED_RST  = P2^5;   // reset(RES)
sbit OLED_DC   = P2^4;   // data/cmd(DC)
sbit OLED_CS   = P2^3;   // chip select(CS)
//* ------------     functions     ------------

/**
 * @brief OLED initialization
 */
void OLED_Init(void);

/**
 * @brief Write command to screen cache
 * 
 * @param cmd command
 */
void OLED_WR_CMD(uint8_t cmd);

/**
 * @brief Write data to screen cache
 * 
 * @param dat data
 */
void OLED_WR_data(uint8_t dat);

/**
 * @brief Clear OLED screen
 */
void OLED_ClearScreen(void);

/**
 * @brief OLED buffer data display
 */
void OLED_Display(void);

/**
 * @brief Set start point x, y value
 * 
 * @param x start point coordinate x value
 * @param y start point coordinate y value
 */
void OLED_SetPos(uint8_t x, uint8_t y);

/**
 * @brief Set OLED display font width
 * 
 * @param w pixel width
 */
void OLED_SetFontWidth(uint8_t w);


/**
 * @brief Draw a byte to buffer
 * @details If the byte 1111 1111 we want to write cross two pages 0001 1111 1110 0000.
 * In redraw mode, to avoid affect adjacent content. Set offset=3, reserve_hl=1,
 * when write 0001 1111. Set offset=5, reserve_hl=0 when write 1110 0000.
 * @param pBuf buffer pointer
 * @param mask ASCII characters' mask
 * @param offset offset of a byte
 * @param resv_hl reserve High or Low
 */
void OLED_ShowByte(uint8_t *pBuf, uint8_t mask, uint8_t offset, bit resv_hl);

/**
 * @brief Show an ASCII character 
 * 
 * @param x start point coordinate x value
 * @param y start point coordinate y value
 * @param chr ASCII character
 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);

void OLED_ShowString(const uint8_t *str);

/**
 * @brief Show digit number on screen
 * 
 * @param digit number 
 * @param len length of the number
 */
void OLED_ShowNum(uint32_t digit, uint8_t len);


void OLED_ShowChinese_16x16(uint8_t loc);

/**
 * @brief Show a pixel at (x,y)
 * 
 * @param x start point coordinate x value
 * @param y start point coordinate y value
 */
void OLED_ShowPixel(uint8_t x, uint8_t y);

/**
 * @brief Show a vertical line on screen start from (x,y), length 'h'
 * 
 * @param x start point coordinate x value
 * @param y start point coordinate y value
 * @param w line height
 */
void OLED_ShowVerticalLine(uint8_t x, uint8_t y, uint8_t h);

/**
 * @brief Show a horizontal line on screen start from (x,y), length 'w'
 * 
 * @param x start point coordinate x value
 * @param y start point coordinate y value
 * @param w line width
 */
void OLED_ShowHorizontalLine(uint8_t x, uint8_t y, uint8_t w);

/**
 * @brief Draw a line on the screen
 * 
 * @param x0 start coordinate x value
 * @param y0 start coordinate y value
 * @param x1 end coordinate x value
 * @param y1 end coordinate y value
 */
void OLED_ShowLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/**
 * @brief Reverse mode selection
 * 
 * @param n 1: reverse; 0: normal
 */
void OLED_Reverse(bit n);

/**
 * @brief Overlap mode selection
 * 
 * @param n 1: overlap; 0: override
 */
void OLED_Overlap(bit n);


#endif  //!__SSD1306__H__