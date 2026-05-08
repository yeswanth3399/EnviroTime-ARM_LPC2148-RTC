//lm35.c
#include <LPC21xx.h>
#include "adc.h"
#include "types.h"

void Read_LM35(u8* tDegC,u8* tDegF)
{
	static s32 flag=0;
	u16 dVAL;
	f32 eAR;
	if(flag==0)
	{
		Init_ADC();
		flag=1;
	}
	Read_ADC(1,&dVAL,&eAR);
	*tDegC=(eAR*100);
	*tDegF=(*tDegC*(9/5.0))+32;
}

void Read_LM35NP(s32* tDegC,s32* tDegF)
{
	static s32 flag=0;
	f32 eAR1,eAR2;
	u16 dVAL1,dVAL2;
	if(flag==0)
	{
		Init_ADC();
		flag=1;
	}
	Read_ADC(1,&dVAL1,&eAR1);
	Read_ADC(2,&dVAL2,&eAR2);
	*tDegC=(s32)(((eAR1-eAR2)*100.0)+0.5);
	// Dead zone near zero
	if((*tDegC > -0.5) && (*tDegC < 0.5))
		*tDegC = 0;
	*tDegF=(s32)(((*tDegC*(9/5.0))+32.0)+0.5);
}
