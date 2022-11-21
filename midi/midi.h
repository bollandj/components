
#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>
#include <stdbool.h>

#include "midi_defs.h"

/* Keep track of MIDI state */
typedef struct midi_handler 
{
    uint8_t rx_ptr;
    uint8_t rx_buf[3];

    uint8_t channel;
    uint8_t omni_mode;
    uint8_t mono_mode;
} midi_handler_t;

typedef struct midi 
{
    uint8_t status;
    uint8_t data_0;
    uint8_t data_1;
} midi_message_t;

void midi_init(midi_handler_t *midi);

void midi_set_channel(midi_handler_t *midi, uint8_t channel);
void midi_set_omni_mode(midi_handler_t *midi, uint8_t omni_mode);
void midi_set_mono_mode(midi_handler_t *midi, uint8_t mono_mode);

bool midi_process_byte(midi_handler_t *midi, midi_message_t *message, uint8_t data);

#endif /* MIDI_H_ */