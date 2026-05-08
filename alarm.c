// alarm.c
#include <LPC21xx.h>
#include "alarm.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "timer0_delay.h"
#include "rtc.h"

u8 snooze_count = 0;

// Configure RTC alarm registers (no interrupts)
void alarm_set(void)
{
    u32 hr,min,sec;

    // ---------------- HOUR ----------------
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Hr(0-23):");
        CmdLCD(GOTO_LINE2_POS0);
        hr = ReadNum();

        if(hr <= 23)
            break;

        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Hour");
        tdelay_ms(1000);
    }

    // ---------------- MIN ----------------
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Min(0-59):");
        CmdLCD(GOTO_LINE2_POS0);
        min = ReadNum();

        if(min <= 59)
            break;

        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Min");
        tdelay_ms(1000);
    }

    // ---------------- SECOND ----------------
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Sec(0-59):");
        CmdLCD(GOTO_LINE2_POS0);
        sec = ReadNum();

        if(sec <= 59)
            break;
				
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Sec");
        tdelay_ms(1000);
    }

    // program alarm registers
    ALHOUR = hr;
    ALMIN  = min;
    ALSEC  = sec;

    // AMR: 0 = compare, 1 = ignore
    // compare HOUR, MIN, SEC ? bits 2:0 = 0, others 1
    AMR = 0xF8;

    // clear any stale alarm flag
    ILR = 0x02;

    CmdLCD(CLEAR_LCD);
    StrLCD("Alarm Set");
    tdelay_ms(500);
		CmdLCD(CLEAR_LCD);
}

// Poll ILR bit1 (alarm)
int alarm_check(void)
{
    if(ILR & 0x02){
        ILR = 0x02; // clear
        return 1;
    }
    return 0;
}

/*
Function : alarm_snooze
Purpose  : Automatically snooze alarm
*/

void alarm_snooze(u32 minutes)
{
    s32 hr,min,sec;
	
    // Maximum 5 snoozes
    if(snooze_count >= 5)
    {
        snooze_count = 0;
        // Disable alarm compare
        AMR = 0xFF;
        return;
    }
		
    snooze_count++;
    GetRTCTimeInfo(&hr,&min,&sec);
    min += minutes;

    while(min >= 60)
    {
        min -= 60;
        hr++;
    }
    if(hr >= 24)
    {
        hr = 0;
    }

    // Set new alarm
    ALHOUR = hr;
    ALMIN  = min;
    ALSEC  = sec;

    // Compare only HOUR/MIN/SEC
    AMR = 0xF8;
    // Clear alarm flag
    ILR = 0x02;
}
