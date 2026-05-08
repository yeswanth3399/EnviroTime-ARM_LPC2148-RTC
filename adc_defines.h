//adc_defines.h
#define FOSC 		12000000
#define CCLK 		(FOSC*5)
#define PCLK		(CCLK/4)
#define ADC_CLK	3000000

//defines for ADCR
#define CLKDIV_BITS 	8//@p0.8 - p0.15
#define PDN_BIT 			21
#define ADC_CONV_START_BIT 	24

//define for ADDR
#define DIGITAL_DATA_BITS		6//@p0.6-p0.15
#define DONE_BIT 						31
#define CLKDIV				((PCLK/ADC_CLK)-1)
#define AIN0_FUNC			0x00400000
#define AIN1_FUNC 		0x15440000
#define AIN2_FUNC			0x15440000
#define AIN3_FUNC			0x15440000
#define AIN4_FUNC     0x15440000
