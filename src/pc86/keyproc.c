
static inline void keyboard_interrupt_process(uint8_t scancode)
{
	if((scancode & PS2_SCANCODE_RELEASED) == 0)
	{
		// key pressed
		if(scancode == PS2_SCANCODE_LSHIFT || scancode == PS2_SCANCODE_RSHIFT)
		{
			// shift
			keyboard_shift = true;
		}
		else
		{
			int c = keyboard_shift ? keyboard_scancode_table[scancode].shifted : keyboard_scancode_table[scancode].normal;
			keyboard_buffer_push(c);
		}
	}
	else
	{
		// key released
		if(scancode == (PS2_SCANCODE_RELEASED|PS2_SCANCODE_LSHIFT) || scancode == (PS2_SCANCODE_RELEASED|PS2_SCANCODE_RSHIFT))
		{
			// shift
			keyboard_shift = false;
		}
	}
}

