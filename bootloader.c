#include "bootloader.h"
#include "printf.h"
#include "timers.h"
#include "sound.h"
#include "fibonnaci.h"
#include "leds.h"

void bootloader();
void L(unsigned int _add);
void G(unsigned int add);
void R(unsigned int add);
void get_hex(char* hex);
void get_hex_timeout(char* hex);
void parse_command(char *cmd);
unsigned int parse_hex(char *string, int *begin);

extern char os_start, ram_start, ram_end, _bend;
static int  error_flag = 0; //FIXME mettre en extern 

void bootloader()
{
  char prompt[MAX_PROMPT_SIZE];
  int first_time = 1;
  int prompt_position, only_spaces_appeared;
  char c;

  printf_ln("ARM7 Bootloader V0.0b. (Clement MOUSSU)");
  printf_ln("You have 5. sec before the OS boot...");
  _printf("You are allowed to access memory between ");
  print_hex((unsigned int) &_bend);
  _printf(" and ");
  print_hex((unsigned int) &ram_end);
  print_ln();
  printf_ln("Serious commands are L, G or R.");  
  printf_ln("Funny commands are M, K or F.");  
  print_ln();  
  
  while(1)
  {
    int i;
    for(i = 0; i < MAX_PROMPT_SIZE; i++) prompt[i]=0;
    _printf("armsh# ");
    
    prompt_position = 0;
    only_spaces_appeared = 1;
    
    if( first_time)
    {
      timer0_initialize(OS_BOOT_TIME * 1000);
      timer0_start();
      uart_init();	

      while(! (UTRSTAT0 & 0x1) && TCNTO0 != 0);
      timer0_stop(); //FIXME useful ?
      if( TCNTO0 == 0 )
      { 
        print_ln(); 
        printf_ln("Timeout 5 sec. Booting OS..."); 
        print_ln(); 
        ((void (*)())&os_start)();
        return;
      }
      c = URXH0;
      if( c != '\n' && c != '\r' && c != 8  ) _putc(c);
    }
    else
    {
      _getc(&c);
      if( c != '\n' && c != '\r'  && c != 8 ) _putc(c);
    }
    if( c != '\n' && c != '\r'  && c != 8 ) prompt[prompt_position++] = c;

    while( prompt_position < 100 && c != '\r' )
    {
      _getc(&c);
      prompt[prompt_position++] = c;
      if( (c != '\n' && c != '\r' && c != 8) || ( prompt_position >1 && c == 8 ) ) _putc(c);
      if( c == 8 && prompt_position > 1) prompt_position-=2;
      else if( c == 8 ) prompt_position = 0;
    }
    
    if( prompt_position < 100 )
    {
      prompt[prompt_position] = 0; // \n \r
      print_ln();
      if(prompt_position != 0)
        parse_command(prompt);
    }
    else
    {
      printf_ln("Prompt overflow: no more than 100 chars please !");
    }
    
    if( first_time ) first_time = 0;
  }
}

void parse_command(char *cmd)
{
  unsigned int add;
  int count = 0;
  while( (cmd[count] == ' ' || cmd[count] == 0) && count < MAX_PROMPT_SIZE) count ++;
  if( count == MAX_PROMPT_SIZE ) return;
  if((cmd[count] == 'L' || cmd[count] == 'l') && cmd[count + 1] == ' ')
  {
    count += 2;
    add = parse_hex(cmd, &count);
    if( !error_flag )
    {
      L(add);
    }
    else
    {
      error_flag = 0;
    }
  }
  else if((cmd[count] == 'G' || cmd[count] == 'g') && cmd[count + 1] == ' ')
  {
    count += 2;
    add = parse_hex(cmd, &count);
    if( !error_flag )
    {
      G(add);
    }
    else
    {
      error_flag = 0;
    }
  }
  else if((cmd[count] == 'R' || cmd[count] == 'r') && cmd[count + 1] == ' ')
  {
    count += 2;
    add = parse_hex(cmd, &count);
    if( !error_flag )
    {
      R(add);
    }
    else
    {
      error_flag = 0;
    }
  }
  else if((cmd[count] == 'M' || cmd[count] == 'm'))
  {
    playmusic("A4R8A8c6A6G6F2E2A4R8A8c6A6G6F6G6F6E2");
  }
  else if((cmd[count] == 'K' || cmd[count] == 'k'))
  {
    k2000(4);
  }
  else if((cmd[count] == 'F' || cmd[count] == 'f'))
  {
    _printf("Fibonnaci ");
    unsigned int i;
    for( i = 0; i< 11; i++)
    {
      print_int(fibonnaci(i));
      _printf(" ");
    }
    print_ln();
  }
 
  else
  {
    _printf("Unknown command: ");
    printf_ln(cmd);
  }
}

int check_address(unsigned int a)
{
  if( (a < (unsigned int)&ram_end)
      && (a >= (unsigned int)&ram_start) 
      && (a > (unsigned int)&_bend)
      && ((a % 4) == 0))
  {
    return 1;
  }
  return 0;
}

void get_hex(char* hex)
{
  int count = 0;
  char c;

  do
  {
    _getc(&c);
    _putc(c);
  }while( c == ' ' );
	  
  hex[count++] = c;

  do
  {	  
    _getc(&c);
    _putc(c);
    hex[count++] = c;
  } while (c != '\r' && c != ' ');
  hex[count -1] = 0;
}

void get_hex_timeout(char* hex)
{
  int count = 0;
  char c;

  do
  {	  
    uart_init();
    while(! (UTRSTAT0 & 0x1) && TCNTO0 != 0);
    timer0_stop(); //FIXME useful ?
    if( TCNTO0 == 0 )
    { 
      error_flag = 1;
      return;
    }
    c = URXH0;
    _putc(c);
    hex[count++] = c;
  } while (c != '\r' && c != ' ' && count < 50);

  if( count == 50 )
  {
    error_flag = 1;
    return;
  }

  print_ln();
  hex[count - 1] = 0;
  error_flag = 0;
}

unsigned int parse_hex(char *string, int* begin)
{
  int count = *begin;
  unsigned int value = 0;
  char c;

  error_flag = 0;
  do
  {
    c = string[count++];
    //_printf("PARSE_CHAR(");_putc(c);printf_ln(")");
  } while(c == ' ');
    
  if( c != '0' )
  {
    printf_ln("Invalid hex number: values begin by 0x !");
    error_flag = 1;
    return 0;
  }

  c = string[count++];    
  //_printf("PARSE_CHAR(");_putc(c);printf_ln(")");
  if( c != 'x' && c != 'X')
  {
    printf_ln("Invalid hex number: values begin by 0x !");
    error_flag = 1;
    return 0;
  }
  unsigned int i;
  for(i = 0; i<8; i++)
  {
    c = string[count++];
    //_printf("PARSE_CHAR(");_putc(c);printf_ln(")");
    if( c >= '0' && c <= '9')
    {
      value += (c - '0') * (1<<(4*(7-i)));
    }
    else if(  c >= 'a' && c <= 'f' )
    {
      value += (c - 'a' + 10) * (1<<(4*(7-i)));
    }
    else if(  c >= 'A' && c <= 'F' )
    {
      value += (c - 'A' + 10) * (1<<(4*(7-i)));
    }
    else
    {
      printf_ln("Invalid hex number: only use 0123456789abcdef !");
      error_flag = 1;
      return 0;
    }
  }
  c = string[count];
  if( c == 0 || c == ' ' || c == '\r' || c == '\n' ) 
  {
    *begin = count;
    return value;    
  }
  printf_ln("Invalid hex number: put a space after an hex number !");
  error_flag = 1;
  return 0;    
}

void L(unsigned int _add)
{
  unsigned int add = _add;
  unsigned int value;
  char value_string[50];
  if( check_address(add) )
  {
    do
    {
      timer0_stop(); //FIXME Place in get_hex_timeout
      timer0_initialize(L_TIME * 1000);
      timer0_start();
        
      get_hex_timeout(value_string);
      if( error_flag ) 
      {
        error_flag = 0;
        printf_ln("L ended: timeout (1s).");
        break;
      }
      int count = 0;
      value = parse_hex(value_string, &count);
      if( !error_flag )
      {
        (*((volatile unsigned int*) (add))) = value;
        add+=4;
      }
      else
      {
        timer0_stop();
        error_flag = 1;
        printf_ln("L failed: Invalid value !");
        print_ln();
        return;
      }
        
    }while(TCNTO0 != 0);
    
    timer0_stop();

    _printf("L success: ");
    print_int((add - _add)/4);
    printf_ln(" values written.");
    return;
  }
  else
  {
    _printf("L failed: Invalid address (addresses end by 0, 4, 8 C) !");
    print_hex(_add);
    _printf(" !");
    print_ln();
    return;
  }
}

void G(unsigned int add)
{
  ((void (*)())(unsigned int)add)(); //FIXME verifier addresse ?
}

void R(unsigned int add)
{
  if( check_address(add) )
  {
    print_hex((*((volatile unsigned int*) (add))));
    print_ln();
  }
  else
  {
    printf_ln("R failed: Invalid address (adresses end by 0, 4, 8, C) !");
  }
}
