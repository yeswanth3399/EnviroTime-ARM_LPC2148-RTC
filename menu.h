// menu.h
#ifndef MENU_H
#define MENU_H
#include "state.h"
state_t menu_handler(void);
int password_check(char *stored);
void change_password(char *stored);

#endif
