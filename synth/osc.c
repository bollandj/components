
#include "osc.h"

oscfunc_t osc_funcs[NUM_OSC_FUNCS] = {
	osc_saw,
	osc_sqr,
	osc_noise,
	osc_tri,
};

void osc_saw(osc_t *osc)
{
	// Increment accumulator
	osc->acc += osc->freq;
	osc->acc &= OSC_ACC_MASK;

	osc->value = osc->acc >> (OSC_ACC_BITS - OSC_OUT_BITS);
}

void osc_sqr(osc_t *osc)
{
	// Increment accumulator
	osc->acc += osc->freq;
	osc->acc &= OSC_ACC_MASK;

	osc->value = (int32_t)(osc->acc << (32 - OSC_ACC_BITS)) >> 31 & OSC_OUT_MASK;
}

void osc_noise(osc_t *osc)
{
	// 32-bit LFSR
	static uint32_t lfsr = 0x12345678;
	uint32_t lsb;

	lsb = lfsr & 1u;
	lfsr >>= 1;
	lfsr ^= (-lsb) & 0xA3000000; // taps 32, 30, 26, 25

	osc->value = ((int32_t)lfsr >> 31) & OSC_OUT_MASK;
}

void osc_tri(osc_t *osc)
{
	int32_t temp, mask;

	// Increment accumulator
	osc->acc += osc->freq;
	osc->acc &= OSC_ACC_MASK;

	temp = osc->acc << (32 - OSC_ACC_BITS);
	mask = temp >> 31;

	osc->value = (uint32_t)(temp ^ mask) >> (31 - OSC_OUT_BITS);
}

