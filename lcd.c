//lcd.c
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "delays.h"
#include "lcd_defines.h"

void WriteLCD(u8 bytes)
{
	//select write operation
	IOCLR0=1<<LCD_RW;
	//place any bit on data pins d0 to d7
	WRITEBYTE(IOPIN0, LCD_DATA, bytes);
	//provide high to low pulse for latching
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}
void CmdLCD(u8 cmd)
{
	//select cmd register
	IOCLR0=1<<LCD_RS;
	//write any cmd to LCD
	WriteLCD(cmd);
}
void InitLCD(void)
{
	//cfg lcd connection gpio output pins
	WRITEBYTE(IODIR0, LCD_DATA, 0xff);
	SETBIT(IODIR0, LCD_RS);
	SETBIT(IODIR0, LCD_RW);
	SETBIT(IODIR0, LCD_EN);
	//power on delay
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(4);
	delay_us(100);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_BLINK);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}
void CharLCD(u8 asciiVal)
{
	//select data register
	IOSET0=1<<LCD_RS;
	//write to ddram via data register
	WriteLCD(asciiVal);
}

void StrLCD(s8 *s)
{
	while(*s)
	{
			CharLCD(*s++);
	}
}
void U32LCD(u32 n)
{
	s32 i=0;
	u8 a[10];
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}
		while(i>0)
		{
			CharLCD(a[--i]);
		}
	}
}

void HexLCD(u32 n)
{
	s32 i=0,rem;
	u8 a[16]={0};
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			rem=n%16;
			if(rem<10)
			{
				a[i++]=rem+48;
			}
			else
			{
				a[i++]=(rem-10)+'A';
			}
			n/=16;
		}
		while(i>0)
		{
			CharLCD(a[--i]);
		}
	}
	
}
void OctLCD(u32 n)
{
	s32 i=0,rem;
	u8 a[8]={0};
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			rem=n%8;
			a[i++]=rem+48;
			n/=8;
		}
		while(i>0)
		{
			CharLCD(a[--i]);
		}
	}
}
void BinLCD(u32 n)
{
	s32 i=0,rem;
	u8 a[32]={0};
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			rem=n%2;
			a[i++]=rem+48;
			n/=2;
		}
		while(i>0)
		{
			CharLCD(a[--i]);
		}
	}
}
		

void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);
}

void F32LCD(f32 fn, u8 nDP)
{
	u32 n,i;
	if(fn<0.0)
	{
		CharLCD('-');
		fn=-fn;
	}
	n=(int)fn;
	U32LCD(n);
	CharLCD('.');
	fn=(float)(fn-n);
	for(i=0; i<nDP; i++)
	{
		fn = fn*10.0;
		n = (u32)fn;
		CharLCD(n + '0');
		fn = fn - n;
	}
}

void BuildCGRAM(u8 *p, u8 nBytes)
{
	u32 i;
	CmdLCD(GOTO_CGRAM_START);
	//write data register
	IOSET0=1<<LCD_RS;
	IOCLR0=1<<LCD_RW;
	for(i=0; i<nBytes; i++)
	{
		//write cgram via data register
		WriteLCD(p[i]);
	}
	//return back ddram
	CmdLCD(GOTO_LINE1_POS0);
}
