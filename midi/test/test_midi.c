#include <stdio.h>

#include "midi.h"
#include "midi_defs.h"

#define ARRAY_LENGTH(x) ((sizeof(x)) / (sizeof(x[0])))

uint8_t midi_data_stream[] = 
{
    MIDI_NOTE_ON,
    43,
    100,
    MIDI_NOTE_OFF,
    43,
    MIDI_TIMING_CLOCK,
    0,
    MIDI_SYSTEM_EXCLUSIVE,
    0x04,
    0x47,
    0x22,
    0x70,
    MIDI_END_OF_EXCLUSIVE,
    MIDI_PITCH_BEND_CHANGE,
    34,
    63,
    MIDI_CONTROL_CHANGE,
    22,
    1,
};

static void midi_process_message(midi_message_t *msg)
{
    printf("Received message 0x%.2X: ", msg->status);

    switch (msg->status)
    {
    /* Channel Voice/Mode Messages */

    case MIDI_NOTE_OFF:
        printf("Note Off\n");
        printf("Note: %u ", msg->data_0);
        printf("Velocity: %u\n", msg->data_1);
        break;

    case MIDI_NOTE_ON:
        printf("Note On\n");
        printf("Note: %u ", msg->data_0);
        printf("Velocity: %u\n", msg->data_1);

        if (msg->data_1 == 0)
        {

        }
        else
        {

        }
        break;

    case MIDI_POLYPHONIC_KEY_PRESSURE:
        printf("Polyphonic Key Pressure\n");
        printf("Note: %u ", msg->data_0);
        printf("Pressure: %u\n", msg->data_1);
        break;

    case MIDI_CONTROL_CHANGE:
        printf("Control Change\n");
        printf("Controller: %u ", msg->data_0);
        printf("Value: %u\n", msg->data_1);
        break;

    case MIDI_PROGRAM_CHANGE:
        printf("Program Change\n");
        printf("Program Number: %u\n", msg->data_0);
        break;

    case MIDI_CHANNEL_PRESSURE:
        printf("Channel Pressure\n");
        printf("Value: %u\n", msg->data_0);
        break;

    case MIDI_PITCH_BEND_CHANGE:
        printf("Pitch Bend Change\n");
        int16_t pb = (int16_t)(msg->data_1 << 7 | msg->data_0) - 0x2000;
        printf("Value: %d\n", pb);
        break;

    /* System Common Messages */

    case MIDI_SYSTEM_EXCLUSIVE:
        printf("System Exclusive\n");
        break;

    case MIDI_TIME_CODE_QUARTER_FRAME:
        printf("MIDI Time Code Quarter Frame\n");
        printf("Message type: 0x%.1X\n", (msg->data_0 >> 4) & 0x07);
        printf("Message values: 0x%.1X\n", msg->data_0 & 0x0F);
        break;

    case MIDI_SONG_POSITION_POINTER:
        printf("Song Position Pointer\n");
        uint16_t sp = msg->data_1 << 7 | msg->data_0;
        printf("Song position: %u\n", sp);
        break;

    case MIDI_SONG_SELECT:
        printf("Song Select\n");
        printf("Song number: %u\n", msg->data_0);
        break;

    case MIDI_TUNE_REQUEST:
        printf("Tune Request\n");
        break;

    case MIDI_END_OF_EXCLUSIVE:
        printf("End of Exclusive\n");
        break;

    /* System Real-Time Messages */

    case MIDI_TIMING_CLOCK:
        printf("Timing Clock\n");
        break;

    case MIDI_START:
        printf("Start\n");
        break;

    case MIDI_CONTINUE:
        printf("Continue\n");
        break;

    case MIDI_STOP:
        printf("Stop\n");
        break;

    case MIDI_ACTIVE_SENSING:
        printf("Active Sensing\n");
        break;

    case MIDI_RESET:
        printf("Reset\n");
        break;

    /* Unimplemented/invalid messages */

    default:
        printf("Unimplemented MIDI message\n");
        break;
    }
}

int main(void)
{
    midi_handler_t midi_handler;
    midi_message_t midi_message;

    midi_init(&midi_handler);

    for (size_t i = 0; i < ARRAY_LENGTH(midi_data_stream); i++)
    {
        if(midi_process_byte(&midi_handler, &midi_message, midi_data_stream[i]))
        {
            midi_process_message(&midi_message);
        }
    }

    return 0;
}