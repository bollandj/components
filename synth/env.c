
#include "env.h"

#include <assert.h>

static void env_lin(env_t *env)
{
    switch (env->state)
    {
        case ENV_ATT:
            env->acc += env->attack;
            if (env->acc >= ENV_ACC_MASK)
            {
                env->acc = ENV_ACC_MASK;
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
    }

    env->value = env->acc >> (ENV_ACC_BITS - ENV_OUT_BITS);
}

void env_exp(env_t *env)
{
    int32_t diff;

    switch (env->state)
    {
        case ENV_ATT:
            diff = (ENV_ACC_MASK + (ENV_ACC_MASK >> ENV_EXP_THRESHOLD) - env->acc) >> (ENV_ACC_BITS - 16);
            env->acc += (diff * env->attack) >> 14;
            if (env->acc >= ENV_ACC_MASK)
            {
                env->acc = ENV_ACC_MASK;
                env->state = ENV_DEC;
            }
            break;

        case ENV_DEC:
            diff = (env->acc - (env->sustain + (env->sustain >> ENV_EXP_THRESHOLD))) >> (ENV_ACC_BITS - 16);
            env->acc -= (diff * env->decay) >> 14;
            if (env->acc < env->sustain)
                env->state = ENV_SUS;
            break;

        case ENV_SUS:
            break;

        case ENV_REL:
            diff = (env->acc + (ENV_ACC_MASK >> ENV_EXP_THRESHOLD)) >> (ENV_ACC_BITS - 16);
            env->acc -= (diff * env->release) >> 14;
            if (env->acc < 0)
            {
                env->state = ENV_OFF;
                env->acc = 0u;
            }
            break;

        default:
            env->acc = 0u;
    }

    env->value = env->acc >> (ENV_ACC_BITS - ENV_OUT_BITS);
}

static env_func_t env_funcs[NUM_ENV_TYPES] =
{
    env_lin,
    env_exp
};

// static_assert(sizeof(env_funcs)/sizeof(env_func_t) == NUM_ENV_TYPES)

void env_set_type(env_t *env, env_type_t type)
{
    assert(type < NUM_ENV_TYPES);
    env->func = env_funcs[type];
};


