
#include "wave.h"

#include <assert.h>

static void wave_saw_up(wave_t *wave)
{
    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;

    wave->value = wave->acc >> (wave->acc_bits - wave->out_bits);
}

static void wave_saw_down(wave_t *wave)
{
    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;

    wave->value = wave->out_mask - (wave->acc >> (wave->acc_bits - wave->out_bits));
}

static void wave_sqr(wave_t *wave)
{
    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;

    wave->value = (int32_t)(wave->acc << (32 - wave->acc_bits)) >> 31 & wave->out_mask;
}

static void wave_tri(wave_t *wave)
{
    int32_t temp, mask;

    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;

    temp = wave->acc << (32 - wave->acc_bits);
    mask = temp >> 31;

    wave->value = (uint32_t)(temp ^ mask) >> (31 - wave->out_bits);
}

static void wave_noise(wave_t *wave)
{
    /* TODO: use xorshift or similar instead */
    /* 32-bit LFSR */
    static uint32_t lfsr = 0x12345678;
    uint32_t lsb;

    lsb = lfsr & 1u;
    lfsr >>= 1;
    lfsr ^= (-lsb) & 0xA3000000; // taps 32, 30, 26, 25

    wave->value = ((int32_t)lfsr >> 31) & wave->out_mask;
}

static void wave_sh(wave_t *wave)
{
    //32-bit LFSR
    static uint32_t lfsr = 0x12345678;
    uint32_t lsb;

    lsb = lfsr & 1u;
    lfsr >>= 1;
    lfsr ^= (-lsb) & 0xA3000000; // taps 32, 30, 26, 25

    // 16-bit LFSR
    //static uint16_t lfsr = 0x1234;
    //uint16_t lsb;

    //lsb = lfsr & 1u;
    //lfsr >>= 1;
    //lfsr ^= (-lsb) & 0xB400; // taps 16, 14, 13, 11

    int32_t acc_prev = wave->acc;

    // increment acc.
    wave->acc += wave->freq;
    wave->acc &= wave->acc_mask;

    // new sample on overflow
    if (wave->acc < acc_prev)
        wave->value = lfsr & wave->out_mask;
}

static wave_func_t wave_funcs[NUM_WAVE_TYPES] = 
{
    wave_saw_up,
    wave_saw_down,
    wave_sqr,
    wave_tri,
    wave_noise,
    wave_sh,	
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