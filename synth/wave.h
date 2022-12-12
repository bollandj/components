
#ifndef WAVE_H_
#define WAVE_H_

#include <stdint.h>

typedef volatile struct wave wave_t; // forward declaration

typedef void (*wave_func_t)(wave_t *);

struct wave
{
    uint16_t value;
    int32_t acc;

    uint8_t out_bits;
    uint32_t out_mask;
    uint8_t acc_bits;
    uint32_t acc_mask;

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
    WAVE_TYPE_SH,
    NUM_WAVE_TYPES
} wave_type_t;

void wave_init(wave_t *wave, uint8_t num_out_bits, uint8_t num_acc_bits);

static inline void wave_update(wave_t *wave) {wave->func(wave);};
static inline uint16_t wave_get_value(wave_t *wave) {return wave->value;};
static inline void wave_reset(wave_t *wave) {wave->acc = 0;};

static inline void wave_set_freq(wave_t *wave, uint32_t freq) {wave->freq = freq;};
void wave_set_type(wave_t *wave, wave_type_t type);

#endif /* WAVE_H_ */
