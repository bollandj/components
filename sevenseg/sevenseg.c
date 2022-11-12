
#include "sevenseg.h"

/* from '0' (0x30) to '9' (0x30) */
static uint8_t font_num[] =
{
    0x00000000,
};

/* from 'A' (0x41) to 'Z' (0x5A)
/* https://en.wikipedia.org/wiki/Seven-segment_display_character_representations */
static uint8_t font_alpha[] =
{
    0x00000000,
};

void sevenseg_init(sevenseg_t *sevenseg)
{

}