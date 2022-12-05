
#include "env.h"

#include <assert.h>

static void env_lin(env_t *env)
{
    env->value = env->acc >> (env->acc_bits - env->out_bits);

    switch (env->state)
    {
        case ENV_ATT:
            env->acc += env->attack;
            if (env->acc >= env->acc_mask)
            {
                env->acc = env->acc_mask;
                env->state = ENV_DEC;
            }
            break;

        case ENV_DEC:
            env->acc -= env->decay;
            if (env->acc < env->sustain)
                env->state = ENV_SUS;
            break;

        case ENV_SUS:
            break;

        case ENV_REL:
            env->acc -= env->release;
            if (env->acc < 0)
            {
                env->state = ENV_OFF;
                env->acc = 0u;
            }
            break;

        default:
            env->acc = 0u;
            break;
    }  
}

static void env_exp(env_t *env)
{
    env->value = env->acc >> (env->acc_bits - env->out_bits);
    
    int32_t diff;

    switch (env->state)
    {
        case ENV_ATT:
            diff = (env->acc_mask + (env->acc_mask >> ENV_EXP_THRESHOLD) - env->acc) >> (env->acc_bits - 16);
            env->acc += (diff * env->attack) >> 14;
            if (env->acc >= env->acc_mask)
            {
                env->acc = env->acc_mask;
                env->state = ENV_DEC;
            }
            break;

        case ENV_DEC:
            diff = (env->acc - (env->sustain + (env->sustain >> ENV_EXP_THRESHOLD))) >> (env->acc_bits - 16);
            env->acc -= (diff * env->decay) >> 14;
            if (env->acc < env->sustain)
                env->state = ENV_SUS;
            break;

        case ENV_SUS:
            break;

        case ENV_REL:
            diff = (env->acc + (env->acc_mask >> ENV_EXP_THRESHOLD)) >> (env->acc_bits - 16);
            env->acc -= (diff * env->release) >> 14;
            if (env->acc < 0)
            {
                env->state = ENV_OFF;
                env->acc = 0u;
            }
            break;

        default:
            env->acc = 0u;
            break;
    }
}

static env_func_t env_funcs[NUM_ENV_TYPES] =
{
    [ENV_TYPE_LIN] = env_lin,
    [ENV_TYPE_EXP] = env_exp
};

// static_assert(sizeof(env_funcs)/sizeof(env_func_t) == NUM_ENV_TYPES)

void env_init(env_t *env, uint8_t num_out_bits, uint8_t num_acc_bits)
{
    env->value = 0;
    env->acc = 0;
    env->state = ENV_OFF;

    env->out_bits = num_out_bits;
    env->out_mask = (1 << num_out_bits) - 1;
    env->acc_bits = num_acc_bits;
    env->acc_mask = (1 << num_acc_bits) - 1;

    env->func = env_exp;
    env->attack = 500;
    env->decay = 500;
    env->sustain = (1<<19);
    env->release = 500;
}

void env_set_type(env_t *env, env_type_t type)
{
    assert(type < NUM_ENV_TYPES);
    env->func = env_funcs[type];
}