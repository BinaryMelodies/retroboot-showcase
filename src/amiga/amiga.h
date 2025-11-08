#ifndef _AMIGA_H
#define _AMIGA_H

#define CUSTOM 0xDFF000
#define INTEQR (*(volatile uint16_t *)(CUSTOM + 0x01E))
#define COP1LCH (*(volatile uint16_t *)(CUSTOM + 0x080))
#define COPJMP1 (*(volatile uint16_t *)(CUSTOM + 0x088))
#define DIWSTRT (*(volatile uint16_t *)(CUSTOM + 0x08E))
#define DIWSTOP (*(volatile uint16_t *)(CUSTOM + 0x090))
#define DDFSTRT (*(volatile uint16_t *)(CUSTOM + 0x092))
#define DDFSTOP (*(volatile uint16_t *)(CUSTOM + 0x094))
#define DMACON (*(volatile uint16_t *)(CUSTOM + 0x096))
#define INTENA (*(volatile uint16_t *)(CUSTOM + 0x09A))
#define INTREQ (*(volatile uint16_t *)(CUSTOM + 0x09C))
#define BPLCON0 (*(volatile uint16_t *)(CUSTOM + 0x100))
#define BPLCON1 (*(volatile uint16_t *)(CUSTOM + 0x102))
#define BPL1MOD (*(volatile uint16_t *)(CUSTOM + 0x108))
#define BPL2MOD (*(volatile uint16_t *)(CUSTOM + 0x10A))
#define COLOR0 (*(volatile uint16_t *)(CUSTOM + 0x180))
#define COLOR1 (*(volatile uint16_t *)(CUSTOM + 0x182))

#define CIAATALO (*(volatile uint8_t *)0xBFE401)
#define CIAATAHI (*(volatile uint8_t *)0xBFE501)
#define CIAATBLO (*(volatile uint8_t *)0xBFE601)
#define CIAATBHI (*(volatile uint8_t *)0xBFE701)
#define CIAASDR (*(volatile uint8_t *)0xBFEC01)
#define CIAAICR (*(volatile uint8_t *)0xBFED01)
#define CIAACRA (*(volatile uint8_t *)0xBFEE01)
#define CIAACRB (*(volatile uint8_t *)0xBFEF01)

#endif // _AMIGA_H
