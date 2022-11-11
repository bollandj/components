
#ifndef DEBOUNCER_H_
#define DEBOUNCER_H_

#include <stdint.h>

#define DEBOUNCER_LENGTH 4

typedef struct debouncer_t_
{
    uint32_t port_states[DEBOUNCER_LENGTH];
    uint32_t port_debounced;
    uint8_t idx;
    uint32_t r_prev;
    uint32_t f_prev;
} debouncer_t;

void debouncer_init(debouncer_t *debouncer);
void debouncer_update(debouncer_t *debouncer, uint32_t port_state_raw);

uint32_t debouncer_get_port_rising(debouncer_t *debouncer);
uint32_t debouncer_get_port_falling(debouncer_t *debouncer);
uint8_t debouncer_get_pin_rising(debouncer_t *debouncer, uint32_t pin_no);
uint8_t debouncer_get_pin_falling(debouncer_t *debouncer, uint32_t pin_no);

#endif /* DEBOUNCER_H_ */
