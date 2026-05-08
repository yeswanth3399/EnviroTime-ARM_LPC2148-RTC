// alarm_ring.c

#include <LPC21xx.h>

#include "alarm_ring.h"
#include "alarm.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "timer0_delay.h"
#include "state.h"
#include "hardware.h"

// Current state variable
extern state_t state;

// -------------------------------------------------
// ACTIVE HIGH BUZZER CONTROL
// -------------------------------------------------

static void buzzer_on(void)
{
    IOSET0 = 1<<BUZZER_PIN;
}

static void buzzer_off(void)
{
    IOCLR0 = 1<<BUZZER_PIN;
}

// -------------------------------------------------
// ALARM RING HANDLER
// -------------------------------------------------

void alarm_ring_handler(void)
{
	s32 hr,min,start_sec,cur_sec;
	u8 stop_flag = 0;

	// Clear LCD and show alarm message
	CmdLCD(CLEAR_LCD);
	StrLCD("ALARM!!");

	// Store alarm start second
	GetRTCTimeInfo(&hr,&min,&start_sec);

	while(1)
	{
		// -------------------------
		// BUZZER ON
		// -------------------------
		buzzer_on();
		tdelay_ms(50);
		
		// Check ACTIVE LOW stop switch
		if(!(ALARM_STOP_SWITCH))
		{
			tdelay_ms(20);
			while(!(ALARM_STOP_SWITCH));
      stop_flag = 1;
			break;
		}

		// -------------------------
		// BUZZER OFF
		// -------------------------
		buzzer_off();
		tdelay_ms(50);

		// Check ACTIVE LOW stop switch
		if(!(ALARM_STOP_SWITCH))
		{
			tdelay_ms(20);
			while(!(ALARM_STOP_SWITCH));
			stop_flag = 1;
			break;
		}

		// -------------------------
		// CHECK ELAPSED RTC TIME
		// -------------------------
		GetRTCTimeInfo(&hr,&min,&cur_sec);
		// Ring exactly for 30 seconds
		if(((cur_sec - start_sec + 60) % 60) >= 30)
			break;
	}

	// Ensure buzzer OFF
  buzzer_off();

	// -------------------------
	// MANUAL STOP
	// -------------------------
	if(stop_flag)
  {
		CmdLCD(CLEAR_LCD);
		StrLCD("ALARM STOP");
		tdelay_ms(1000);
		state = STATE_NORMAL;
		return;
	}
	
	// -------------------------
	// AUTO SNOOZE
	// -------------------------
	alarm_snooze(5);
	CmdLCD(CLEAR_LCD);
	StrLCD("AUTO SNOOZE");
	tdelay_ms(1000);
	state = STATE_NORMAL;
}
