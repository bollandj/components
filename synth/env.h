
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
    ENV_OFF=0,
    ENV_ATT,
    ENV_DEC,
    ENV_SUS,
    ENV_REL
} env_state_t;

typedef volatile struct env env_t; // forward declaration

typedef void (*env_func_t)(env_t *);

struct env
{
    int32_t value;
    int32_t acc;
    env_state_t state;

    /* User parameters */
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

typedef enum 
{
    ENV_TYPE_LIN=0,
    ENV_TYPE_EXP,
    NUM_ENV_TYPES
} env_type_t;

inline void env_update(env_t *env) {env->func(env);};
inline void env_set_attack(env_t *env, uint16_t attack) {env->attack = attack;};
inline void env_set_decay(env_t *env, uint16_t decay) {env->decay = decay;};
inline void env_set_sustain(env_t *env, uint32_t sustain) {env->sustain = sustain;};
inline void env_set_release(env_t *env, uint16_t release) {env->release = release;};
void env_set_type(env_t *env, env_type_t type);

#endif /* ENV_H_ */
