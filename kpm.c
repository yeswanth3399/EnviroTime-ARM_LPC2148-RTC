//kpm.c
#include <LPC21xx.h>
#include "types.h"
#include "kpm_defines.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "timer0_delay.h"
u8 kpmLUT[4][4]=
{
		{'7','8','9','A'},
		{'4','5','6','B'},
		{'1','2','3','C'},
		{'*','0','#','D'}
};
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

u32 ReadNum(void)
{
	u32 KeyV,sum=0;
	while(1)
	{
		KeyV=KeyScan();
		if((KeyV>='0') && (KeyV<='9'))
		{
			sum=((sum*10)+(KeyV-48));
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(sum);
			while(ColScan()==0);
		}
		else
		{
			if(KeyV=='#')
			{
				CmdLCD(CLEAR_LCD);
			}
			while(ColScan()==0);
			break;
		}
	}
	return sum;
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
