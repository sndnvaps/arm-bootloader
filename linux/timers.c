#include "timers.h"
#include "printf.h"
#include "leds.h"

void timer0_initialize(int ms)
{
  TCFG0 |= (0xff); //Prescaler 0 (timers 0 et 1) = 255
	
  TCFG1 &= ~(0x1); //MUX 0 : 0001 -> 1/4
  TCFG1 &= ~(0x1 << 1);
  TCFG1 |= (0x1 << 2);
  TCFG1 &= ~(0x1 << 3);
	
  TCON &= ~(0x1 << 3); //Auto reload OFF
  TCON &= ~(0x1 << 2); //Inverter OFF
  TCON |= (0x1 << 1);     //Manual Update ON
  TCON &= ~(0x1);	      //STOP

  //min_period 0,000124121
  //max_period 8,13
  const float period = 0.124121;
	
  TCNTB0 = (unsigned long) ((float)(ms) / period); //Set the count buffer value
}

void timer0_start()
{
  TCON = (TCON | (0x1)) & (~(0x1 << 1)); //START et Manual Update OFF
}

void timer0_stop()
{
  TCON &= ~(0x1);	      //STOP
}


void timer3_initialize(float hz)
{
  TCFG0 |= (0x0f << 8); //Prescaler 1 (timers 2 et 3) = 255
	
  TCFG1 &= ~(0x1<< 12); //MUX 3 : 0000 -> 1/2
  TCFG1 &= ~(0x1 << 13);
  TCFG1 &= ~(0x1 << 14);
  TCFG1 &= ~(0x1 << 15);
	
  TCON |= (0x1 << 19); //Auto reload ON
  TCON |= (0x1 << 18); //Inverter ON
  TCON |= (0x1 << 17); //Manual Update ON
  TCON &= ~(0x1 << 16); //STOP
	
  //max_frequency = 128906.25;
  //min_period_s = 0.000007757576;
  //min_frequency = 1.96;
  //max_period_s = 0,508392743; //(65535 * min_period_s)
  const float period_ms = 0.007757576;
	
  TCNTB3 =(unsigned long) ((1000.0/hz) / period_ms); //Set the count buffer value
  TCMPB3 = TCNTB3 / 2;
}

void timer3_start()
{
  TCON = (TCON | (0x1 << 16)) & (~(0x1 << 17)); //START et Manual Update OFF
}

void timer3_stop()
{
  TCON &= ~(0x1 << 16); //STOP
}



void timer_start(int nb)
{
  switch(nb)
  {
    case 0:
      timer0_start();
      break;
    case 3:
      timer3_start();
      break;
    default:
      break;
  }
}

void timer_stop(int nb)
{
  switch(nb)
  {
    case 0:
      timer0_stop();
      break;
    case 3:
      timer3_stop();
      break;
    default:
      break;
  }
}

void wait_ms(int ms)
{
  timer0_initialize(ms);
  timer_start(0);
  while(TCNTO0 != 0);
  timer_stop(0);
}

