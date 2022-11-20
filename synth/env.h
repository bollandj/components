
#ifndef ENV_H_
#define ENV_H_

#include <stdint.h>

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

    uint8_t out_bits;
    uint32_t out_mask;
    uint8_t acc_bits;
    uint32_t acc_mask;

    /* User parameters */
    env_func_t func;
    uint16_t attack;  // 0 - 65535
    uint16_t decay;   // 0 - 65535
    uint32_t sustain; // 0 - ENV_ACC_MASK
    uint16_t release; // 0 - 65535
};

typedef enum 
{
    ENV_TYPE_LIN=0,
    ENV_TYPE_EXP,
    NUM_ENV_TYPES
} env_type_t;

void env_init(env_t *env, uint8_t num_out_bits, uint8_t num_acc_bits);

inline void env_update(env_t *env) {env->func(env);};
inline uint16_t env_get_value(env_t *env) {return env->value;};

inline void env_set_attack(env_t *env, uint16_t attack) {env->attack = attack;};
inline void env_set_decay(env_t *env, uint16_t decay) {env->decay = decay;};
inline void env_set_sustain(env_t *env, uint32_t sustain) {env->sustain = sustain;};
inline void env_set_release(env_t *env, uint16_t release) {env->release = release;};
void env_set_type(env_t *env, env_type_t type);

#endif /* ENV_H_ */
