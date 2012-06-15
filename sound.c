#include "sound.h"
#include "timers.h"
#include "leds.h"

void enable_speaker();

void enable_speaker()
{
  PCONE |=      (0x1<<13);	//E6 -> TOUT3
  PCONE &= ~(0x1<<12);
}

void beep(float frequency, int ms)
{
  enable_speaker();
  timer3_initialize(frequency);
  timer0_initialize(ms);
	
  TCON = ((TCON |  0x10001) & (~0x20002)); //START
	
  while(TCNTO0 != 0);

  timer_stop(3);	
  timer_stop(0);
}

void playmusic(char* s)
{
  unsigned int i = 0;
  float frequency;
  float divisor;
  float beat_period = 60.0 / TEMPO;  

  wait_ms(100);
  while(*(s+i) != 0)
  {
    if((i/2) % 4 == 0) led_grid(1,0,0,0);
    if((i/2) % 4 == 1) led_grid(0,1,0,0);
    if((i/2) % 4 == 2) led_grid(0,0,1,0);
    if((i/2) % 4 == 3) led_grid(0,0,0,1);	
    switch(*(s+i))
    {
      // capitals are notes, non-capitals are flat, except for c wich is C octave
      case 'C':
        frequency = NOTE_C;
        break;
      case 'd':
        frequency = NOTE_Db;
        break;
      case 'D':
        frequency = NOTE_D;
        break;
      case 'e':
        frequency = NOTE_Eb;
        break;
      case 'E':
        frequency = NOTE_E;
        break;
      case 'F':
        frequency = NOTE_F;
        break;
      case 'g':
        frequency = NOTE_Gb;
        break;
      case 'G':
        frequency = NOTE_G;
        break;
      case 'a':
        frequency = NOTE_Ab;
        break;
      case 'A':
        frequency = NOTE_A;
        break;
      case 'b':
        frequency = NOTE_Bb;
        break;
      case 'B':
        frequency = NOTE_B;
        break;
      case 'c':
        frequency = NOTE_Co;
        break;
      default: 
        frequency = 0.0; //rests
        break;
    }
    switch(*(s+i+1))
    {
      case '1': 
      case '2': 
      case '4': 
      case '8':
      case '6': //4th triolet
        divisor = (float) (*(s+i+1) - 48);
        break;
      default:
        divisor = 1.0;
        break;
    }
    
    if( frequency != 0.0)
    {
      beep(frequency, (int)(4000.0 * beat_period / divisor));
    }
    else
    {
      wait_ms((int)(4000.0 * beat_period / divisor));
    }
		    
    i+=2;
  }
}
