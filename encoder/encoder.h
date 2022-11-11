
#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

typedef struct _encoder_t
{
    unsigned int count;
    uint8_t a_prev;
    uint8_t b_prev;
} encoder_t;

void encoder_init(encoder_t *encoder);
void encoder_update(encoder_t *encoder, uint8_t a, uint8_t b);

inline unsigned int encoder_get_count(encoder_t *encoder) { return encoder->count; }

#endif /* ENCODER_H_ */