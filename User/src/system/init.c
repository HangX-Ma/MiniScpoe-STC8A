#include "system/init.h"
#include "config.h"

void SYS_SetIRCClock(enum CLK_type type) {
    P_SW2    = 0x80; // enable XSFR read/write
    switch (type)
    {
    case IRC24M_CLK:
        IRC24MCR = 0x80; // enable inner 24MHz IRC
        while (!(IRC24MCR & 0x01)) {
            continue;
        }                // wait until the clock being stable
        break;
    case IRC32K_CLK:
        IRC32KCR = 0x80; // enable inner 32kHz IRC
        while (!(IRC32KCR & 0x01)) {
            continue;
        }                // wait until the clock being stable
        break;
    default:
        IRC24MCR = 0x80; // enable inner 24MHz IRC
        while (!(IRC24MCR & 0x01)) {
            continue;
        }                // wait until the clock being stable
        break;
    }

    CLKDIV   = 0x00; // no clock division
    CKSEL    = 0x00; // no output at p5.4, 24MHz inner IRC
    P_SW2    = 0x80; // enable SFR
}

void SYS_LowVolDetection(bool ifRST, bool ifInterrupt) {
    RSTCFG = 0x50;
}


