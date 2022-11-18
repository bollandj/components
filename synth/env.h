
#ifndef ENV_H_
#define ENV_H_

#include <stdint.h>

#define ENV_OUT_BITS 12 // 1 - 16
#define ENV_OUT_MASK ((1<<ENV_OUT_BITS) - 1)

#define ENV_ACC_BITS 20 // 1 - 31
#define ENV_ACC_MASK ((1<<ENV_ACC_BITS) - 1)

#define ENV_EXP_THRESHOLD 4

typedef enum
{
	ENV_OFF = 0,
	ENV_ATT,
	ENV_DEC,
	ENV_SUS,
	ENV_REL
} env_state_t;

typedef volatile struct env env_t; // forward declaration

typedef void (*env_func_t)(env_t *);

struct env
{
	// internal values
	int32_t value;
	int32_t acc;
	env_state_t state;

	// user parameters
	env_func_t func;

	uint16_t attack;  // 0 - 65535
	uint16_t decay;   // 0 - 65535
	uint32_t sustain; // 0 - ENV_ACC_MASK
	uint16_t release; // 0 - 65535
};

#define ENV_DEFAULT		\
{              			\
	.value = 0,      	\
	.acc = 0,         	\
	.state = ENV_OFF,	\
	.func = env_exp,	\
	.attack = 500,   	\
	.decay = 500,       \
	.sustain = (1<<19),	\
	.release = 500   	\
}

#define NUM_ENV_TYPES 2

void env_lin(env_t *env);
void env_exp(env_t *env);

#endif /* ENV_H_ */
