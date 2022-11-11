
#include "encoder.h"

#include <stddef.h>

void encoder_init(encoder_t *encoder)
{
    encoder->count = 0;
    encoder->a_prev = 0;
    encoder->b_prev = 0;
}

void encoder_update(encoder_t *encoder, uint8_t a, uint8_t b)
{
	if (a != encoder->a_prev)
	{
		if  (b != a)
		{	// CW rotation 
			encoder->count++;	
		}
		else
		{	// CCW rotation 
			encoder->count--;	
		}
	}

	encoder->a_prev = a;
    encoder->b_prev = b;
}