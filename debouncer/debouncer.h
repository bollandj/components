
#ifndef DEBOUNCER_H_
#define DEBOUNCER_H_

#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCER_LENGTH 4

typedef struct debouncer_t_
{
    uint32_t port_states[DEBOUNCER_LENGTH];

    uint32_t port_debounced;
    uint32_t port_debounced_prev;

    uint32_t port_held;

    uint32_t port_mon_mask; // 0 = ignore bit, 1 = monitor bit
    uint32_t port_inv_mask; // 0 = active high, 1 = active low

    unsigned int hold_timer;
    unsigned int hold_timeout;

    int idx;  
} debouncer_t;

void debouncer_init(debouncer_t *debouncer);
void debouncer_set_mask(debouncer_t *debouncer, uint32_t mask);
void debouncer_set_inv_mask(debouncer_t *debouncer, uint32_t inv_mask);

void debouncer_update(debouncer_t *debouncer, uint32_t port_state_raw);

uint32_t debouncer_get_port_state(debouncer_t *debouncer);
bool debouncer_get_pin_state(debouncer_t *debouncer, uint32_t pin_no);

#endif /* DEBOUNCER_H_ */
