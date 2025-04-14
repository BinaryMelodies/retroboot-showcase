SECTIONS
{
	. = 0x7C00;
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(512);
		*(.text)
	}
	.rodata :
	{
		*(.rodata)
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
		bss_end = .;
		*(stack)
	}
	bss_size = bss_end - bss_start;
}
