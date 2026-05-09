# EnviroTime: Digital Clock with Real-Time Temperature Monitoring

---

## Project Overview

EnviroTime is a real-time embedded system developed using the LPC2148 ARM7 microcontroller.

The system continuously displays:

- Digital Clock Information
- Real-Time Temperature Monitoring
- Multi-Alarm Management

The project integrates:

- LPC2148 Internal RTC
- LM35 Temperature Sensor
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Multi-Alarm Functionality
- Flash-Based Password Storage
- Password Protected Editing
- Timer0 Based Hardware Delays
- Buzzer Indication System

The system allows secure user-controlled modification of:

- RTC Time and Date
- Alarm Settings
- Password Settings

through keypad authentication and menu navigation.

---

# Aim

To design and develop a real-time embedded system using the LPC2148 microcontroller that displays accurate digital clock information along with continuous ambient temperature monitoring while providing secure password-protected editing, persistent flash-based password storage, and multi-alarm scheduling functionality.

---

# Objectives

- Implement RTC for accurate display of:
  - Time
  - Date
  - Day
  - Month
  - Year

- Monitor ambient temperature using LM35 through ADC

- Display RTC and temperature information on 16x2 LCD

- Implement multi-alarm functionality

- Store password permanently in LPC2148 internal flash memory

- Provide password-protected menu access

- Generate buzzer indication during alarm events

- Implement timeout-based embedded UI handling

---

# Hardware Requirements

| Component | Description |
|-----------|-------------|
| LPC2148 | ARM7 Microcontroller |
| 16x2 LCD | HD44780 Compatible LCD |
| 4x4 Matrix Keypad | User Input |
| LM35 | Temperature Sensor |
| Buzzer | Alarm Indication |
| Push Switches | Menu & Alarm Stop |
| USB-UART / DB9 Cable | Programming Interface |

---

# Software Requirements

- Embedded C
- Keil uVision
- Flash Magic
- Proteus Simulation

---

# Block Diagram

```text
                 +----------------+
                 |    LPC2148     |
                 +----------------+
                    |    |    |
         ------------    |    ------------
         |               |               |
      Keypad           LCD            Buzzer
         |
      Switches
         |
        RTC
         |
        ADC
         |
       LM35
```

---

# LCD Display Format

```text
HH:MM:SS  T:28°C
DD/MM/YYYY DAY
```

---

# Alarm Features

- Support for storing up to 5 alarms
- Software-managed multi-alarm architecture
- Alarm browsing using keypad navigation
- Alarm enable/disable functionality
- Alarm editing support
- Alarm deletion support
- Exact 30-second alarm ringing
- Active HIGH buzzer support
- Automatic snooze after 5 minutes
- Maximum snooze count: 5 times
- Manual alarm stop switch support
- Timeout-based alarm menu handling

---

# Security Features

## Password Protected Editing

Before allowing modifications:

- User must enter password through keypad
- Password validated against stored password
- Password masking using '*' implemented
- Backspace support implemented using D key
- Timeout-based password entry implemented

---

## Flash-Based Password Storage

The system stores the latest changed password inside LPC2148 internal flash memory.

This allows:

- Password retention after restart
- Password retention after complete power OFF
- Runtime password modification
- Automatic password retrieval during startup

Flash implementation uses:

- LPC2148 Internal Flash Memory
- IAP (In-Application Programming)
- Dedicated flash sector storage

### Default Password

```text
1234
```

---

# Editable Parameters

After successful authentication, user can modify:

- RTC Time
- RTC Date
- RTC Day
- Alarm Time
- Password

---

# RTC Features

## RTC Display

Displays:

- Hour
- Minute
- Second
- Date
- Month
- Year
- Day

## RTC Editing

Validation implemented for:

| Parameter | Valid Range |
|-----------|-------------|
| Hour | 0-23 |
| Minute | 0-59 |
| Second | 0-59 |
| Date | 1-31 |
| Month | 1-12 |
| Day | 0-6 |

---

# Temperature Monitoring

- LM35 sensor interfaced through ADC
- Real-time temperature displayed on LCD
- Temperature shown in degree Celsius

---

# Enhanced Keypad Features

- 4x4 Matrix keypad interface
- Backspace support using D key
- '#' key used as ENTER
- Password masking using '*'
- Maximum digit protection implemented
- Input timeout handling implemented
- Key release synchronization implemented
- Debounce handling implemented
- Empty input validation implemented

---

# Timer0 Delay System

Software delays replaced with Timer0 hardware delay implementation.

Functions used:

```c
void tdelay_us(u32 us);
void tdelay_ms(u32 ms);
void tdelay_s(u32 sec);
```

### Advantages

- Accurate delays
- Stable LCD timing
- Better keypad response
- Reliable alarm timing

---

# Timeout-Based UI Handling

Timeout handling implemented across multiple menus.

If user remains inactive for predefined duration:

- Current menu automatically exits
- System returns to previous screen

Timeout support implemented in:

- RTC Editing
- Alarm Configuration
- Password Entry
- Menu Navigation

Advantages:

- Prevents UI lockup
- Improves embedded user experience
- Handles unattended operation safely

---

# Pin Configuration

## LCD Connections

| LCD Pin | LPC2148 Pin |
|----------|--------------|
| D0-D7 | P0.8 - P0.15 |
| RS | P0.16 |
| RW | P0.17 |
| EN | P0.18 |

---

## Keypad Connections

| Keypad | LPC2148 |
|---------|----------|
| Rows | P1.16 - P1.19 |
| Columns | P1.20 - P1.23 |

---

## Alarm & Switch Connections

| Device | LPC2148 Pin |
|----------|--------------|
| Buzzer | P0.7 |
| Edit/Menu Switch | P0.22 |
| Alarm Stop Switch | P0.23 |

---

# Software Architecture

## State Machine Based Design

The project follows modular embedded design.

### States Used

| State | Purpose |
|--------|----------|
| STATE_NORMAL | RTC Display Mode |
| STATE_PASSWORD | Password Verification |
| STATE_MENU | Menu Handling |
| STATE_EDIT_TIME | RTC Editing |
| STATE_SET_ALARM | Alarm Configuration |
| STATE_CHANGE_PASSWORD | Password Modification |
| STATE_ALARM_RING | Alarm Ringing State |

---

# Flash Memory Implementation

The project uses LPC2148 internal flash memory for persistent password storage.

Features:

- Password retained after power OFF
- Password retained after restart
- Automatic password loading during boot
- Runtime password updates supported

Implementation files:

- flash.c
- flash.h

Flash operations implemented using:

- LPC2148 IAP (In-Application Programming)
- Internal flash sector management

Selected Flash Sector:

- Sector 14

Flash Storage Address:

```text
0x0003C000
```

---

# Project Structure

```text
Project
│
├── rtc_test.c
├── rtc.c
├── rtc.h
├── rtc_edit.c
├── rtc_edit.h
├── alarm.c
├── alarm.h
├── alarm_ring.c
├── alarm_ring.h
├── flash.c
├── flash.h
├── lcd.c
├── lcd.h
├── kpm.c
├── kpm.h
├── menu.c
├── menu.h
├── normal.c
├── normal.h
├── timer0_delay.c
├── timer0_delay.h
├── lm35.c
├── lm35.h
├── hardware.h
├── state.h
└── types.h
```

---

# Applications

Useful in:

- Homes
- Offices
- Laboratories
- Hospitals
- Industrial Monitoring Systems

where both timekeeping and environmental monitoring are important.

---

# Future Improvements

Possible future enhancements:

- External EEPROM support
- UART debugging support
- 12-hour clock mode
- Melody alarm tones
- External RTC module support
- Interrupt-based keypad handling
- SD card logging
- Bluetooth connectivity
- GSM alert integration
- Mobile app synchronization

---

# Development Notes

The project follows:

- Modular embedded programming practices
- Structured source/header separation
- Peripheral abstraction
- Hardware timer based delay handling
- State-machine architecture
- Flash memory management using LPC2148 IAP
- Software-managed multi-alarm architecture
- Timeout-based embedded UI handling
- Persistent password storage implementation

---

# Author

Developed using LPC2148 ARM7 Microcontroller for embedded systems learning and real-time environmental monitoring applications.

---

# License

This project is open-source and intended for educational and embedded systems learning purposes.
