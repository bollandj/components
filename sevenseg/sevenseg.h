#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include <stdint.h>

typedef struct sevenseg
{
    
} sevenseg_t;

void sevenseg_init(sevenseg_t *sevenseg);
uint8_t sevenseg_from_char(char c);
void sevenseg_from_str(uint8_t *sevenseg, const char *string, int len);
void sevenseg_from_int(uint8_t *sevenseg, int num, int digits);

#endif /* SEVENSEG_H_ */

