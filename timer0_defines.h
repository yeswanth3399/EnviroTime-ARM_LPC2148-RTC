//timer0_defines.h
/*--------------------------------------------------------------------------
TIMER0 CONTROL REGISTER BIT DEFINITIONS
----------------------------------------------------------------------------*/
// T0TCR (Timer Control Register) bits
#define TIMER0_ENABLE 0x01  //Bit 0 = 1 -> Enable Timer Counter (TC) and Prescale Counter (PC)
#define TIMER0_RST    0x02  //Bit 1 = 1 -> Reset TC & PC to 0 (auto-clears after write)

// T0MCR (Match Control Register) bits
#define TIMER0_INT_ON_MR0  0x01  //Interrupt on MR0 match
#define TIMER0_RST_ON_MR0  0x02  //Reset TC on MR0 match
#define TIMER0_STOP_ON_MR0 0x04  //Stop TC on MR0 match
