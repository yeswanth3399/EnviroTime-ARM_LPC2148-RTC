// menu.c
#include "menu.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "timer0_delay.h"
#include <string.h>

int password_check(char *stored)
{
    char in[8];
    int tries=0;

    while(tries < 3)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Enter Pass:");
        CmdLCD(GOTO_LINE2_POS0);

		//timeout check
        if((read_str_timeout(in, sizeof(in),10000)) ==TIMEOUT_STR)
		{
			CmdLCD(CLEAR_LCD);
			StrLCD("TIMEOUT");
			tdelay_ms(1000);
			return 0;
		}

        if(strcmp(in,stored)==0)
            return 1;

        CmdLCD(CLEAR_LCD);
        StrLCD("Wrong!");
        tdelay_ms(1000);
        tries++;
    }

    CmdLCD(CLEAR_LCD);
    StrLCD("LOCKED!");
    tdelay_ms(1000);
    return 0;
}

state_t menu_handler(void)
{
    u8 k;
		u32 elapsed=0;
    CmdLCD(CLEAR_LCD);
    StrLCD("1:Tim 2:Alarm");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("3:Pass 4:Ext");

    
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
					return STATE_NORMAL;
        }
    }
    
		k = KeyScan();
    while(!ColScan());
    tdelay_ms(20);
		
    if(k=='1') 
		{
			return STATE_EDIT_TIME; // STATE_EDIT_TIME
		}
    if(k=='2')
		{
			return STATE_SET_ALARM; // STATE_SET_ALARM
		}
    if(k=='3')
		{
			return STATE_CHANGE_PASSWORD; // STATE_CHANGE_PASSWORD
		}
    if(k=='4')
		{
			CmdLCD(CLEAR_LCD);
			return STATE_NORMAL; // STATE_NORMAL
		}

    return STATE_MENU; // STATE_MENU
}

void change_password(char *stored)
{
    char oldp[8], newp[8], conf[8];

    CmdLCD(CLEAR_LCD); StrLCD("Old:");
    CmdLCD(GOTO_LINE2_POS0);
    
	if((read_str_timeout(oldp, sizeof(oldp), 10000)))
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIME_OUT");
		tdelay_ms(1000);
		return;
	}

    if(strcmp(oldp,stored)!=0)
			{
        CmdLCD(CLEAR_LCD); 
				StrLCD("Wrong!");
        tdelay_ms(1000);
        return;
    }

    CmdLCD(CLEAR_LCD); 
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("New:");
    CmdLCD(GOTO_LINE2_POS0);
    if((read_str_timeout(newp, sizeof(newp),10000)))
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIME_OUT");
		tdelay_ms(1000);
		return;
	}

    CmdLCD(CLEAR_LCD); 
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Conf:");
    CmdLCD(GOTO_LINE2_POS0);
    if((read_str_timeout(conf, sizeof(conf),10000)))
	{
		CmdLCD(CLEAR_LCD);
		StrLCD("TIME_OUT");
		tdelay_ms(1000);
		return;
	}

    if(strcmp(newp,conf)!=0)
		{
			CmdLCD(CLEAR_LCD); 
			StrLCD("Mismatch!");
			tdelay_ms(1000);
			return;
    }

		// Update password in RAM
    strcpy(stored,newp);

    CmdLCD(CLEAR_LCD);
    StrLCD("Updated!");
    tdelay_ms(1000);
}
