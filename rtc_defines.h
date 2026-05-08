//rtc_defines.h
// System clock and peripheral clock Macros
#define FOSC 12000000
#define CCLK (FOSC*5)    
#define PCLK (CCLK/4)

// RTC Prescaler Calculation Macros
// RTC requires 32.768 kHz clock for 1-second increment.
// PREINT and PREFRAC registers divide PCLK to generate 32.768 kHz.

// PREINT = int (PCLK / 32768) - 1;
// PREFRAC = PCLK -((PREINT + 1) * 32768);
// Note: This information collected from LPC2129 Manual

#define PREINT_VAL  ((int) ((PCLK / 32768) - 1))
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))

//RTC Control Register (CCR) Bit Definitions
// Bit 0 ? Clock Enable --> 1 = Enable RTC counters  0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

// Bit 1 ? Clock Reset --> 1 = Reset RTC counters    0 = Normal operation
#define RTC_RESET (1<<1)
  
//only for LPC2148
// Bit 4 ? Clock Source Select 
// 1 = Use external 32.768 kHz oscillator
// 0 = Use internal PCLK as RTC clock source
#define RTC_CLKSRC  (1<<4) 

//CPU selection
//#define LPC2129
#define LPC2148
