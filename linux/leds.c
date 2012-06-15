#include "leds.h"
#include "timers.h"

void outputs_initialize();

void outputs_initialize()
{
  //PCONC Datasheet page 225
  //PC3 [7:6] 00 = Input  01 = Output
  //          10 = DATA19 11 = IISCLK
  //PC2 [5:4] 00 = Input  01 = Output
  //          10 = DATA18 11 = IISDI
  //PC1 [3:2] 00 = Input  01 = Output
  //          10 = DATA17 11 = IISDO
  PCONC |= (0x1<<2);
  PCONC |= (0x1<<4);
  PCONC |= (0x1<<6);

  PCONC &= ~(0x2<<2);
  PCONC &= ~(0x2<<4);
  PCONC &= ~(0x2<<6);

  //PCONE Datasheet page 228
  //PE5 [11:10] 00 = Input 01 = Output
  //            10 = TOUT2 11 = TCLK in
  PCONE |= (0x1<<10);
  PCONE &= ~(0x2<<10);

  int i = 0;
  for( i = 0; i < 4; i++) led_status(i + 1, 0);
}

void led_status(int led, int val)
{
  if( val )
  {
    switch(led)
    {
      case 1:
        PDATC |= (0x1<<1);
        break;
      case 2:
        PDATC |= (0x1<<2);
        break;
      case 3:
        PDATC |= (0x1<<3);
        break;
      case 4:
        PDATE |= (0x1<<5);
        break;
    }
  }
  else
  {
    switch(led)
    {
      case 1:
        PDATC &= ~(0x1<<1);
        break;
      case 2:
        PDATC &= ~(0x1<<2);
        break;
      case 3:
        PDATC &= ~(0x1<<3);
        break;
      case 4:
        PDATE &= ~(0x1<<5);
        break;
    }
  }
}

void led_grid(int val1, int val2, int val3, int val4)
{
  led_status(1, val1);
  led_status(2, val2);
  led_status(3, val3);
  led_status(4, val4);
}

void led_grid_hex(int val)
{
  led_grid(val & 0x1, (val & 0x2)>>1, (val & 0x4)>>2, (val & 0x8)>>3);
}

void k2000(unsigned int times)
{
  int speed = 1;
  const unsigned int low = 200;
  const unsigned int high = 500;
  int i = 0;

  if(times == 0) i = -1;

  outputs_initialize();
  while(i < (int)times)
  {
    if(speed) wait_ms(low);
    else wait_ms(high);
		
    led_grid(1,0,0,0);
    if(speed) wait_ms(low);
    else wait_ms(high);
		
    led_grid(0,1,0,0);
    if(speed) wait_ms(low);
    else wait_ms(high);
	
    led_grid(0,0,1,0);
    if(speed) wait_ms(low);
    else wait_ms(high);
		
    led_grid(0,0,0,1);

    speed = ! speed;
    if( times != 0 ) i++;
  }
}

