
#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

#include "midi_defs.h"

void midi_init(uint8_t channel, uint8_t omni_mode);

void midi_set_channel(uint8_t channel);
void midi_set_omni_mode(uint8_t omni_mode);

uint8_t midi_get_note(void);
uint8_t midi_get_gate(void);

void midi_register_note_on_cb(void (*note_on_cb)(uint8_t note, uint8_t vel));
void midi_register_note_off_cb(void (*note_off_cb)(uint8_t note, uint8_t vel));

void midi_process_byte(uint8_t data);

#endif /* MIDI_H_ */