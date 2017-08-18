#include "game.h"

unsigned long
strlen(const char* s)
{
  int32_t count = 0;
  while (*s++ != 0) {
    count++;
  }

  return count;
}


char *
strcat(char *dest, const char *src)
{
  strcpy(&dest[strlen(dest)], src);
  return dest;
}


char *
itoa(int32_t i)
{
  static char buf[12];
  char *p = buf + 11;
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  } else {
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}


char *
itoan(int32_t i, int16_t nChars)
{
  static volatile char *buf = "000000000000";

  for (int16_t n = 0; n < 12; n++) {
    buf[n] = '0';
  }

  volatile char *p = buf + 12;
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
  } else {
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }

  //  USE(nChars);
  return (char *)buf + (12-nChars);
}



char *
strcpy(char *dest, const char *src)
{
  char *s = dest;
  while ((*s++ = *src++) != 0);
  return dest;
}

char *
strncpy(char *s1, const char *s2, size_t n)
{
  char *s = s1;
  while (n > 0 && *s2 != '\0') {
    *s++ = *s2++;
    --n;
  }
  while (n > 0) {
    *s++ = '\0';
    --n;
  }
  return s1;
}

static char _text_hex[] = {'A', 'B', 'C', 'D', 'E', 'F'};
static char _text_buf[9];

static inline
int32_t _hexChar(int16_t s)
{
  int32_t c;

  c = (s >= 0 && s <= 9) ? s + '0' : _text_hex[s - 10];
  return c;
}


char*
itoh(uint32_t n, uint16_t numChars)
{
  uint32_t c;
  char* ptr = &_text_buf[numChars];
  *ptr = 0;
  ptr--;
  for (c = 1; c <= numChars; c++) {
    *ptr = _hexChar(n & 0xf);
    ptr--;
    n = n >> 4;
  }

  return _text_buf;
}
