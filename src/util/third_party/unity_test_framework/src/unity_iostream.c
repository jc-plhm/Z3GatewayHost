#include "sl_iostream.h"

void unity_iostream_putchar(char c)
{
  sl_iostream_putchar(SL_IOSTREAM_STDOUT, c);
}
