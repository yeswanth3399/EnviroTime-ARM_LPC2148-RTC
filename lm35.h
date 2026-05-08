//lm35.h
#ifndef LM35_H
#define LM35_H
#include "types.h"
void Read_LM35(u8* tDegC,u8* tDegF);
void Read_LM35NP(s32* tDegC,s32* tDegF);
#endif
