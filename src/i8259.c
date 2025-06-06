
// Intel 8259 programmable interrupt controller

#define PIC_ICW1_ICW4 0x01
#define PIC_ICW1_INIT 0x10
#define PIC_ICW4_8086 0x01
#define PIC_EOI       0x20

static inline void i8259_setup(uint8_t pic1_data, uint8_t pic2_data)
{
	outb(PORT_PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	io_wait();
	outb(PORT_PIC2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	io_wait();
	outb(PORT_PIC1_DATA,    pic1_data);
	io_wait();
	outb(PORT_PIC2_DATA,    pic2_data);
	io_wait();
	outb(PORT_PIC1_DATA,    4);
	io_wait();
	outb(PORT_PIC2_DATA,    2);
	io_wait();
	outb(PORT_PIC1_DATA,    PIC_ICW4_8086);
	io_wait();
	outb(PORT_PIC2_DATA,    PIC_ICW4_8086);
	io_wait();
	outb(PORT_PIC1_DATA,    0);
	outb(PORT_PIC2_DATA,    0);
}

