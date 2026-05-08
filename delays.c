//delays.c

#include "types.h"

void delay_us(u32 dlyUS)
{
	for(dlyUS*=12; dlyUS>0; dlyUS--);
}
void delay_ms(u32 dly_MS)
{
	for(dly_MS*=12000; dly_MS>0; dly_MS--);
}
void delay_s(u32 dly_S)
{
	for(dly_S*=12000000; dly_S>0; dly_S--);
}
