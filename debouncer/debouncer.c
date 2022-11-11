
#include "debouncer.h"

void debouncer_init(debouncer_t *debouncer)
{  
    debouncer->idx = 0;   
}

void debouncer_update(debouncer_t *debouncer, uint32_t port_state_raw)
{
    debouncer->port_states[debouncer->idx++] = port_state_raw;
    if (debouncer->idx >= DEBOUNCER_LENGTH) debouncer->idx = 0; 
}

uint32_t debouncer_get_port_rising(debouncer_t *debouncer)
{
    uint32_t r = 0xFFFFFFFF;
    uint32_t f = 0x00000000;
    for (int i=0; i<DEBOUNCER_LENGTH; i++)
    {
        r &= debouncer->port_states[i];
        f |= debouncer->port_states[i];
    }

    debouncer->port_debounced = r;
    
    debouncer->r_prev = r;
    debouncer->f_prev = f;

    return debouncer->port_debounced;
}

uint32_t debouncer_get_port_falling(debouncer_t *debouncer)
{
    uint32_t r = 0xFFFFFFFF;
    uint32_t f = 0x00000000;
    for (int i=0; i<DEBOUNCER_LENGTH; i++)
    {
        r &= debouncer->port_states[i];
        f |= debouncer->port_states[i];
    }

    debouncer->port_debounced = f;
    
    debouncer->r_prev = r;
    debouncer->f_prev = f;

    return debouncer->port_debounced;
}

uint8_t debouncer_get_pin_rising(debouncer_t *debouncer, uint32_t pin_no)
{  
    return (debouncer_get_port_rising(debouncer) >> pin_no) & 0x00000001;
}

uint8_t debouncer_get_pin_falling(debouncer_t *debouncer, uint32_t pin_no)
{  
    return (debouncer_get_port_falling(debouncer) >> pin_no) & 0x00000001;
}