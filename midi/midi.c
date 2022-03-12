
#include "midi.h"

static uint8_t _midi_rx_ptr;
static uint8_t _midi_rx_buf[3];

static uint8_t _midi_channel;
static uint8_t _midi_omni_mode;

static uint8_t _midi_note;
static uint8_t _midi_gate;

static void (*_midi_note_on_cb)(uint8_t note, uint8_t vel);
static void (*_midi_note_off_cb)(uint8_t note, uint8_t vel);

static void _midi_note_on_dummy_cb(uint8_t note, uint8_t vel) {};
static void _midi_note_off_dummy_cb(uint8_t note, uint8_t vel) {};

void midi_init(uint8_t channel, uint8_t omni_mode)
{	
	_midi_rx_ptr = 0;
	
	_midi_channel = channel;		// 0-15 (channels 1-16)
	_midi_omni_mode = omni_mode;	// Omni off by default

	_midi_note_on_cb = _midi_note_on_dummy_cb;
	_midi_note_off_cb = _midi_note_off_dummy_cb;
}

void midi_set_channel(uint8_t channel) 
{
	_midi_channel = channel;
}

void midi_set_omni_mode(uint8_t omni_mode)
{
	_midi_omni_mode = omni_mode;
}

uint8_t midi_get_note(void)
{
	return _midi_note;
}

uint8_t midi_get_gate(void)
{
	return _midi_gate;
}

void midi_register_note_on_cb(void (*note_on_cb)(uint8_t note, uint8_t vel))
{
	_midi_note_on_cb = note_on_cb;
}

void midi_register_note_off_cb(void (*note_off_cb)(uint8_t note, uint8_t vel))
{
	_midi_note_off_cb = note_off_cb;
}
	
void midi_process_byte(uint8_t data)
{
	/* _midi_rx_buf stores the content of a MIDI message as follows: */
	/* _midi_rx_buf[0] - status byte */
	/* _midi_rx_buf[1] - data byte 1 */
	/* _midi_rx_buf[2] - data byte 2 */
		
	/* Check for System Real-Time messages first */
	/* These are a special case of MIDI message that only consist of 1 byte and can be received at any time */
	/* (including in the middle of another MIDI message) */
	/* Don't need to store in _midi_rx_buf or increment _midi_rx_ptr */
	if (IS_SYSTEM_REAL_TIME(data))
	{
		switch(data)
		{	
		/* Reset */
		/* _midi_rx_buf[0] = 0xFF */	
		case RESET:				
			break;
				
		default:		
			break;
		}
	}
	/* All other message types start here */
	else
	{
		/* Check for repeat messages (Running Status): */
		/* control bytes may only be sent once to save bandwidth */
		if (_midi_rx_ptr == 0 && IS_DATA(data)) _midi_rx_ptr++;
		
		/* Now write into buffer */	
		_midi_rx_buf[_midi_rx_ptr] = data;
		
		/* System Common/Exclusive Messages */
		/* _midi_rx_buf[0] & 0xF0 = 0xF0 */
		/* Don't need to filter channel regardless of Omni mode */
		if (IS_SYSTEM_MESSAGE(_midi_rx_buf[0]))
		{
			switch (_midi_rx_buf[0])
			{
			/* Ignore other System Messages */
			default:		
				_midi_rx_ptr = 0;
				break;
			}	
		}
		/* Channel filtering when Omni mode is off */
		else if (_midi_omni_mode == MIDI_OMNI_OFF)
		{
			if ((_midi_rx_buf[0] & 0x0F) != _midi_channel)
			{
				_midi_rx_ptr = 0;
				return; // ignore messages not sent to our channel
			}				
		}
		
		/* Channel Voice/Mode Messages */	
		switch (_midi_rx_buf[0] & 0xF0)
		{
		/* Note On Event */
		/* _midi_rx_buf[0] = 0x9[channel] */
		/* _midi_rx_buf[1] = note number */
		/* _midi_rx_buf[2] = note velocity */
		case NOTE_ON:
			if (_midi_rx_ptr == 2)
			{
				/* A Note On with velocity = 0 is equivalent to a Note Off */
				if (_midi_rx_buf[2] == 0) 
				{
					// _midi_note = 0;
					_midi_gate = 0;
					_midi_note_off_cb(_midi_rx_buf[1], _midi_rx_buf[2]);
				}
				else 
				{
					_midi_note = _midi_rx_buf[1];
					_midi_gate = 1;
					_midi_note_on_cb(_midi_rx_buf[1], _midi_rx_buf[2]);
				}                  

				_midi_rx_ptr = 0; // reset rx pointer
			}
			else _midi_rx_ptr++;
			break;
			
		/* Note Off Event */
		/* _midi_rx_buf[0] = 0x8[channel] */
		/* _midi_rx_buf[1] = note number */
		/* _midi_rx_buf[2] = note velocity */
		case NOTE_OFF:					
			if (_midi_rx_ptr == 2)
			{
				// _midi_note = 0;
				_midi_gate = 0;	
				_midi_note_off_cb(_midi_rx_buf[1], _midi_rx_buf[2]);

				_midi_rx_ptr = 0; // reset rx pointer
			}
			else _midi_rx_ptr++;
			break;
			
		/* Control Change and Channel Mode */
		/* _midi_rx_buf[0] = 0xB[channel] */
		/* _midi_rx_buf[1] = controller number (0-119) or channel mode (120-127) */
		/* _midi_rx_buf[2] = controller value (0-127) */	
		case CONTROL_CHANGE:			
			if (_midi_rx_ptr == 2)
			{
				switch (_midi_rx_buf[1])
				{
				case 0x40: // Damper (sustain) pedal
					if (_midi_rx_buf[2] > 63) // ON
					{
							
					}
					else // OFF
					{
						
					}
					break;
					
				default:
					break;
				}
					
				_midi_rx_ptr = 0;
			}
			else _midi_rx_ptr++;
			break;
			
		/* Program Change */
		/* _midi_rx_buf[0] = 0xC[channel] */
		/* _midi_rx_buf[1]	= program number (0-127) */	
		case PROGRAM_CHANGE:		
			if (_midi_rx_ptr == 1)
			{					
				_midi_rx_ptr = 0;
			}
			else _midi_rx_ptr++;
			break;
			
		/* Ignore other control bytes */
		/* _midi_rx_buf[0] = anything else */	
		default:	
			_midi_rx_ptr = 0;
			break;
		}
	}
}


