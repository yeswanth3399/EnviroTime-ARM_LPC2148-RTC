// rtc_edit.c
#include "rtc_edit.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "timer0_delay.h"

// Using same day names from rtc.c
extern char *week[];

/*
Function: rtc_edit_all
Purpose : Edit full RTC (time + date + day)
Uses    : Your rtc.c APIs
Flow    : HH ? MM ? SS ? DD ? MM ? YYYY ? DAY
*/

void rtc_edit_all(void)
{
    u32 hr, min, sec;
    u32 date, month, year;
    u32 day;

    // ----------- TIME -----------
    CmdLCD(CLEAR_LCD);
    StrLCD("Set Hour(0-23)");
    CmdLCD(GOTO_LINE2_POS0);
    hr = ReadNumTimeout(10000);
	if(hr== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(hr > 23) goto error;

    CmdLCD(CLEAR_LCD);
    StrLCD("Set Min(0-59)");
    CmdLCD(GOTO_LINE2_POS0);
    min = ReadNumTimeout(10000);
	if(min== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(min > 59) goto error;

    CmdLCD(CLEAR_LCD);
    StrLCD("Set Sec(0-59)");
    CmdLCD(GOTO_LINE2_POS0);
    sec = ReadNumTimeout(10000);
	if(sec== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(sec > 59) goto error;

    // ----------- DATE -----------
    CmdLCD(CLEAR_LCD);
    StrLCD("Set Date(1-31)");
    CmdLCD(GOTO_LINE2_POS0);
    date = ReadNumTimeout(10000);
	if(date== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(date < 1 || date > 31) goto error;

    CmdLCD(CLEAR_LCD);
    StrLCD("Set Month(1-12)");
    CmdLCD(GOTO_LINE2_POS0);
    month = ReadNumTimeout(10000);
	if(month== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(month < 1 || month > 12) goto error;

    CmdLCD(CLEAR_LCD);
    StrLCD("Set Year");
    CmdLCD(GOTO_LINE2_POS0);
    year = ReadNumTimeout(10000);
	if(year== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    // ----------- DAY -----------
    CmdLCD(CLEAR_LCD);
    StrLCD("Day 0SUN-6SAT:");
    CmdLCD(GOTO_LINE2_POS0);
    day = ReadNumTimeout(10000);
	if(day== TIMEOUT_NUM)
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIMEOUT");
		tdelay_ms(1000);
		return;
	}

    if(day > 6) goto error;

    // ----------- APPLY RTC SETTINGS -----------
    SetRTCTimeInfo(hr, min, sec);
    SetRTCDateInfo(date, month, year);
    SetRTCDay(day);

    // ----------- SUCCESS MESSAGE -----------
    CmdLCD(CLEAR_LCD);
    StrLCD("RTC Updated!");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD(week[day]); 		// show selected day
		tdelay_ms(1000);
		CmdLCD(CLEAR_LCD);
		

    return;

error:
    CmdLCD(CLEAR_LCD);
    StrLCD("Invalid Input!");
    tdelay_ms(1000);
}
