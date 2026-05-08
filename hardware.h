// hardware.h


// ---------------- GPIO PINS ----------------

// Buzzer
#define BUZZER_PIN 7  // P0.7

// Menu/Edit switch
#define EDIT_PIN          (1<<22)   // P0.22 (menu switch)

// Alarm stop switch
#define ALARM_STOP_PIN    (1<<23)   // P0.23 (new switch)

// ACTIVE LOW switches
#define EDIT_SWITCH        (!(IOPIN0 & EDIT_PIN))

#define ALARM_STOP_SWITCH  ((IOPIN0 & ALARM_STOP_PIN))
