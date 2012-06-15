#ifndef LEDS_H
#define LEDS_H

#define PCONC (*((volatile unsigned int*)(0x01D20010)))
#ifndef PCONE
#define PCONE (*((volatile unsigned int*)(0x01d20028)))
#endif
#define PDATC (*((volatile unsigned int*)(0x01D20014)))
#define PDATE (*((volatile unsigned int*)(0x01D2002C)))

void led_status(int led, int val);
void led_grid(int val1, int val2, int val3, int val4);
void led_grid_hex(int val);
void k2000(unsigned int times);

#endif
