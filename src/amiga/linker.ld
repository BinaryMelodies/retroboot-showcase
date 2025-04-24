SECTIONS
{
	. = 0x1000;
	chipram :
	{
		*(chipram)
		. = ALIGN(1024);
	}
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(BOOT_BLOCK_SIZE);
		*(.text)
		. = ALIGN(4);
	}
	.rodata :
	{
		*(.rodata)
		. = ALIGN(4);
	}
	.data :
	{
		*(.data)
		. = ALIGN(SECTOR_SIZE);
	}
	image_end = .;
	image_size = image_end - image_start;
	.bss :
	{
		bss_start = .;
		*(COMMON)
		*(.bss)
		. = ALIGN(4);
		bss_end = .;
		*(stack)
	}
	bss_size = bss_end - bss_start;
}
