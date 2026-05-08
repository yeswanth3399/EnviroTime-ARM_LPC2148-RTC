//rtc_test.c
#include <LPC21xx.h>

//header file contains gpio pin defines
#include "hardware.h"

//inlcuded kpm header to initialize kpm
#include "kpm.h"

//included rtc header for rtc initialization
#include "rtc.h"

//included lcd header for initialization and displaying content
#include "lcd.h"
#include "lcd_defines.h"

//included lm35 initiliztion and reading lm35 values
#include "lm35.h"

//header file contains all states
#include "state.h"
#include "normal.h"
#include "rtc_edit.h"
#include "menu.h"
#include "alarm.h"
#include "alarm_ring.h"

//timer delays
#include "timer0_delay.h"

//default password stores in RAM at initial state
char stored_pass[8]= "1234";

state_t state = STATE_NORMAL;


// Main function
int main()
{
	//Initialize timer0 
	InitTimer0();
  // Initialize RTC 
	RTC_Init();
  // Initialize the LCD
	InitLCD();
	//Initialize kpm
	InitKPM();
	
	//cfg buzzer pin as gpio out
	IODIR0|= 1<<BUZZER_PIN;
	
	
  while (1) 
  {
		switch(state)
		{
			case STATE_NORMAL:	
				state=normal_mode();
				if(EDIT_SWITCH)
				{
						state= STATE_PASSWORD;
				}
				break;
				
			case STATE_PASSWORD: 
				state= password_check(stored_pass)? STATE_MENU: STATE_NORMAL;
				break;
			
			case STATE_MENU: 
				state= menu_handler();
				break;
			
			case STATE_EDIT_TIME: 
				rtc_edit_all();
				state= STATE_MENU;
				break;
			
			case STATE_SET_ALARM: 
				alarm_set();
				state=STATE_MENU;
				break;
			
			case STATE_CHANGE_PASSWORD: 
				change_password(stored_pass);
				state= STATE_MENU;
				break;
			
			case STATE_ALARM_RING:
				alarm_ring_handler();
			  break;
		}														
  }
}
