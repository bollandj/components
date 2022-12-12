
#include "wave.h"

#include <assert.h>

static void wave_saw_up(wave_t *wave)
{
    wave->value = wave->acc >> (wave->acc_bits - wave->out_bits);

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;  
}

static void wave_saw_down(wave_t *wave)
{
    wave->value = wave->out_mask - (wave->acc >> (wave->acc_bits - wave->out_bits));

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask; 
}

static void wave_sqr(wave_t *wave)
{
    wave->value = (int32_t)(wave->acc << (32 - wave->acc_bits)) >> 31 & wave->out_mask;

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;
}

static void wave_tri(wave_t *wave)
{
    int32_t temp, mask;

    temp = wave->acc << (32 - wave->acc_bits);
    mask = temp >> 31;

    wave->value = (uint32_t)(temp ^ mask) >> (31 - wave->out_bits);

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;
}

static void wave_sh(wave_t *wave)
{
    static int32_t acc_prev = 0;
    static uint32_t x = 0xDEADBEEF;

    if (wave->acc < acc_prev)
    {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        wave->value = x & wave->out_mask;
    }

    acc_prev = wave->acc;

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;
}

static wave_func_t wave_funcs[NUM_WAVE_TYPES] = 
{
    [WAVE_TYPE_SAW_UP] = wave_saw_up,
    [WAVE_TYPE_SAW_DOWN] = wave_saw_down,
    [WAVE_TYPE_SQR] = wave_sqr,
    [WAVE_TYPE_TRI] = wave_tri,
    [WAVE_TYPE_SH] = wave_sh,	
};

// static_assert(sizeof(wave_funcs)/sizeof(wave_func_t) == NUM_WAVE_TYPES)

void wave_init(wave_t *wave, uint8_t num_out_bits, uint8_t num_acc_bits)
{
    wave->acc = 0;
    wave->value = 0;

    wave->out_bits = num_out_bits;
    wave->out_mask = (1 << num_out_bits) - 1;
    wave->acc_bits = num_acc_bits;
    wave->acc_mask = (1 << num_acc_bits) - 1;
    
    wave->func = wave_funcs[0];
    wave->freq = 0;
}

void wave_set_type(wave_t *wave, wave_type_t type)
{
    assert(type < NUM_WAVE_TYPES);
    wave->func = wave_funcs[type];
};