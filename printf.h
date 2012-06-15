#ifndef SERIAL_H
#define SERIAL_H

#define UTRSTAT0 (*((volatile unsigned int*)(0x01d00010)))
#define UTXH0 (*((volatile unsigned char*)(0x01d00020)))
#define URXH0 (*((volatile unsigned char*)(0x01d00024)))
#define ULCON0 (*((volatile unsigned int*)(0x01d00000)))
#define UCON0 (*((volatile unsigned int*)(0x01d00004)))
#define UBRDIV0 (*((volatile unsigned int*)(0x01d00028)))

void uart_init();
void _putc(char c);
void _getc(char* c);
void _puts(char* s);
void _printf(char* s);
void printf_ln(char* s);
void print_ln();
void print_int(int n);
void print_hex(long n);

#endif
