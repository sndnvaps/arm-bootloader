#include "printf.h"

void uart_init()
{
  /* Initialize the UART */
	
  ULCON0 = 0x3;
  // ULCON0 page 263
  // [6]=0 Normal (no infra red)
  // [5:3]=000 No Parity
  // [2]=0 1 bit de stop
  // [1:0]=11 8 bits
	
  ULCON0 = 0x3;
  UCON0 =   0x5;
  //UCON0 page 264
  //[3:2]=10 BDMA0 request (Only for UART0)
  //[1:0]=10 BDMA0 request (Only for UART0)

  UBRDIV0 = 0x23;
}

void _putc(char c)
{

  while(! (UTRSTAT0 & 0x2));
  UTXH0 = c;
}

void _getc(char* c)
{
  uart_init();
  while(! (UTRSTAT0 & 0x1));
  *c = URXH0;
}

void _puts(char* s)
{
  unsigned int i = 0;
  while(*(s+i) != 0)
  {
    _putc( *(s + i) );
    i++;
  }
}

void _printf(char* s)
{
  uart_init();
  _puts(s);
}

void printf_ln(char* s)
{
  uart_init();
  _puts(s);
  print_ln();
}

void print_ln()
{
  _puts("\n\r");
}

void print_int(int n)
{
  int div = 1;
  int nb = 0;
	
  if( n == 0 ) _putc(48);
  else
  {
    while(n / div != 0)
    {
      nb++;
      div *= 10;
    } 
		
    while(div/10 != 0)
    {
      _putc(48 + (n - (n / div) * div) / (div / 10) );
      div /= 10;
    }
  } 
}

void print_hex(long n)
{
  int nb = 0;
  int i = 0;

  _printf("0x");	

  for(i = 0; i<8; i++)
  {
    nb = ((n>>((7 - i) * 4)) & 0xF);
    if(nb < 10)
      _putc(48 + nb);
    else
      _putc(65 + nb - 10);
  }
}
