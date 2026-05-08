// alarm.c
#include <LPC21xx.h>
#include "alarm.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "timer0_delay.h"
#include "rtc.h"

alarm_t alarms[MAX_ALARMS];

u8 current_alarm = 0;

u8 snooze_count = 0;

//alarm list display
void alarm_display(u8 index)
{
    CmdLCD(CLEAR_LCD);
    // -------------------------
    // FIRST LINE
    // -------------------------
    CharLCD('A');
    CharLCD(index+'1');
    CharLCD(' ');

    // Hour
    CharLCD((alarms[index].hr/10)+'0');
    CharLCD((alarms[index].hr%10)+'0');
    CharLCD(':');
    // Minute
    CharLCD((alarms[index].min/10)+'0');
    CharLCD((alarms[index].min%10)+'0');
    CharLCD(':');
    // Second
    CharLCD((alarms[index].sec/10)+'0');
    CharLCD((alarms[index].sec%10)+'0');
	
    // Enable/Disable status
    if(alarms[index].enabled)
    {
        StrLCD(" ON");
    }
    else
    {
        StrLCD(" OFF");
    }
    // Second line
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("A< B> CED #E *Ex");
}

// Configure RTC alarm registers (no interrupts)
void alarm_set()
{
    u32 hr,min,sec;

    // ---------------- HOUR ----------------
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Hr(0-23):");
        CmdLCD(GOTO_LINE2_POS0);
        hr = ReadNumTimeout(10000);
				if(hr== TIMEOUT_NUM)
				{
					CmdLCD(CLEAR_LCD);
					StrLCD("TIMEOUT");
					tdelay_ms(1000);
					return;
				}

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
        min = ReadNumTimeout(10000);
				if(min== TIMEOUT_NUM)
				{
					CmdLCD(CLEAR_LCD);
					StrLCD("TIMEOUT");
					tdelay_ms(1000);
					return;
				}

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
        sec = ReadNumTimeout(10000);
				if(sec== TIMEOUT_NUM)
				{
					CmdLCD(CLEAR_LCD);
					StrLCD("TIMEOUT");
					tdelay_ms(1000);
					return;
				}

        if(sec <= 59)
            break;
				
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Sec");
        tdelay_ms(1000);
    }

    // program alarm registers
    alarms[current_alarm].hr  = hr;
		alarms[current_alarm].min = min;
		alarms[current_alarm].sec = sec;
		alarms[current_alarm].enabled = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("Alarm Set");
    tdelay_ms(500);
		CmdLCD(CLEAR_LCD);
}

void alarm_menu(void)
{
    u8 key;
    u32 elapsed;
	
    while(1)
    {
        elapsed = 0;
        alarm_display(current_alarm);

        // -------------------------
        // WAIT FOR KEY / TIMEOUT
        // -------------------------
        while(ColScan())
        {
            tdelay_ms(1);
            elapsed++;

            // 10 sec timeout
            if(elapsed >= 10000)
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("TIMEOUT");
                tdelay_ms(1000);
                return;
            }
        }

        key = KeyScan();
        while(!ColScan());
        tdelay_ms(20);

        // -------------------------
        // PREVIOUS
        // -------------------------
        if(key == 'A')
        {
            if(current_alarm > 0)
            {
                current_alarm--;
            }
        }

        // -------------------------
        // NEXT
        // -------------------------
        else if(key == 'B')
        {
            if(current_alarm < (MAX_ALARMS-1))
            {
                current_alarm++;
            }
        }
				
        // -------------------------
        // ENABLE / DISABLE
        // -------------------------

        else if(key == 'C')
        {
            alarms[current_alarm].enabled ^= 1;
        }
				
        // -------------------------
        // EDIT
        // -------------------------

        else if(key == '#')
        {
            alarm_set();
        }

        // -------------------------
        // DELETE
        // -------------------------

        else if(key == 'D')
        {
            alarms[current_alarm].enabled = 0;
            alarms[current_alarm].hr  = 0;
            alarms[current_alarm].min = 0;
            alarms[current_alarm].sec = 0;
        }
				
        // -------------------------
        // EXIT
        // -------------------------

        else if(key == '*')
        {
            return;
        }
    }
}

// Poll ILR bit1 (alarm)
int alarm_check(void)
{
    s32 hr,min,sec;
    u8 i;
    GetRTCTimeInfo(&hr,&min,&sec);
    for(i=0;i<MAX_ALARMS;i++)
    {
        if(alarms[i].enabled)
        {
            if((alarms[i].hr  == hr) &&
               (alarms[i].min == min) &&
               (alarms[i].sec == sec))
            {
                current_alarm = i;

                return 1;
            }
        }
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
        alarms[current_alarm].enabled = 0;
        return;
    }
		
    snooze_count++;
    // Get current RTC time
    GetRTCTimeInfo(&hr,&min,&sec);

    // Add snooze minutes
    min = min + minutes;
    // Handle minute overflow
    if(min >= 60)
    {
        hr = hr + (min/60);
        min = min % 60;
    }

    // Handle hour overflow
    if(hr >= 24)
    {
        hr = hr % 24;
    }

    // Update current alarm
    alarms[current_alarm].hr  = hr;
    alarms[current_alarm].min = min;
    alarms[current_alarm].sec = sec;
}
