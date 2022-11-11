
#include "encoder.h"

#include <stddef.h>

void encoder_init(encoder_t *encoder)
{
    encoder->count = 0;
    encoder->a_prev = 0;
    encoder->b_prev = 0;  
	encoder->cw_cb = NULL;
	encoder->ccw_cb = NULL;
}

void encoder_register_cbs(encoder_t *encoder, void (*_cw_cb)(void), void (*_ccw_cb)(void))
{
	encoder->cw_cb = _cw_cb;
	encoder->ccw_cb = _ccw_cb;
}

void encoder_update(encoder_t *encoder, uint8_t a, uint8_t b)
{
	if (a != encoder->a_prev)
	{
		if  (b != a)
		{	// CW rotation 
			encoder->count++;
			if (encoder->cw_cb) encoder->cw_cb();	
		}
		else
		{	// CCW rotation 
			encoder->count--;
			if (encoder->ccw_cb) encoder->ccw_cb();		
		}
	}

	encoder->a_prev = a;
    encoder->b_prev = b;
}