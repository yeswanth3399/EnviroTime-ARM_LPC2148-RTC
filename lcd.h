//lcd.h
#ifndef LCD_H
#define LCD_H
#include "types.h"
void WriteLCD(u8 bytes);
void CmdLCD(u8 cmd);
void InitLCD(void);
void CharLCD(u8 asciiVal);
void StrLCD(s8 *);
void U32LCD(u32);
void S32LCD(s32);
void HexLCD(u32);
void OctLCD(u32);
void BinLCD(u32);
void F32LCD(f32,u8);
void BuildCGRAM(u8 *p, u8 nBytes);
#endif
