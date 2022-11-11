
#ifndef OSC_H_
#define OSC_H_

#include <stdint.h>

#define OSC_OUT_BITS 10 // 1 - 16
#define OSC_OUT_MASK ((1<<OSC_OUT_BITS) - 1)

#define OSC_ACC_BITS 24 // 1 - 32
#define OSC_ACC_MASK ((1<<OSC_ACC_BITS) - 1)

typedef volatile struct _osc_t osc_t; // forward declaration

typedef void (*oscfunc_t)(osc_t *);

struct _osc_t
{
  // internal accumulator
  int32_t acc;

  // output
  uint16_t value;

  // user parameters
  oscfunc_t func;

  int32_t freq;
};

#define OSC_DEFAULT 	\
{              			\
    .acc = 0,        	\
    .value = 0,      	\
    .func = osc_saw,	\
    .freq = 0     		\
}

#define NUM_OSC_FUNCS 4

void osc_saw(osc_t *osc);
void osc_sqr(osc_t *osc);
void osc_noise(osc_t *osc);
void osc_tri(osc_t *osc);

#endif /* OSC_H_ */
