//kpm.h
#ifndef KPM_H
#define KPM_H
#include "types.h"

#define TIMEOUT_NUM 0xFFFFFFFF
#define TIMEOUT_STR	0xFF

void InitKPM(void);
u8 ColScan(void);
u8 RowCheck(void);
u8 ColCheck(void);
u8 KeyScan(void);
u32 ReadNumTimeout(u32 timeout_ms);
void ReadNum2(u32 *, u8*);
u8 read_str_timeout(char *, u8, u32);
#endif
