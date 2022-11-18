
#ifndef WAVE_H_
#define WAVE_H_

#include <stdint.h>

#define WAVE_OUT_BITS 10 // 1 - 16
#define WAVE_OUT_MASK ((1<<WAVE_OUT_BITS) - 1)

#define WAVE_ACC_BITS 24 // 1 - 32
#define WAVE_ACC_MASK ((1<<WAVE_ACC_BITS) - 1)

typedef volatile struct wave wave_t; // forward declaration

typedef void (*wave_func_t)(wave_t *);

struct wave
{
  // internal accumulator
  int32_t acc;

  // output
  uint16_t value;

  // user parameters
  wave_func_t func;

  int32_t freq;
};

#define WAVE_DEFAULT 	\
{              			\
    .acc = 0,        	\
    .value = 0,      	\
    .func = wave_saw,	\
    .freq = 0     		\
}

#define NUM_WAVE_FUNCS 4

void wave_saw(wave_t *wave);
void wave_sqr(wave_t *wave);
void wave_noise(wave_t *wave);
void wave_tri(wave_t *wave);

#endif /* WAVE_H_ */
