#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define MAX_PROMPT_SIZE 100 //FIXME Put into header file
#define OS_BOOT_TIME 5
#define L_TIME 1

void bootloader();
int  check_address(unsigned int a);

#endif
