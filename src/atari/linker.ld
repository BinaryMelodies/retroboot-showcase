SECTIONS
{
	. = 0;
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(512);
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
		. = ALIGN(512);
	}
	image_end = .;
	image_size = image_end - image_start;
	sector_count = (image_end - image_start) >> 9;
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
	memory_end = .;
	memory_size = memory_end - image_start;
}
