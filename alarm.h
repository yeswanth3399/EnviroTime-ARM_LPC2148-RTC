//alarm.h
#include "types.h"
#define MAX_ALARMS 5

typedef struct
{
    u8 hr;
    u8 min;
    u8 sec;

    u8 enabled;

}alarm_t;

void alarm_set(void);
void alarm_menu(void);
int alarm_check(void);
void alarm_snooze(u32);
void alarm_display(u8);

extern alarm_t alarms[MAX_ALARMS];

extern u8 current_alarm;
