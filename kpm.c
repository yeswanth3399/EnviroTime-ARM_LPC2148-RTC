//kpm.c
#include <LPC21xx.h>
#include "kpm.h"
#include "types.h"
#include "kpm_defines.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "timer0_delay.h"

#define PROTEUS

#ifdef PROTEUS
u8 kpmLUT[4][4]=
{
		{'7','8','9','A'},
		{'4','5','6','B'},
		{'1','2','3','C'},
		{'*','0','#','D'}
};
#else
u8 kpmLUT[4][4]=
{
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
};
#endif
void InitKPM(void)
{
	//ground all rows
	WRITENIBBLE(IODIR1,ROW0,15);
	//all col are at high and input
	//anyway default they are high and input
}

u8 ColScan(void)
{
	u8 t;
	t=(READNIBBLE(IOPIN1,COL0)<15)?0:1;
	return t;
}

u8 RowCheck(void)
{
	u8 r;
	for(r=0;r<4;r++)
	{
		//ground iteratively one rwo
		//at a time, keep other row as high
		WRITENIBBLE(IOPIN1,ROW0,(~(1<<r)));
		//check if key is pressed in that row
		if(!(ColScan()))
			break;
	}
	//re-initialize the rows as gorund
	WRITENIBBLE(IOPIN1, ROW0, 0);
	return r;
}

u8 ColCheck(void)
{
	u8 c;
	for(c=0; c<4; c++)
	{
		if((READBIT(IOPIN1, (COL0+c)))==0)
			break;
	}
	return c;
}

	
u8 KeyScan(void)
{
	u8 r,c,KeyV;
	//wait for key press
	while(ColScan());
	
	//debounce delay
	tdelay_ms(20);
	
	//if any key was pressed
	//identify row in which key was pressed
	r=RowCheck();
	//identify the col which key was pressed
	c=ColCheck();
	//extract key Value from LUT
	KeyV=kpmLUT[r][c];
	
	//debounce delay
	tdelay_ms(20);
	
	return KeyV;
}

u32 ReadNumTimeout(u32 timeout_ms)
{
	u32 KeyV,sum=0,elapsed=0;
	u8 digit_flag=0,digits=0;
	while(1)
	{
		//-------------------------
		//Wait for key /timeout
		//-------------------------
		while(ColScan())
		{
			tdelay_ms(1);
			elapsed++;
			
			if(elapsed >= timeout_ms)
			{
				return TIMEOUT_NUM;
			}
		}
		elapsed=0;		

		KeyV=KeyScan();

		//numeric input
		if((KeyV>='0') && (KeyV<='9'))
		{
			digit_flag=1;

			//max upto 4 digits
			if(digits<4)
			{
				sum=((sum*10)+(KeyV-48));
				digits++;
				CmdLCD(GOTO_LINE2_POS0);
				U32LCD(sum);
				while(ColScan()==0);
			}
		}
		else if(KeyV =='#')
		{
			//no digits entered
			if(digit_flag==0)
			{
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("               ");
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("Give Input");
				tdelay_ms(500);
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("               ");
				CmdLCD(GOTO_LINE2_POS0);
				continue;
			}
			while(!(ColScan()));
			tdelay_ms(20);
			break;
		}
		else if(KeyV=='D')
		{
			if(digits >0)
			{
				sum=sum/10;
				digits--;
				
				// Reset input flag when all deleted
        if(sum == 0)
        {
            digit_flag = 0;
        }
				
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("               ");
				CmdLCD(GOTO_LINE2_POS0);

				//display onlu if number exit
				if(sum!=0)
				{
					U32LCD(sum);
				}
			}
		}
		while(!(ColScan()));
		tdelay_ms(20);
	}
	return sum;
}

// masked string input, ends with '#'
u8 read_str_timeout(char *buf, u8 maxlen, u32 timeout_ms)
{
    u8 i=0,k,j;
	u32 elapsed=0;
    while(1)
    {
		//-----------------------------
		//wait for key/timeout
		//-----------------------------
		while(ColScan())
		{
			tdelay_ms(1);
			elapsed++;

			if(elapsed >= timeout_ms)
			{
				buf[0]='\0';
				return 0xFF;
			}
		}
		elapsed=0;
		
        k = KeyScan();
        if(!k) continue;
        
		// ENTER key
        if(k=='#')
        {
            while(ColScan()==0);   // wait release
            break;
        }
	  
	  //backspace
	  else if(k =='D')
	  {
	 	if(i>0)
		{
			i--;
			buf[i]='\0';

			//Clear lcd line
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("                ");

			//redisplay password
			CmdLCD(GOTO_LINE2_POS0);
			for(j=0;j<i;j++)
				CharLCD('*');
		}
	  }
	  else
	  {
	     if(i < maxlen-1)
	     {
            	buf[i++] = k;
            	CharLCD('*');
           }
	  }
	  while(!ColScan());
    }
    buf[i]='\0';
	return 0;
}

void ReadNum2(u32 *num,u8 *lastKey)
{
	while(1)
	{
		*lastKey=KeyScan();
		if((*lastKey>='0') && (*lastKey<='9'))
		{
			*num=(*num*10)+(*lastKey-'0');
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(*num);
			while(ColScan()==0);
		}
		else
		{
			if(*lastKey=='#')
			{
				CmdLCD(CLEAR_LCD);
			}
			while(ColScan()==0);
			break;
		}
	}
}
