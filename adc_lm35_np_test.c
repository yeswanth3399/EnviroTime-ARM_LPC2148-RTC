//adc_lm35_NP_test.c

#include "lcd_defines.h"
#include "lcd.h"
#include "delays.h"
#include "lm35.h"

int main()
{
	f32 tDegC,tDegF;
	InitLCD();
	StrLCD("LM35 Test:");
	while(1)
	{
		Read_LM35NP(&tDegC,&tDegF);
		CmdLCD(GOTO_LINE2_POS0);
		F32LCD(tDegC,2);
		StrLCD("dC ");
		F32LCD(tDegF,2);
		StrLCD("dF  ");
		delay_ms(100);
	}
}
