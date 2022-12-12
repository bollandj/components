
#include "midi.h"

void midi_init(midi_handler_t *midi)
{
    midi->rx_ptr = 0;
    midi->channel = 0;
    midi->omni_mode = 0;
}

bool midi_process_byte(midi_handler_t *midi, midi_message_t *message, uint8_t data)
{
    /* This function returns true when a complete MIDI message has been received, */
    /* unless it is unimplemented, not directed to our channel etc. */

    /* rx_buf stores the content of a MIDI message as follows: */
    /* rx_buf[0] - status byte */
    /* rx_buf[1] - data byte 1 */
    /* rx_buf[2] - data byte 2 */

    /* Check for System Real-Time messages first */
    /* These are a special case of MIDI message that only consist of 1 byte and can be received at any time */
    /* (including in the middle of another MIDI message) */
    /* Don't need to store in rx_buf or increment rx_ptr */
    if (MIDI_IS_SYSTEM_REAL_TIME(data))
    {
        switch(data)
        {
        /* Timing Clock */
        /* rx_buf[0] = 0xF8 */
        case MIDI_TIMING_CLOCK:

        /* Start */
        /* rx_buf[0] = 0xFA */
        case MIDI_START:

        /* Continue */
        /* rx_buf[0] = 0xFB */
        case MIDI_CONTINUE:

        /* Stop */
        /* rx_buf[0] = 0xFC */
        case MIDI_STOP:

        /* Active Sensing */
        /* rx_buf[0] = 0xFE */
        case MIDI_ACTIVE_SENSING:

        /* Reset */
        /* rx_buf[0] = 0xFF */
        case MIDI_RESET:
            message->status = data;
            message->data_0 = 0xFF;
            message->data_1 = 0xFF;
            return true;

        /* Reserved/invalid */
        /* rx_buf[0] = 0xF? */
        default:
            return false;
        }
    }

    /* All other message types start here */

    /* Check for repeat messages (Running Status): */
    /* control bytes may only be sent once to save bandwidth */
    if (midi->rx_ptr == 0 && MIDI_IS_DATA(data)) midi->rx_ptr++;

    /* Now write into buffer */
    midi->rx_buf[midi->rx_ptr] = data;

    /* System Common/Exclusive Messages */
    /* rx_buf[0] & 0xF0 = 0xF0 */
    /* Don't need to filter channel regardless of Omni mode */
    if (MIDI_IS_SYSTEM_MESSAGE(midi->rx_buf[0]))
    {
        switch (midi->rx_buf[0])
        {
        /* Song Position Pointer */
        /* rx_buf[0] = 0xF2 */
        /* rx_buf[1] = song position (LSB) */
        /* rx_buf[2] = song position (MSB) */
        case MIDI_SONG_POSITION_POINTER:
            if (midi->rx_ptr == 2)
            {
                message->status = midi->rx_buf[0];
                message->data_0 = midi->rx_buf[1];
                message->data_1 = midi->rx_buf[2];
                midi->rx_ptr = 0;
                return true;
            }
            midi->rx_ptr++;
            return false;

        /* System Exclusive */
        /* rx_buf[0] = 0xF0 */
        /* rx_buf[1] = data */
        case MIDI_SYSTEM_EXCLUSIVE:

        /* MIDI Time Code Quarter Frame */
        /* rx_buf[0] = 0xF1 */
        /* rx_buf[1] = time code ([6:4] = message type, [3:0] = values) */
        case MIDI_TIME_CODE_QUARTER_FRAME:

        /* Song Select */
        /* rx_buf[0] = 0xF3 */
        /* rx_buf[1] = song number (0-127) */
        case MIDI_SONG_SELECT:
            if (midi->rx_ptr == 1)
            {
                /* Don't need to do anything special for System Exclusive messages - the Running Status */
                /* check just results in a new message being dispatched for each new byte of SysEx data */
                message->status = midi->rx_buf[0];
                message->data_0 = midi->rx_buf[1];
                message->data_1 = 0xFF;
                midi->rx_ptr = 0;
                return true;
            }
            midi->rx_ptr++;
            return false;

        /* Tune Request */
        /* rx_buf[0] = 0xF6 */
        case MIDI_TUNE_REQUEST:

        /* End of Exclusive */
        /* rx_buf[0] = 0xF7 */
        case MIDI_END_OF_EXCLUSIVE:
            message->status = midi->rx_buf[0];
            message->data_0 = 0xFF;
            message->data_1 = 0xFF;
            return true;

        /* Reserved/invalid */
        /* rx_buf[0] = 0xF? */
        default:
            midi->rx_ptr = 0;
            return false;
        }
    }

    /* Channel Voice/Mode Messages */
    switch (midi->rx_buf[0] & 0xF0)
    {
    /* Note Off Event */
    /* rx_buf[0] = 0x8[channel] */
    /* rx_buf[1] = note number (0-127) */
    /* rx_buf[2] = note velocity (0-127) */
    case MIDI_NOTE_OFF:

    /* Note On Event */
    /* rx_buf[0] = 0x9[channel] */
    /* rx_buf[1] = note number (0-127) */
    /* rx_buf[2] = note velocity (0-127) */
    case MIDI_NOTE_ON:

    /* Polyphonic Key Pressure */
    /* rx_buf[0] = 0xA[channel] */
    /* rx_buf[1] = note number (0-127) */
    /* rx_buf[2] = pressure value (0-127) */
    case MIDI_POLYPHONIC_KEY_PRESSURE:

    /* Control Change and Channel Mode */
    /* rx_buf[0] = 0xB[channel] */
    /* rx_buf[1] = controller number (0-119) or channel mode (120-127) */
    /* rx_buf[2] = controller value (0-127) */
    case MIDI_CONTROL_CHANGE:

    /* Pitch Bend Change */
    /* rx_buf[0] = 0xE[channel] */
    /* rx_buf[1] = pitch bend value (LSB) */
    /* rx_buf[2] = pitch bend value (MSB) */
    case MIDI_PITCH_BEND_CHANGE:
        if (midi->rx_ptr == 2)
        {
            message->status = midi->rx_buf[0];
            message->data_0 = midi->rx_buf[1];
            message->data_1 = midi->rx_buf[2];
            midi->rx_ptr = 0;
            return true;
        }
        midi->rx_ptr++;
        return false;

    /* Program Change */
    /* rx_buf[0] = 0xC[channel] */
    /* rx_buf[1] = program number (0-127) */
    case MIDI_PROGRAM_CHANGE:

    /* Channel Pressure */
    /* rx_buf[0] = 0xD[channel] */
    /* rx_buf[1] = pressure value (0-127) */
    case MIDI_CHANNEL_PRESSURE:
        if (midi->rx_ptr == 1)
        {
            message->status = midi->rx_buf[0];
            message->data_0 = midi->rx_buf[1];
            message->data_1 = 0xFF;
            midi->rx_ptr = 0;
            return true;
        }
        midi->rx_ptr++;
        return false;

    /* Reserved/invalid */
    /* rx_buf[0] = ? */
    default:
        midi->rx_ptr = 0;
        return false;
    }

    return false;
}


