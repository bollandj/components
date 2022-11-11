
#include "lfo.h"

lfofunc_t lfo_funcs[NUM_LFO_FUNCS] = {
	lfo_saw_up,
	//lfo_saw_down,
	lfo_sqr,
	lfo_sh,
	lfo_tri,
};

void lfo_saw_up(lfo_t *lfo)
{
	// increment acc.
	lfo->acc += lfo->freq;
	lfo->acc &= LFO_ACC_MASK;

	lfo->value = lfo->acc >> (LFO_ACC_BITS - LFO_OUT_BITS);
}

/*

void lfo_saw_down(lfo_t *lfo)
{
	// increment acc.
	lfo->acc += lfo->freq;
	lfo->acc &= LFO_ACC_MASK;

	lfo->value = LFO_OUT_MASK - (lfo->acc >> (LFO_ACC_BITS - LFO_OUT_BITS));
}

*/

void lfo_sqr(lfo_t *lfo)
{
	// increment acc.
	lfo->acc += lfo->freq;
	lfo->acc &= LFO_ACC_MASK;

	lfo->value = (int32_t)(lfo->acc << (32 - LFO_ACC_BITS)) >> 31 & LFO_OUT_MASK;
}

void lfo_sh(lfo_t *lfo)
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

	int32_t acc_prev = lfo->acc;

	// increment acc.
	lfo->acc += lfo->freq;
	lfo->acc &= LFO_ACC_MASK;

	// new sample on overflow
	if (lfo->acc < acc_prev)
		lfo->value = lfsr & LFO_OUT_MASK;
}

void lfo_tri(lfo_t *lfo)
{
	int32_t temp, mask;

	// increment acc.
	lfo->acc += lfo->freq;
	lfo->acc &= LFO_ACC_MASK;

	temp = lfo->acc << (32 - LFO_ACC_BITS);
	mask = temp >> 31;

	lfo->value = (uint32_t)(temp ^ mask) >> (31 - LFO_OUT_BITS);
}

