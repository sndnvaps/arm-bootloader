#ifndef TIMERS_H
#define TIMERS_H

#ifndef PCONE
#define PCONE (*((volatile unsigned int*) (0x01d20028)))
#endif

#define TCFG0 (*((volatile unsigned int*)(0x01d50000)))
#define TCFG1 (*((volatile unsigned int*)(0x01d50004)))
#define TCON (*((volatile unsigned int*)(0x01d50008)))
#define TCNTB0 (*((volatile unsigned int*)(0x01d5000c)))
#define TCNTO0 (*((volatile unsigned int*)(0x01d50014)))

#define TCNTB3 (*((volatile unsigned int*)(0x01d50030)))
#define TCNTO3 (*((volatile unsigned int*)(0x01d50038)))
#define TCMPB3 (*((volatile unsigned int*)(0x01D50034)))

void timer0_initialize(int ms);
void timer0_start();
void timer0_stop();
void timer3_initialize(float hz);
void timer3_start();
void timer3_stop();
void timer_start(int nb);
void timer_stop(int nb);
void wait_ms(int ms);

#endif
