//adc.c
#include "types.h"
#include <LPC21xx.h>
#include "adc_defines.h"
#include "timer0_delay.h"

void Init_ADC(void)
{
	//cfg pins p0.27-p0.30
	//using to connect block for 
	//AIN0-AIN1 pin functions
	PINSEL1&=(~((u32)0xff<<((u32)((27-16)*2))));
	PINSEL1|=AIN0_FUNC|AIN1_FUNC|AIN2_FUNC|AIN3_FUNC;
	
	//cfg ADCR
	ADCR=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
}

void Read_ADC(u8 chNo,u16* dVal,f32* eAR)
{
	//clear any previous channel selection
	ADCR&=0xffffff00;
	//update on req channel selection
	//start sampling for adc conversion
	ADCR|=(1<<ADC_CONV_START_BIT)|(1<<chNo);
	//wait for conv time
	tdelay_ms(3);
	//check status for conv
	while(((ADDR>>DONE_BIT)&1)==0);
	//stop sampling and conv
	ADCR&=(~(1<<(ADC_CONV_START_BIT)));
	//read / extract digital data
	*dVal=((ADDR>>DIGITAL_DATA_BITS)&1023);
	*eAR=((*dVal)*(3.3/1023));
}
