#include "leds.h"
#include "bootloader.h"

int main(void) 
{
  /* Leds stuffs */		
  k2000(1);
	
  /* bootloader */
  bootloader();
	
  return 0;
}
