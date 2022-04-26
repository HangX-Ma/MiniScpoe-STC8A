#ifndef __INIT__H__
#define __INIT__H__

#include "global.h"
#include <STDIO.H>
#include <STDLIB.H>

/**
 * @brief alias for IRC clock selection
 * 
 */
typedef enum {
    IRC24M_CLK = 0,
    IRC32K_CLK,
}IRC_ClkType;

/**
 * @brief Change system clock
 * @details \verbatim Usage guidance:
 *   - invoke this in the beginning of code
 *   - don't invoke this if the target frequency is already set by STC-ISP 
 * \endverbatim
 * 
 * @verbatim 
 * 
 * STC8H Clock: 
 *  MCKSEL                     ||===> MCLKODIV ==> MCLKO_S(1,0) => P1.6/P5.4
 *  00 Internal IRC    |       ||
 *  01 External OSC    |==> CLKDIV ==> SYSCLK
 *  10 External 32KHz  |
 *  11 Internal 32KHz  |
 * @endverbatim
 * 
 */
void SYS_SetClock(IRC_ClkType type);


/**
 * @brief Low voltage level selection
 */
#define LVD2V2 0x00 // Low voltage detection 2.2V
#define LVD2V4 0x01 // Low voltage detection 2.4V
#define LVD2V7 0x02 // Low voltage detection 2.7V
#define LVD3V0 0x03 // Low voltage detection 3.0V
void SYS_LowVolDetection(void);




#endif  //!__INIT__H__