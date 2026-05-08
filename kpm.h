//kpm.h
#ifndef KPM_H
#define KPM_H
#include "types.h"
void InitKPM(void);
u8 ColScan(void);
u8 RowCheck(void);
u8 ColCheck(void);
u8 KeyScan(void);
u32 ReadNum(void);
void ReadNum2(u32 *, u8*);
#endif
