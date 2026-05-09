// flash.c

#include <LPC21xx.h>
#include <string.h>

#include "flash.h"
#include "types.h"

#define IAP_LOCATION 0x7FFFFFF1

typedef void (*IAP)(unsigned int[], unsigned int[]);

static IAP iap = (IAP)IAP_LOCATION;

// Last sector of LPC2148
#define FLASH_SECTOR	14
#define FLASH_ADDR		0x0003C000

#define CCLK_KHZ       60000

// IAP Status Codes
#define CMD_SUCCESS    0

typedef struct
{
    u8  pass[8];

    u32 checksum;

    u8  dummy[244];

}flash_t;

// ----------------------------------------------------
// Checksum function
// ----------------------------------------------------

static u32 cs(char *p)
{
    u32 s = 0;

    s32 i;

    for(i=0; i<8; i++)
    {
        s += p[i];
    }

    return s;
}

// ----------------------------------------------------
// Write Password into Flash
// ----------------------------------------------------

void flash_write_password(char *p)
{
    u32 cmd[5];
    u32 res[5];

    // 256-byte aligned RAM buffer
    __align(256) static flash_t data;

    // Clear structure
    memset(&data,0,sizeof(data));

    // Copy password
    strncpy((char*)data.pass,p,7);

    data.pass[7] = '\0';

    // Generate checksum
    data.checksum = cs((char*)data.pass);

    // Disable IRQ during IAP
    __disable_irq();

    // Disable MAM (important in Proteus sometimes)
    MAMCR = 0;

    // --------------------------------------------
    // PREPARE SECTOR
    // --------------------------------------------

    cmd[0] = 50;
    cmd[1] = FLASH_SECTOR;
    cmd[2] = FLASH_SECTOR;

    iap(cmd,res);

    if(res[0] != CMD_SUCCESS)
        goto exit;

    // --------------------------------------------
    // ERASE SECTOR
    // --------------------------------------------

    cmd[0] = 52;
    cmd[1] = FLASH_SECTOR;
    cmd[2] = FLASH_SECTOR;
    cmd[3] = CCLK_KHZ;

    iap(cmd,res);

    if(res[0] != CMD_SUCCESS)
        goto exit;

    // --------------------------------------------
    // PREPARE AGAIN
    // --------------------------------------------

    cmd[0] = 50;
    cmd[1] = FLASH_SECTOR;
    cmd[2] = FLASH_SECTOR;

    iap(cmd,res);

    if(res[0] != CMD_SUCCESS)
        goto exit;

    // --------------------------------------------
    // COPY RAM TO FLASH
    // --------------------------------------------

    cmd[0] = 51;

    cmd[1] = FLASH_ADDR;

    cmd[2] = (u32)&data;

    cmd[3] = 256;

    cmd[4] = CCLK_KHZ;

    iap(cmd,res);

exit:

    // Enable MAM again
    MAMCR = 2;

    // Enable interrupts
    __enable_irq();
}

// ----------------------------------------------------
// Read Password from Flash
// ----------------------------------------------------

void flash_read_password(char *dest)
{
    flash_t *p = (flash_t*)FLASH_ADDR;

    // Empty flash condition
    if((u8)p->pass[0] == 0xFF)
    {
        strcpy(dest,"1234");

        return;
    }

    // Verify checksum
    if(p->checksum == cs((char*)p->pass))
    {
        strncpy(dest,(char*)p->pass,7);

        dest[7] = '\0';
    }
    else
    {
        strcpy(dest,"1234");
    }
}
