#ifndef __STRING_H
#define __STRING_H

unsigned long
strlen(const char* s);
char*
strcat(char *dest, const char *src);
char*
itoa(int32_t i);
char *
itoan(int32_t i, int16_t nChars);
char*
itoh(uint32_t i, uint16_t numChars);
char *
strcpy(char *dest, const char *src);
char *
strncpy(char *s1, const char *s2, size_t n);
#endif
