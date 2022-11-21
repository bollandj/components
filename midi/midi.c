
#include "midi.h"

void midi_init(midi_handler_t *midi)
{	
    midi->rx_ptr = 0; 
    midi->channel = 0;
    midi->omni_mode = 0;
}

/* TODO: can add midi->rx_ptr = 0; and just return false? */
/* Alternatively, replace this macro with a goto jump */
#define MIDI_DISPATCH(s, d0, d1) \
do \
{ \
    dispatch = true; \
    message->status = (s); \
    message->data_0 = (d0); \
    message->data_1 = (d1); \
} while (0)
 
bool midi_process_byte(midi_handler_t *midi, midi_message_t *message, uint8_t data)
{
    /* rx_buf stores the content of a MIDI message as follows: */
    /* rx_buf[0] - status byte */
    /* rx_buf[1] - data byte 1 */
    /* rx_buf[2] - data byte 2 */

    /* This function returns true when a complete MIDI message has been received, */
    /* unless it is unimplemented, not directed to our channel etc. */
    bool dispatch = false;
        
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
            MIDI_DISPATCH(midi->rx_buf[0], 0xFF, 0xFF);		
            break;	

        /* Reset */
        /* rx_buf[0] = 0xFF */	
        case MIDI_RESET:
            MIDI_DISPATCH(midi->rx_buf[0], 0xFF, 0xFF);				
            break;

        /* Reserved/undefined/unimplemented */
        /* rx_buf[0] = 0xF? */       
        default:		
            break;
        }
    }
    /* All other message types start here */
    else
    {
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
            /* System Exclusive */
            /* rx_buf[0] = 0xF0 */
            case MIDI_SYSTEM_EXCLUSIVE:
                if (midi->rx_ptr == 1)
                {
                    /* Don't need to do anything else here - a side effect of the Running Status */
                    /* check is that a new message should be dispatched for each new byte of SysEx */
                    /* data, as long as we keep setting rx_ptr to 0 */
                    MIDI_DISPATCH(midi->rx_buf[0], midi->rx_buf[1], 0xFF);
                    midi->rx_ptr = 0;
                }
                else midi->rx_ptr++;
                break;
            /* End of Exclusive */
            /* rx_buf[0] = 0xF7 */
            case MIDI_END_OF_EXCLUSIVE:
                MIDI_DISPATCH(midi->rx_buf[0], 0xFF, 0xFF);
                midi->rx_ptr = 0;
                break;
            /* Ignore other System Messages */
            default:
                midi->rx_ptr = 0;
                break;
            }	
        }
        /* Channel filtering when Omni mode is off */
        else if (midi->omni_mode == MIDI_OMNI_OFF)
        {
            /* Ignore messages not sent to our channel */
            if ((midi->rx_buf[0] & 0x0F) != midi->channel)
            {
                midi->rx_ptr = 0;
                return; 
            }				
        }
        
        /* Channel Voice/Mode Messages */	
        switch (midi->rx_buf[0] & 0xF0)
        {
        /* Note On Event */
        /* rx_buf[0] = 0x9[channel] */
        /* rx_buf[1] = note number */
        /* rx_buf[2] = note velocity */
        case MIDI_NOTE_ON:
            if (midi->rx_ptr == 2)
            {
                /* A Note On with velocity = 0 is equivalent to a Note Off */
                if (midi->rx_buf[2] == 0) 
                {
                    MIDI_DISPATCH(MIDI_NOTE_OFF, midi->rx_buf[1], midi->rx_buf[2]);
                }
                else 
                {
                    MIDI_DISPATCH(midi->rx_buf[0], midi->rx_buf[1], midi->rx_buf[2]);
                }                  

                midi->rx_ptr = 0; // reset rx pointer
            }
            else midi->rx_ptr++;
            break;
            
        /* Note Off Event */
        /* rx_buf[0] = 0x8[channel] */
        /* rx_buf[1] = note number */
        /* rx_buf[2] = note velocity */
        case MIDI_NOTE_OFF:					
            if (midi->rx_ptr == 2)
            {
                MIDI_DISPATCH(midi->rx_buf[0], midi->rx_buf[1], midi->rx_buf[2]);	

                midi->rx_ptr = 0; // reset rx pointer
            }
            else midi->rx_ptr++;
            break;
            
        /* Control Change and Channel Mode */
        /* rx_buf[0] = 0xB[channel] */
        /* rx_buf[1] = controller number (0-119) or channel mode (120-127) */
        /* rx_buf[2] = controller value (0-127) */	
        case MIDI_CONTROL_CHANGE:			
            if (midi->rx_ptr == 2)
            {
                MIDI_DISPATCH(midi->rx_buf[0], midi->rx_buf[1], midi->rx_buf[2]);
                    
                midi->rx_ptr = 0;
            }
            else midi->rx_ptr++;
            break;
            
        /* Program Change */
        /* rx_buf[0] = 0xC[channel] */
        /* rx_buf[1] = program number (0-127) */	
        case MIDI_PROGRAM_CHANGE:		
            if (midi->rx_ptr == 1)
            {	
                MIDI_DISPATCH(midi->rx_buf[0], midi->rx_buf[1], 0xFF);

                midi->rx_ptr = 0;
            }
            else midi->rx_ptr++;
            break;
            
        /* Ignore other control bytes */
        /* rx_buf[0] = anything else */	
        default:	
            midi->rx_ptr = 0;
            break;
        }
    }

    return dispatch;
}


