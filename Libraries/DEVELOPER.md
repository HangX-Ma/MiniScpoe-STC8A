# `ZJUER:` :rocket: Record The Development of STC8A

## Storage
### Absolute Variable Location
The Cx51 guidance points out that `_at_` is used to define the variable absolute address in memory space. 
- Bit variables, functions can not use `_at_`.
- `_at_` Variables cannot be initialized.
- To access **XDATA** peripherals, **volatile** keywords is needed to adorn the variables.

## Pointers
### Abstract Pointers
The address in language C is a constant value.
[Abstract Pointers](https://www.keil.com/support/man/docs/c51/c51_le_abstractptrs.htm) \
[C51: Warning 259 (Pointer: Different Mspace)](https://www.keil.com/support/docs/832.htm)

## DataType
### Special Function Register
#### 1. sbit 
The sbit type defines a bit within a special function register (SFR). Only those SFRs whose address is evenly divisible by 8 are bit-addressable. The lower nibble of the SFR's address must be 0 or 8.

## Functions
### Standard C differ from C51
#### 2. sprintf()