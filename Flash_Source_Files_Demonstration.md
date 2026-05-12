# FLASH SOURCE FILES EXPLANATION
# LPC2148 FLASH PASSWORD STORAGE IMPLEMENTATION

---

# 1. INTRODUCTION

This document explains the complete implementation of flash-based password storage used in the LPC2148 project.

## Files Used

1. `flash.h`
2. `flash.c`

## Purpose

- Store password permanently in internal flash memory
- Retrieve password after restart/power OFF
- Avoid password reset after reboot

---

# 2. WHY FLASH MEMORY IS REQUIRED

RAM is volatile memory.

This means:

- Data stored in RAM is lost after power OFF
- Variables reset after restart

Example:

```c
char stored_pass[8] = "1234";
```

This password exists only during runtime.

After power OFF:

- RAM clears
- Password resets to default

To solve this issue:

Internal FLASH memory is used.

FLASH memory is non-volatile memory.

Data remains stored even after:

- Power OFF
- Restart
- Reset
- Adapter removal

---

# 3. LPC2148 FLASH MEMORY OVERVIEW

LPC2148 contains:

```text
512 KB internal flash memory
```

Flash memory is divided into sectors.

Each sector:

- Has fixed address
- Has fixed size
- Can be erased independently

---

# 4. FLASH SECTOR DETAILS

| Sector | Start Address | Size |
|---|---|---|
| 0 | 0x00000000 | 4 KB |
| 1 | 0x00001000 | 4 KB |
| 2 | 0x00002000 | 4 KB |
| 3 | 0x00003000 | 4 KB |
| 4 | 0x00004000 | 4 KB |
| 5 | 0x00005000 | 4 KB |
| 6 | 0x00006000 | 4 KB |
| 7 | 0x00007000 | 4 KB |
| 8 | 0x00008000 | 32 KB |
| 9 | 0x00010000 | 32 KB |
| 10 | 0x00018000 | 32 KB |
| 11 | 0x00020000 | 32 KB |
| 12 | 0x00028000 | 32 KB |
| 13 | 0x00030000 | 32 KB |
| 14 | 0x00038000 | 32 KB |
| 15 | 0x00040000 | 32 KB |

---

# 5. WHY SECTOR 14 IS USED

Sector 14 is selected because:

- Application code size is small
- Sector usually remains unused
- Safe for user data storage
- Prevents overlap with application code

Selected Address:

```text
0x0003C000
```

---

# 6. flash.h FILE EXPLANATION

## Purpose

- Contains macros
- Structure definitions
- Function declarations

---

## FLASH_SECTOR

```c
#define FLASH_SECTOR 14
```

Defines flash sector used for storage.

---

## FLASH_ADDR

```c
#define FLASH_ADDR 0x0003C000
```

Defines flash memory address used for password storage.

---

## STRUCTURE

```c
typedef struct
{
    u8  pass[8];

    u32 checksum;

    u8  dummy[244];

}flash_t;
```

## Purpose

Stores password data.

Future expansion possible for:

- Alarm settings
- User preferences
- RTC configuration

---

## FUNCTION DECLARATIONS

```c
void flash_write_password(char *pass);
```

Used to write password into flash.

---

```c
void flash_read_password(char *pass);
```

Used to read password from flash.

---

# 7. flash.c FILE EXPLANATION

## Purpose

Contains complete flash operation logic.

---

# 8. IAP MECHANISM

IAP means:

```text
In Application Programming
```

LPC2148 uses internal ROM functions for:

- Flash erase
- Flash write
- Sector prepare
- RAM to flash copy

without external programmer.

---

# 9. IAP ENTRY ADDRESS

```c
#define IAP_LOCATION 0x7FFFFFF1
```

This is LPC2148 ROM IAP entry address.

---

# 10. FUNCTION POINTER

```c
typedef void (*IAP)(unsigned int[], unsigned int[]);
```

```c
IAP iap_entry = (IAP)IAP_LOCATION;
```

## Purpose

Used to call ROM-based IAP functions.

---

## WHAT IS A FUNCTION POINTER?

A function pointer stores the address of a function.

Example:

```c
int *ptr;
```

stores address of integer variable.

Similarly:

```c
void (*func_ptr)();
```

stores address of a function.

---

## BREAKDOWN OF THE STATEMENT

```c
typedef void (*IAP)(unsigned int[], unsigned int[]);
```

### STEP 1

```c
(*IAP)
```

Means:

```text
IAP is a pointer to a function
```

---

### STEP 2

```c
(unsigned int[], unsigned int[])
```

Means:

The function accepts two unsigned integer arrays.

These arrays are:

- Command Array
- Result Array

---

### STEP 3

```c
void
```

Means:

```text
Function returns nothing
```

---

## FINAL MEANING

```c
typedef void (*IAP)(unsigned int[], unsigned int[]);
```

defines a datatype for a function pointer that:

- returns void
- accepts two unsigned integer arrays

---

# 11. WHY FUNCTION POINTER IS REQUIRED

LPC2148 stores flash programming functions inside internal ROM.

These ROM functions are called:

- IAP routines
- In Application Programming routines

ROM IAP routine exists at fixed address:

```c
#define IAP_LOCATION 0x7FFFFFF1
```

To execute ROM IAP function, we must call it using function pointer.

---

# 12. FUNCTION POINTER CREATION

```c
IAP iap_entry = (IAP)IAP_LOCATION;
```

## Explanation

- `IAP` -> function pointer datatype
- `iap_entry` -> function pointer variable
- `IAP_LOCATION` -> ROM function address

This means:

```text
iap_entry now points to LPC2148 internal ROM IAP function
```

---

# 13. VISUAL REPRESENTATION

```text
iap_entry
    |
    |-----> 0x7FFFFFF1
                  |
                  |
            LPC2148 ROM
            IAP FUNCTIONS
```

---

# 14. HOW IT IS USED IN FLASH SOURCE FILE

After creating function pointer:

```c
IAP iap_entry = (IAP)IAP_LOCATION;
```

we can call ROM IAP functions like normal C functions.

Example:

```c
iap_entry(command,result);
```

This internally jumps to:

```text
0x7FFFFFF1
```

and executes ROM flash programming routine.

---

# 15. COMMAND ARRAY PURPOSE

Example:

```c
command[0] = 50;
```

Command 50 means:

```text
Prepare Sector For Write
```

---

```c
command[0] = 52;
```

Command 52 means:

```text
Erase Sector
```

---

```c
command[0] = 51;
```

Command 51 means:

```text
Copy RAM To Flash
```

These command values are defined by LPC2148 IAP specification.

---

# 16. RESULT ARRAY PURPOSE

```c
unsigned int result[5];
```

ROM IAP function stores:

- Status codes
- Operation result
- Success/failure information

inside result array.

Example:

```text
result[0] = 0
```

means:

```text
Command Successful
```

---

# 17. FLASH BUFFER

```c
__align(256) static flash_t data;
```

## Purpose

Creates aligned RAM buffer.

## Why aligned?

LPC2148 requires:

```text
256-byte aligned RAM buffer for flash writing
```

Without alignment:

- Flash write fails
- Data corruption may occur

---

# 18. flash_write_password() EXPLANATION

## Purpose

Stores password permanently into flash memory.

---

## STEP 1: CLEAR BUFFER

```c
memset(&flash_buffer,0,sizeof(flash_t));
```

Clears previous data.

---

## STEP 2: COPY PASSWORD

```c
strcpy(flash_buffer.pass,pass);
```

Copies password into RAM buffer.

---

## STEP 3: PREPARE SECTOR

```c
command[0] = 50;
```

Prepare sector command.

Sector must be prepared before:

- Erase
- Write

---

## STEP 4: ERASE SECTOR

```c
command[0] = 52;
```

Erases complete flash sector.

Flash memory cannot overwrite existing data directly.

Must erase before write.

---

## STEP 5: PREPARE AGAIN

Sector must again be prepared before writing.

---

## STEP 6: COPY RAM TO FLASH

```c
command[0] = 51;
```

Copies aligned RAM buffer into flash memory.

Password is now permanently stored.

---

# 19. flash_read_password() EXPLANATION

## Purpose

Reads password from flash memory.

---

## POINTER CREATION

```c
flash_t *ptr;

ptr = (flash_t *)FLASH_ADDR;
```

Maps structure directly to flash memory location.

---

## EMPTY FLASH CHECK

```c
if(ptr->pass[0] == 0xFF)
```

Fresh erased flash contains:

```text
0xFF
```

This detects:

- First boot
- Empty flash

---

## DEFAULT PASSWORD INITIALIZATION

```c
strcpy(pass,"1234");

flash_write_password("1234");
```

Initializes default password automatically.

---

## NORMAL PASSWORD LOAD

```c
strcpy(pass,ptr->pass);
```

Reads previously stored password.

---

# 20. rtc_test.c MODIFICATIONS

## OLD

```c
char stored_pass[8] = "1234";
```

## NEW

```c
char stored_pass[8];
```

## Reason

Password now loads from flash memory.

---

Added before while(1):

```c
flash_read_password(stored_pass);
```

Purpose:

```text
Loads stored password during startup
```

---

# 21. change_password() MODIFICATION

Added:

```c
flash_write_password(stored_pass);
```

after successful password change.

Purpose:

```text
Stores updated password permanently
```

---

# 22. PASSWORD FLOW

```text
System Startup
      ↓
Read Password From Flash
      ↓
Password Verification
      ↓
User Changes Password
      ↓
Write New Password To Flash
      ↓
Power OFF
      ↓
Power ON
      ↓
Previous Password Retrieved
```

---

# 23. ADVANTAGES

- Password retained after power OFF
- No external EEPROM required
- Runtime password update possible
- Uses internal flash memory
- Persistent storage supported

---

# 24. LIMITATIONS

- Flash erase cycles are limited
- Entire sector erased during update
- Full MCU reflashing may erase password

---

# 25. FINAL RESULT

The LPC2148 project now supports:

- Persistent password storage
- Flash memory operations
- Runtime password modification
- Password recovery after restart
- Embedded flash management using IAP
