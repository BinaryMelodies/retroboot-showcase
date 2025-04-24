SECTIONS
{
	. = 0x5000;
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(1024);
		*(.text)
	}
	.rodata :
	{
		*(.rodata)
	}
	.data :
	{
		*(.data)
		. = ALIGN(1024);
	}
	image_end = .;
	image_size = image_end - image_start;
	sector_count = (image_end - image_start) >> 10;
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
