
#include "sevenseg.h"

/* from '0' (0x30) to '9' (0x30) */
/* Bit order from MSB to LSB is: dp g f e d c b a */
static uint8_t font_num[] =
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

/* from 'A' (0x41) to 'Z' (0x5A)
/* Based on https://en.wikipedia.org/wiki/Seven-segment_display_character_representations - */
/* the least ambiguous option between uppercase and lowercase was chosen in each case */
static uint8_t font_alpha[] =
{
    0x00,
};

void sevenseg_init(sevenseg_t *sevenseg)
{

}