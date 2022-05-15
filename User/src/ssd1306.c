/**
 * @file ssd1306.c
 * @author MContour (m-contour@qq.com)
 * @brief ssd1306 OLED display functions
 * @version 0.1
 * @date 2022-05-14
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

#include "ssd1306.h"
#include "ssd1306font.h"
#include "stc8x_delay.h"
#include "STRING.H"
#include "STDLIB.H"
#include "MATH.H"

extern void delay_nms(uint16_t nms);

#ifndef _swap_u8
    #define _swap_u8(a, b) do {uint8_t tmp = a; a = b; b = tmp;} while(0)
#endif


bit ReverseMode = 0;    //!< 1: reverse display; 0: normal display
bit OverlapMode = 1;    //!< 1: overlap; 0: override

uint8_t pixel_buf[WIDTH*PAGES]; // whole screen pixel size: WIDTH * (8 pages-8bits)
static  uint8_t _font_width = 6;
static  int8_t _locX, _locY;


void OLED_Init(void) {
    _SET_SBIT(OLED_RST);
    delay_nms(50);
    _RESET_SBIT(OLED_RST);
    delay_nms(50);
    _SET_SBIT(OLED_RST);
    delay_nms(1);

    OLED_WR_CMD(0xAE); // set display off (sleep mode)
    delay_nms(5);

    OLED_WR_CMD(0x20); // Set Memory Addressing Mode: 0x00 Horizontal, 0x01 Vertical, 0x02 Page(RESET)
    OLED_WR_CMD(0x00); // Horizontal

    OLED_WR_CMD(0x21); // Set Column Address
    OLED_WR_CMD(0x00); // Start address
    OLED_WR_CMD(0x7F); // End address

    OLED_WR_CMD(0x22); // Set Page Address
    OLED_WR_CMD(0x00); // Start address
    OLED_WR_CMD(0x07); // End address

    OLED_WR_CMD(0x40); // Set Display Start Line (0x40h~0x7F)

    OLED_WR_CMD(0x81); // Set Contrast Control
    OLED_WR_CMD(0x80); // (0x00 ~ 0xFF) The segment output current increases as the contrast step value increases

    OLED_WR_CMD(0xA1); // Set Segment Re-map. 0xA1: Normal, 0xA0: Re-map left to right
    
    OLED_WR_CMD(0xC8); // Set COM Output Scan Direction. 0xC8: Normal, 0xC0: Re-map up to down

    OLED_WR_CMD(0xA6); // Set Normal/Inverse Display, 0xA6:Normal, 0xA7: Inverse

    OLED_WR_CMD(0xA8); // Set Multiplex Ratio (16MUX~64MUX)
    OLED_WR_CMD(0x3F); // 64 multiplex mode

    OLED_WR_CMD(0xD3); // Set Display Offset (0x00~0x3F)
    OLED_WR_CMD(0x00); // No offset

    OLED_WR_CMD(0xD5); // Set display clock divide ratio/oscillator frequency
    OLED_WR_CMD(0x20); // Set Clock as 60 Frames/Sec

    OLED_WR_CMD(0xD9); // Set pre-charge period
    OLED_WR_CMD(0xF2); // Set Pre-Charge as 15 Clocks & Discharge as 2 Clock

    OLED_WR_CMD(0xDA); // Set COM Pins Hardware Configuration
    OLED_WR_CMD(0x12); // Alternative COM pin configuration. Disable COM Left/Right remap

    OLED_WR_CMD(0xDB); // Set VCOM Deselect Level
    OLED_WR_CMD(0x30); // 0.83 x Vcc

    OLED_WR_CMD(0xA4); // Entire Display ON, 0xA4: Disable, 0xA5: Enable
    OLED_WR_CMD(0xAF); // Set Display ON/OFF, 0xAF: Turn on oled panel
}


void OLED_WR_CMD(uint8_t cmd) {
    uint8_t i;
    /* write command mode */
    _RESET_SBIT(OLED_DC);
    /* select device */
    _RESET_SBIT(OLED_CS);
    for (i = 0; i < 8; i++) {
        if (cmd & 0x80) {
            _SET_SBIT(OLED_SDIN);
        } else {
            _RESET_SBIT(OLED_SDIN);
        }
        cmd <<= 1;
    }
    _SET_SBIT(OLED_CS);
}


void OLED_WR_data(uint8_t dat) {
    uint8_t i;
    /* write data mode */
    _SET_SBIT(OLED_DC);
    /* select device */
    _RESET_SBIT(OLED_CS);
    for (i = 0; i < 8; i++) {
        if (dat & 0x80) {
            _SET_SBIT(OLED_SDIN);
        } 
        else {
            _RESET_SBIT(OLED_SDIN);
        }
        dat <<= 1;
    }
    _SET_SBIT(OLED_CS);
}

void OLED_SetPos(uint8_t x, uint8_t y) {
    _locX = x;
    _locY = y;
}


void OLED_ClearScreen(void) {
    memset(pixel_buf, 0x00, WIDTH*PAGES);
}

void OLED_Display(void) {
    uint8_t i, j;
    uint8_t *pBuf;
    pBuf = pixel_buf;

    for (i = 0; i < PAGES; i++) {
        OLED_WR_CMD(0xB0+i); // Set Page Start Address for Page Addressing Mode
        OLED_WR_CMD(0x00);   // Set Lower Column Start Address for Page Addressing Mode
        OLED_WR_CMD(0x10);   // Set Higher Column Start Address for Page Addressing Mode
        for (j = 0; j < WIDTH; j++) {
            OLED_WR_data(*pBuf++);
        }
    }
}


void OLED_ShowByte(uint8_t *pBuf, uint8_t mask, uint8_t offset, bit resv_hl) {
    if (OverlapMode) {
        if (ReverseMode) {
            *pBuf |= ~mask;
        }
        else {
            *pBuf |= mask;
        }
    } // overlap mode
    else {
        if (ReverseMode) {
            if (resv_hl) {
                *pBuf &= (~mask) | (0xFF << (8 - offset));
                *pBuf |= (~mask) & (0xFF >> offset);
            } /* Reserve higher */
            else {
                *pBuf &= (~mask) | (0xFF >> (8 - offset));
                *pBuf |= (~mask) & (0xFF << offset);
            } /* Reserve lower */
        }
        else {
            if (resv_hl) {
                *pBuf &= mask | (0xFF << (8 - offset));
                *pBuf |= mask & (0xFF >> offset);
            } /* Reserve higher */
            else {
                *pBuf &= mask | (0xFF >> (8 - offset)); // clear the lower part but keep previous set data at other bit positions
                *pBuf |= mask & (0xFF << offset);       // read the desired part in mask and set value
            } /* Reserve lower */
        }
    } // override mode
}

// (left to right) small to large: x
// (up to down) small to large: y
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
    uint8_t *pBuf;
    uint8_t i, ch, offset, mask;
    /* pixel_buf is uint8_t type, so 'y' need to become 'y >> 3' to fit each page container size. */
    pBuf   = pixel_buf + (y >> 3) * WIDTH + x; 
    ch     = chr - ' '; // get the character group number, ASCII 
    offset = y & 0x07;  // offset (0~7) for a byte

    for (i = 0; i < _font_width; i++) {
        mask = F6x8[ch][i] << offset; 
        OLED_ShowByte(pBuf++, mask, offset, 0);
    } // deal with the lower part first if there exist an offset

    if (offset && y < HEIGHT - 8)
    {
        pBuf = &pixel_buf[((y >> 3) + 1) * WIDTH + x];
        for (i = 0; i < _font_width; i++)
        {
            mask = F6x8[ch][i] >> (8 - offset);
            OLED_ShowByte(pBuf++, mask, 8 - offset, 1);
        }
    } // deal with the higher part if there exist an offset
}


void OLED_ShowString(uint8_t *str) {
    while (*str) {
        if (_locX > WIDTH - _font_width) {
            _locX = 0;
            _locY += 8;
        }

        if (_locY > HEIGHT - 8) {
            _locY = 0;
        } // back to first page

        OLED_ShowChar(_locX, _locY, *str++);
        _locX += _font_width; // move to next font
    }
}

uint32_t OLED_pow(uint8_t coefficient, uint8_t exponent) {
    uint32_t res = 1;
    while (exponent--) {
        res *= coefficient;
    }
    
    return res;
}

void OLED_ShowNum(uint32_t digit, uint8_t len) {
    uint8_t index, currDigit;

    for (index = 0; index < len; index++) {
        currDigit = (digit/OLED_pow(10, len-index-1))%10;

        // adjust display area: X-axis
        if (_locX > len - _font_width) {
            _locX = 0;
            _locY += 8;
        }
        // adjust display area: Y-axis
        if (_locY > HEIGHT - 8) {
            _locY = 0;
        }

        OLED_ShowChar(_locX + _font_width*index , _locY, currDigit + '0' );
    }
    _locX += len;
}

void OLED_ShowPixel(uint8_t x, uint8_t y) {
    uint8_t mask;
    uint8_t *pBuf;

    if (_locX > WIDTH - 1) {
        _locX = 0;
        _locY += 1;
    }

    if (_locX > HEIGHT - 1) {
        _locX = 0;
    }

    pBuf = pixel_buf + (y >> 3)*WIDTH + x + 1;
    mask = 1 << (y & 7);

    if (ReverseMode) {
        *pBuf++ &= ~mask;
    }
    else {
        *pBuf++ |= mask;
    }
}


void OLED_ShowLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int8_t dx, dy, incx, incy, distance;
    int8_t x_tmp = 0, y_tmp = 0, x = 0, y = 0;
    int8_t index;

    // boundary checking 
    if (x0 > WIDTH - 1) {
        x0 = WIDTH - 1;
    }

    if (y0 > HEIGHT - 1) {
        y0 = HEIGHT - 1;
    }

    if (x1 > WIDTH - 1) {
        x1 = WIDTH - 1;
    }

    if (y1 > HEIGHT - 1) {
        y1 = HEIGHT - 1;
    }
    
    /* Bresenham algorithm drawing Slash Line*/
    /* calculate coordinate offsets */
    dx = x1 - x0;
    dy = y1 - y0;

    if (dx > 0) {
        incx = 1;
    } // draw from left to right
    else if (dx == 0) {
        incx = 0;
    } // vertical slash line (straight line) 
    else {
        incx = -1;
        dx = -dx;
    }  // draw from right to left

    if (dy > 0) {
        incy = 1;
    } // draw from left to right
    else if (dy == 0) {
        incy = 0;
    } // horizontal slash line (straight line) 
    else {
        incy = -1;
        dy = -dy;
    }  // draw from right to left

    /* calculate the stroke distance of the brush (take the maximum of two intervals) */
    if(dx > dy) {
        distance = dx;
    }
    else {
        distance = dy;
    }

    /* draw */
    x = x0;
    y = y0;

    /* first pixel point is invalid, total index needs to be increased. */
    for (index = 0; index <= distance + 1; index++) {
        OLED_ShowPixel(x, y);
        /* recognize the actually nearest pixel */
        x_tmp += dx;
        if (x_tmp > distance) {
            x_tmp -= distance; // x direction overstep the boundary, decrease the 'distance' for next detection.
            x += incx; // draw pixel at x direction
        }

        y_tmp += dy;
        if (y_tmp > distance) {
            y_tmp -= distance; // y direction overstep the boundary, decrease the 'distance' for next detection.
            y += incy; // draw pixel at y direction
        }
    }
}

void OLED_Reverse(bit n) {
    ReverseMode = n;
}

void OLED_Overlap(bit n) {
    OverlapMode = n;
}


