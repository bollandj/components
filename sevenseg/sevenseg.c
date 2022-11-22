
#include "sevenseg.h"

#include <stdbool.h>

/* Bit order from MSB to LSB is: dp g f e d c b a */
static uint8_t sevenseg_font_num[] =
{
    0x3f, // 0
    0x06, // 1
    0x5b, // 2
    0x4f, // 3
    0x66, // 4
    0x6d, // 5
    0x7d, // 6
    0x07, // 7
    0x7f, // 8
    0x67  // 9
};

/* Mostly based on https://en.wikipedia.org/wiki/Seven-segment_display_character_representations - */
/* the least ambiguous option between uppercase and lowercase was chosen in each case */
/* Bit order from MSB to LSB is: dp g f e d c b a */
static uint8_t sevenseg_font_alpha[] =
{
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00010001, // i
    0b00001110, // J
    0b01110101, // k
    0b00111000, // L
    0b01010101, // m
    0b01010100, // n
    0b01011100, // o
    0b01110011, // p
    0b01100111, // q
    0b01010000, // r
    0b00101101, // S
    0b01111000, // t
    0b00011100, // u
    0b00101010, // V
    0b01101010, // W
    0b01001001, // X
    0b01101110, // y
    0b00011011  // Z	
};

void sevenseg_init(sevenseg_t *sevenseg)
{

}

uint8_t sevenseg_from_char(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c -= 'A';
        return sevenseg_font_alpha[c];
    }
    if (c >= 'a' && c <= 'z')
    {
        c -= 'a';
        return sevenseg_font_alpha[c];
    }   
    if (c >= '0' && c <= '9')
    {
        c -= '0';
        return sevenseg_font_num[c];
    }

    return 0x00;
}

void sevenseg_from_str(uint8_t *sevenseg, const char *string, int len)
{

}

#ifdef DOUBLE_DABBLE

void sevenseg_from_int(uint8_t *sevenseg, int num, int digits)
{
    /* An implementation of the double dabble algorithm */
    uint32_t bcd = 0;

    /* An integer of this width can have at most 8 decimal digits (2^26 = 67108864) */
    const unsigned int NUM_BITS = 26;
    for (int i = 0; i < NUM_BITS; i++)
    {
        /* Add 3 to each nibble which is greater than 4 */
        for (int j = 0; j < 32; j += 4)
            if (((bcd >> j) & 0x0F) > 4) 
                bcd += (3u << j);

        /* Shift entire scratch space */
        bcd <<= 1;
        bcd |= (num >> (NUM_BITS - 1 - i)) & 1;
    }

    for (int i = 28, j = 0; i >= 0; i -= 4, j++)
    {
        uint8_t q = (bcd >> i) & 0x0F;
        sevenseg[j] = sevenseg_font_num[q];
    }
}

#else

void sevenseg_from_int(uint8_t *sevenseg, int num, int digits)
{
    static const uint32_t factors[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, \
                                     10000000, 100000000, 1000000000};

    bool not_leading_zero = false;
    int pos = 0;

    for (int i = digits - 1; i >= 0; i--)
    {
        int quotient = num / factors[i];

        if (quotient != 0)
        {
            if (quotient > 9) quotient = 9; // If this happens num has overflowed the number of decimal digits specified
            sevenseg[digits - 1 - i] = sevenseg_font_num[quotient];
            num -= quotient * factors[i];
            not_leading_zero = true;
        }
        else
        {
            if (not_leading_zero || i == 0)
                sevenseg[digits - 1 - i] = sevenseg_font_num[0];
            else
                sevenseg[digits - 1 - i] = 0x00;
        }

        pos++;
    }
}

#endif