#ifndef __STRING_H
#define __STRING_H

int
strlen(char* s);
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

#endif
