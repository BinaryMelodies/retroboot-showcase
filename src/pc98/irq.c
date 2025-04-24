
// hardware interrupts

#define PORT_PIC1_COMMAND 0x00
#define PORT_PIC2_COMMAND 0x08
#define PORT_PIC1_DATA    (PORT_PIC1_COMMAND + 2)
#define PORT_PIC2_DATA    (PORT_PIC2_COMMAND + 2)

#include "../i8259.c"

