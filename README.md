# :rocket: MiniScope on STC8A8K64D4

![GitHub](https://img.shields.io/github/license/HangX-Ma/MiniScpoe-STC8A?color=blue&logo=github&logoColor=black) ![GitHub last commit](https://img.shields.io/github/last-commit/HangX-Ma/MiniScpoe-STC8A?style=flat) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/HangX-Ma/MiniScpoe-STC8A?color=green)

I want to use this **MiniScope** project to learn embedded system. I imitate [Open-ELL/MiniDSO-Pro](https://gitee.com/open-ell/mini-dso-pro) to make a **MiniScope** on my own but the hardware and software seems not suitable for me on account of self-changes. I have written a convenient system library for _STC8x_ easy usage. Currently I only finish the _STC8Ax_ and _STC8Hx_(mainly based on _STC8A8K64D_).

## Test Recorder
- [x] ADC Inquiry Method
- [x] ADC `VBAT` Module
- [x] UARTx Module
- [x] External Interrupt 0 and 1
- [x] TM0 and TM1 for keys scanning and VBAT information update
- [x] EEPROM Module
- [x] SSD1306 OLED
- [x] Setting options selection
- [x] Chart options selection
- [x] Wave scroll options selection
- [x] Run/Stop function

**NOTE: EC11 external Interrupt seems wrong. I think this may cause by hardware design or the EC11 device itself.**

## New Design

### Timer Source Usage
- `TM0`: VBAT info updating rate
- `TM1`: Keys press types recognition, short or long
- `TM3`: UART1 Rx out of time


### FUNCTION INTRODUCTION

#### OPERATION INTRO:
##### Main Interface - Options Selection Mode: 
- `SW Button`:              Switch Stop/Run
- `Short Press SETTING`:    Switch to [Waveform Horizontal Scroll Mode]
- `SEL`:                    Select or deselect this option.
- `NEXT/INC`:               Jump to next option or increase value.
- `LAST/DEC`:               Jump to last option or decrease value.


##### Main Interface - Waveform Horizontal Scroll Mode:
- `SW Button`:              Switch Stop/Run
- `Short Press SETTING`:    Switch to [Options Selection Mode]
- `[SEL] SET(Horizontal waveform scroll, Valid in suspended sampling state)`:
  - `NEXT/INC`:           Jump to next option or increase value.
  - `LAST/DEC`:           Jump to last option or decrease value.
- `[SEL] RESET(Vertical waveform scroll, Valid in suspended sampling state):`
    `NEXT/INC`:           Jump to next option or increase value.
    `LAST/DEC`:           Jump to last option or decrease value.

##### Setting Interface: (SETTING)
- `Long Press SETTING`: Return to main interface or enter setting interface from main interface.
- `SEL`:                    Select or de-select this option.
- `NEXT/INC`:               Jump to next option or increase value.
- `LAST/DEC`:               Jump to last option or decrease value.

#### MAIN INTERFACE INTRO:
- `Vertical Time Range`:     "500ms", "200ms", "100ms", "50ms", "20ms", "10ms","5ms", "2ms", "1ms", "500us", "200us", "100us"(100us only used in AUTO Mode)
- `Horizontal Voltage Range`: Set voltage upper range, whose maximum value is 30V. If the voltage upper range is set to '0', system run into AUTO mode.
- `Trigger Value`:            Set trigger voltage value. The trigger position is set to the screen center.
- `Trigger Direction`:        Use up or down arrow indicator, separately representing rising edge trigger and falling edge trigger.
- `Trigger Mode`:
  - `AUTO`:   Sample continuously, whose indicator led is always turned on. SW Button is used to select RUN or STOP;
                If trigger process is successful, trigger position is set to the screen center, otherwise the waveform scrolling irregularly. FAIL will be shown.
  - `NORMAL`: Wait for prerequisite sampling. Indicator led is turned on, indicating the prerequisite sampling finished. Signal can be entered.
                After successfully triggering, display the waveform, and continue to wait for the next trigger;
                If a new trigger is successful, the new waveform will be displayed automatically. If no new trigger is successful, the screen waveform will remain and wait for the next trigger;
                (Be careful to set the correct trigger value, otherwise has been waiting for the trigger state, the screen will not display the waveform)
  - `SINGLE`: Wait for prerequisite sampling. Indicator led is turned on, indicating the prerequisite sampling finished. Signal can be entered.
                After triggering successfully, display the waveform and stop sampling, the user needs to click on the encoder to start the next sampling.
                (Be careful to set the correct trigger value, otherwise has been waiting for the trigger state, the screen will not display the waveform)

#### OPTIONS SETTING SAVING:
Long press SETTING key to leave the setting interface. If options have been changed, the program will save the setting to EEPROM.

#### LVD EVENT:
If LVD event happens, system will not allow user to operate EEPROM to save data.

#### MAIN INTERFACE STATUS: 
- `RUN`:        Sampling in progress 
- `STOP`:       Stop Sampling 
- `FAIL`:       In AUTO mode, trigger value exceeds waveform range causing trigger failure 
- `SUCC`:       In automatic mode, the trigger value displays the trigger success in the waveform range.
- `AUTO`:       Auto range.

#### SETTING INTERFACE STATUS: 
- `PMODE (drawing mode)`: 
  - Vector: waveform display using vector 
  - Dots:   waveform display using dots
- `LSB (sampling factor)`: 100 times the partial voltage factor, (8.2K+2K)/2K*100 = 510. Adjust this value subtly in accordance with actual register values.
- `BRT`:    OLED brightness


> Reference: [Open-ELL/MiniDSO-Pro](https://gitee.com/open-ell/mini-dso-pro)