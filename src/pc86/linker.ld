SECTIONS
{
	. = 0x5000;
	image_start = .;
	.text :
	{
		*(boot)
		. = ALIGN(SECTOR_SIZE);
		*(.text)
	}
	.rodata :
	{
		*(.rodata)
	}
	.data :
	{
		*(.data)
		. = ALIGN(SECTOR_SIZE);
	}
	image_end = .;
	image_size = image_end - image_start;
	sector_count = (image_end - image_start) >> SECTOR_SIZE_SHIFT;
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
