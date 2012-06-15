#include "init.h"

inline void set_reg(unsigned long addr, unsigned long val)
{
  (* ((unsigned long*) addr)) =  val;
}

void init_data_bss()
{
  extern char _etext, _data, _edata, _bstart, _bend;
  char *src = &_etext;
  char *dst = &_data;
  /* ROM has data at end of text; copy it. */
  while (dst < &_edata)
    *dst++ = *src++;

  /* Zero bss */
  for (dst = &_bstart; dst< &_bend; dst++)
    *dst = 0;
}
