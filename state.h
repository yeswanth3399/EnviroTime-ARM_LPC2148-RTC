#ifndef STATE_H
#define STATE_H

typedef enum{
    STATE_NORMAL = 0,
    STATE_PASSWORD,
    STATE_MENU,
    STATE_EDIT_TIME,
    STATE_SET_ALARM,
    STATE_CHANGE_PASSWORD,
    STATE_ALARM_RING
} state_t;

extern state_t state;

#endif
