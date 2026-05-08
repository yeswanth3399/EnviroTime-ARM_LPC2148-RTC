//normal mode display time and temp
#include "normal.h"
#include "lm35.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "alarm.h"
#include "timer0_delay.h"


extern state_t state;
s32 hour,min,sec,date,month,year,day;
enum days{SUN,MON,TUE,WED,THU,FRI,SAT};

state_t normal_mode(void)
{
	s32 tDegC,tDegF;
	
	Read_LM35NP(&tDegC,&tDegF);
	
  // Get and display the current time info on LCD
	GetRTCTimeInfo(&hour,&min,&sec);
	DisplayRTCTime(hour,min,sec);
	
	//display temp reading from LM35
	CmdLCD(GOTO_LINE1_POS0+8);
	StrLCD("        ");

	CmdLCD(GOTO_LINE1_POS0+11);
	S32LCD(tDegC);
	StrLCD("dC ");
	
	// Get and display the current date info on LCD
	GetRTCDateInfo(&date,&month,&year);
	DisplayRTCDate(date,month,year);
	
	// Get and display the current day info on LCD
	GetRTCDay(&day);
	CmdLCD(GOTO_LINE2_POS0+10);
	StrLCD("      ");
	DisplayRTCDay(day);
	
	tdelay_ms(100);
	
	// Poll RTC alarm flag
  if(alarm_check())
		return STATE_ALARM_RING;
	
	return STATE_NORMAL;
}
