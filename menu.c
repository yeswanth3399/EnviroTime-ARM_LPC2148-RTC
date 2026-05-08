// menu.c
#include "menu.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "timer0_delay.h"
#include <string.h>

// masked string input, ends with '#'
static void read_str(char *buf, u8 maxlen)
{
    u8 i=0,k;
    while(1)
    {
        k = KeyScan();
        if(!k) continue;
        // ENTER key
        if(k=='#')
        {
            while(ColScan()==0);   // wait release
            break;
        }

        if(i < maxlen-1){
            buf[i++] = k;
            CharLCD('*');
        }
        while(!ColScan());
    }
    buf[i]='\0';
}

int password_check(char *stored)
{
    char in[8];
    int tries=0;

    while(tries < 3)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Enter Pass:");
        CmdLCD(GOTO_LINE2_POS0);

        read_str(in, sizeof(in));

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
    CmdLCD(CLEAR_LCD);
    StrLCD("1:Tim 2:Alarm");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("3:Pass 4:Ext");

    u8 k;
    do{ 
			k = KeyScan(); 
		}while(!k);
    while(!ColScan());

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
    read_str(oldp, sizeof(oldp));

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
    read_str(newp, sizeof(newp));

    CmdLCD(CLEAR_LCD); 
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD("Conf:");
    CmdLCD(GOTO_LINE2_POS0);
    read_str(conf, sizeof(conf));

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
