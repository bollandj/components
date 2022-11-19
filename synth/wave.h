
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
    uint16_t value;
    int32_t acc;

    /* User parameters */
    wave_func_t func;
    int32_t freq;
};

typedef enum 
{
    WAVE_TYPE_SAW_UP=0,
    WAVE_TYPE_SAW_DOWN,
    WAVE_TYPE_SQR,
    WAVE_TYPE_TRI,
    WAVE_TYPE_NOISE,
    WAVE_TYPE_SH,
    NUM_WAVE_TYPES
} wave_type_t;

void wave_init(wave_t *wave);

inline void wave_update(wave_t *wave) {wave->func(wave);};
inline uint16_t wave_get_value(wave_t *wave) {return wave->value;};

inline void wave_set_freq(wave_t *wave, uint32_t freq) {wave->freq = freq;};
void wave_set_type(wave_t *wave, wave_type_t type);

#endif /* WAVE_H_ */