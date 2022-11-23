
#include "debouncer.h"

void debouncer_init(debouncer_t *debouncer)
{
    debouncer->idx = 0;
    debouncer->port_mon_mask = 0xFFFFFFFF;
    debouncer->port_inv_mask = 0x00000000;
    debouncer->hold_timer = 0;
    debouncer->hold_timeout = 0;
}

void debouncer_set_mask(debouncer_t *debouncer, uint32_t mask)
{
    debouncer->port_mon_mask = mask;
}

void debouncer_set_inv_mask(debouncer_t *debouncer, uint32_t inv_mask)
{
    debouncer->port_inv_mask = inv_mask;
}

void debouncer_update(debouncer_t *debouncer, uint32_t port_state_raw)
{
    port_state_raw ^= debouncer->port_inv_mask;
    port_state_raw &= debouncer->port_mon_mask;

    uint32_t port_state_changed = debouncer->port_states[debouncer->idx] ^ port_state_raw;

    if (port_state_changed || !port_state_raw)
    {
        debouncer->hold_timer = 0;
    }
    else
    {
        if (debouncer->hold_timer == debouncer->hold_timeout)
        {
            debouncer->port_held = port_state_raw;
        }
        else
        {
            debouncer->hold_timer++;
        }
    }

    debouncer->idx++;
    if (debouncer->idx >= DEBOUNCER_LENGTH) debouncer->idx = 0;

    debouncer->port_states[debouncer->idx] = port_state_raw;
}

uint32_t debouncer_get_port(debouncer_t *debouncer)
{
    uint32_t r = 0xFFFFFFFF;
    // uint32_t f = 0x00000000;

    /* A button press will only be registered when all DEBOUNCER_LENGTH port states agree that the button is pressed */
    for (int i=0; i<DEBOUNCER_LENGTH; i++)
    {
        r &= debouncer->port_states[i];
        // f |= debouncer->port_states[i];
    }

    debouncer->port_debounced_prev = debouncer->port_debounced;
    debouncer->port_debounced = r;

    return debouncer->port_debounced;
}

bool debouncer_get_pin(debouncer_t *debouncer, uint32_t pin_no)
{
    return (debouncer_get_port(debouncer) >> pin_no) & 0x00000001;
}
