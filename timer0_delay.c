/*=============================================================================
File   : timers_delay.c
Purpose: Demonstrate Timer0 usage for microsecond, millisecond, and second delays
===============================================================================*/

// Header file for LPC21xx register definitions
#include <LPC21xx.h>   

/*-----------------------------------------------------------------------------
   TYPE DEFINITIONS
   (These make the code more readable and portable)
-----------------------------------------------------------------------------*/
#include "types.h"

/*--------------------------------------------------------------------------
TIMER0 CONTROL REGISTER BIT DEFINITIONS
----------------------------------------------------------------------------*/
#include "timer0_defines.h"

/*-----------------------------------------------------------------------------
   FUNCTION: InitTimer0
   PURPOSE : Initialize Timer0 in a known state
-----------------------------------------------------------------------------*/
void InitTimer0(void)
{
    T0TCR = TIMER0_RST;        // Reset Timer0 counters (TC & PC) to 0
    T0MCR = TIMER0_STOP_ON_MR0;// Stop Timer0 when MR0 matches TC
}

/*-----------------------------------------------------------------------------
FUNCTION: tdelay_us
PURPOSE : Generate a delay in microseconds using Timer0
ARG     : microseconds -> delay value in µs
-----------------------------------------------------------------------------*/
void tdelay_us(u32 microseconds)
{
    T0MR0 = microseconds;	// Match after 'microseconds' ticks
    T0PR  = 15 - 1;       // Prescaler: 15MHz PCLK -> 1 tick = 1 µs
    T0TC  = 0;            // Reset Timer Counter
    T0TCR = TIMER0_ENABLE;// Start Timer0
    while (T0TC != T0MR0);// Wait until TC reaches match value
}

/*---------------------------------------------------------------------------
   FUNCTION: tdelay_ms
   PURPOSE : Generate a delay in milliseconds using Timer0
   ARG     : milliseconds ? delay value in ms
-----------------------------------------------------------------------------*/
void tdelay_ms(u32 milliseconds)
{
    T0MR0 = milliseconds;   //Match value in milliseconds
    T0PR  = 15000 - 1;      //Prescaler: 15MHz/15000 -> 1 tick = 1 ms
    T0TC  = 0;              //Reset Timer Counter
    T0TCR = TIMER0_ENABLE;  //Enable Timer0
    while (T0TC != T0MR0);  //Wait until TC reaches match value
}

/*---------------------------------------------------------------------------
   FUNCTION: tdelay_s
   PURPOSE : Generate a delay in seconds using Timer0
   ARG     : seconds ? delay value in seconds
-----------------------------------------------------------------------------*/
void tdelay_s(u32 seconds)
{
    T0MR0 = seconds;         //Match value in seconds
    T0PR  = 15000000 - 1;    //Prescaler: 15MHz/15000000 -> 1 tick = 1 second
    T0TC  = 0;               //Reset Timer Counter
    T0TCR = TIMER0_ENABLE;   //Enable Timer0
    while (T0TC != T0MR0);   //Wait until TC reaches match value
}
