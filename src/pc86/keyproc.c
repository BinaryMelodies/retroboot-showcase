
#if MACHINE_IBMPC
static bool keyboard_extended_key;
#endif

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
			int vscancode = scancode;
#if MACHINE_IBMPC
			if(keyboard_extended_key)
				vscancode += __KBD_EXTENDED_FLAG;
#endif
			int c = keyboard_shift ? keyboard_scancode_table[vscancode].shifted : keyboard_scancode_table[vscancode].normal;
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

#if MACHINE_IBMPC
	keyboard_extended_key = (scancode == _KBD_EXTENDED_PREFIX);
#endif
}

