reset:
b _init
b reset /* reserve space for interruption vectors*/
b reset
b reset
b reset
b reset
b reset
b reset
b reset

_init:

/* SVC32 mode */
ldr r0,=0x800000d3
msr cpsr,r0

/* Configuration (see /comelec/softs/opt/openocd/enst/FS44B0XII.cfg)  */
_config:

.macro mww add, data
ldr r0,= \add
ldr r1,= \data
str r1, [r0]
.endm

// WTCON : disable watchdog
mww 0x01d30000, 0
// INTMSK
mww 0x01e0000c, 0x03fffeff
// INTCON
mww 0x01e00000, 0x5
// LOCKTIME
mww 0x01d8000c, 0xf0
// PLLCON
mww 0x01d80000, 0x0003a031
// CLKCON
mww 0x01d80004, 0x00007ff8
// PCONA
mww 0x01d20000, 0x000003ff
// PDATB
mww 0x01d2000c, 0x7cf
// PCONB
mww  0x01d20008, 0x07ff
// PDATC
mww 0x01d20014, 0x0100
// PCONC
mww 0x01d20010, 0xfff5ff54
// PUPC
mww 0x01d20018, 0x00000000
// PCOND
mww 0x01d2001c, 0x0000aaaa
// PUPD
mww 0x01d20024, 0x00000000
// PCONE
mww 0x01d20028, 0x00021569
// PDATE
mww 0x01d2002c, 0x00000000
// PUPE
mww 0x01d20030, 0x00000000
// PCONF
mww 0x01d20034, 0x24900a
// PUPF
mww 0x01d2003c, 0x00000000
// PCONG
mww 0x01d20040, 0xff3c
// PUPG
mww 0x01d20048, 0x00000000
// SPUCR
mww 0x01d2004c, 0x6
// INTMSK
mww 0x01e0000c, 0x03fffeff
// INTCON
mww 0x01e00000, 0x05
// INTMOD
mww 0x01e00008, 0x0
// EXTINT
mww 0x01d20050, 0x00040440

/* Initialisation de la RAM  */

ldr   r0, =SMRDATA
ldmia r0, {r1-r13}
ldr   r0, =0x01c80000  /* BWSCON Address */
stmia r0, {r1-r13}

_stack:
/* Stack initialisation */
ldr sp,=stack_init

/* Initialisation data et bss  */
_data_bss:
bl init_data_bss

.global _start
_start:
bl main

_exit:
b _exit

SMRDATA:
        .word   0x01001102       /* BWSCON */
        .word   0x00007ff4       /* GCS0 */
        .word   0x00000a40       /* GCS1 */
        .word   0x00001480       /* GCS2 */
        .word   0x00007ffc       /* GCS3 */
        .word   0x00007ffc       /* GCS4 */
        .word   0x00000c40       /* GCS5 */
        .word   0x00018004       /* GCS6, EDO DRAM(Trcd=3, Tcas=2, Tcp=1, CAN=10bit) */
        .word   0x00000a40       /* GCS7, EDO DRAM */
        .word   0x008603fb       /* Refresh(REFEN=1, TREFMD=0, Trp=3, Trc=5, Tchr=3) */
        .word   0x00000010       /* Bank Size, 32MB/32MB */
        .word   0x00000020       /* MRSR 6(CL=2) */
        .word   0x00000020       /* MRSR 7(CL=2) */

