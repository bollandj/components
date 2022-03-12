
#ifndef MIDI_DEFS_H_
#define MIDI_DEFS_H_

/* Channel Voice/Mode Messages */
#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define CONTROL_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0

/* System Common Messages */
#define SYSTEM_EXCLUSIVE 0xF0
#define SONG_POSITION_POINTER 0xF2
#define SONG_SELECT 0xF3
#define END_OF_EXCLUSIVE 0xF7

/* System Real-Time Messages */
#define TIMING_CLOCK 0xF8
#define START 0xFA
#define CONTINUE 0xFB
#define STOP 0xFC
#define ACTIVE_SENSE 0xFE
#define RESET 0xFF

/* Macros */
#define IS_COMMAND(x) (x & 0x80)
#define IS_DATA(x)    (!(x & 0x80))

#define IS_SYSTEM_REAL_TIME(x) ((x & 0xF8) == 0xF8) // sys RT messages occupy 0xF8 - 0xFF
#define IS_SYSTEM_MESSAGE(x)   ((x & 0xF0) == 0xF0)

#define MIDI_OMNI_OFF 0
#define MIDI_OMNI_ON  1

#define MIDI_CHANNEL_1 0
#define MIDI_CHANNEL_2 1

#endif /* MIDI_DEFS_H_ */
