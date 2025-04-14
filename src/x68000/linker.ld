SECTIONS
{
	. = 0x2000;
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(1024);
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
		. = ALIGN(1024);
	}
	image_end = .;
	image_size = image_end - image_start;
	sector_count = (image_end - image_start) >> 9;
	.bss :
	{
		bss_start = .;
		*(COMMON)
		*(.bss)
		bss_end = .;
		*(stack)
	}
	bss_size = bss_end - bss_start;
}
