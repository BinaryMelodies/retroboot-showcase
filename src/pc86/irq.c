
// hardware interrupts

#define PORT_PIC1_COMMAND 0x20
#define PORT_PIC2_COMMAND 0xA0
#define PORT_PIC1_DATA    (PORT_PIC1_COMMAND + 1)
#define PORT_PIC2_DATA    (PORT_PIC2_COMMAND + 1)

#include "../i8259.c"

