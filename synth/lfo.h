
#ifndef LFO_H_
#define LFO_H_

#include <stdint.h>

#define LFO_OUT_BITS 12 // 1 - 15
#define LFO_OUT_MASK ((1<<LFO_OUT_BITS) - 1)

#define LFO_ACC_BITS 16 // 1 - 31
#define LFO_ACC_MASK ((1<<LFO_ACC_BITS) - 1)

typedef volatile struct _lfo_t lfo_t; // forward declaration

typedef void (*lfofunc_t)(lfo_t *);

struct _lfo_t
{
  // internal accumulator
  int32_t acc;

  // output
  int16_t value;

  // user parameters
  lfofunc_t func;

  uint16_t freq;
};

#define LFO_DEFAULT     \
{              		    \
    .acc = 0,           \
    .value = 0,         \
    .func = lfo_tri,    \
    .freq = 64          \
}

#define NUM_LFO_FUNCS 4

void lfo_saw_up(lfo_t *lfo);
//void lfo_saw_down(lfo_t *lfo);
void lfo_sqr(lfo_t *lfo);
void lfo_sh(lfo_t *lfo);
void lfo_tri(lfo_t *lfo);

#endif /* LFO_H_ */
