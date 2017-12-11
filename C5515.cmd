/****************************************************************************/
/*  C5515.cmd                                                               */
/*  Copyright (c) 2012  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a C5515.              */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

-stack    0x800      /* Primary stack size   */
-sysstack 0x400      /* Secondary stack size */
-heap     0x400     /* Heap area size       */

-c                    /* Use C linking conventions: auto-init vars at runtime */
-u _Reset             /* Force load of reset interrupt handler                */

MEMORY
{
    MMR     (RW)  : origin = 0000000h length = 0000c0h  /* MMRs */
    DARAM_0 (RW)  : origin = 00000c0h length = 001f40h	/* on-chip DARAM 0 */
    DARAM_1 (RW)  : origin = 0002000h length = 002000h	/* on-chip DARAM 1 */
    DARAM_2 (RW)  : origin = 0004000h length = 002000h	/* on-chip DARAM 2 */
    DARAM_3 (RW)  : origin = 0006000h length = 002000h	/* on-chip DARAM 3 */
    DARAM_4 (RW)  : origin = 0008000h length = 002000h	/* on-chip DARAM 4 */
    DARAM_5 (RW)  : origin = 000a000h length = 002000h	/* on-chip DARAM 5 */
    DARAM_6 (RW)  : origin = 000c000h length = 002000h	/* on-chip DARAM 6 */
    DARAM_7 (RW)  : origin = 000e000h length = 002000h	/* on-chip DARAM 7 */

    SARAM   (RW) : origin = 0010000h length = 040000h	/* on-chip SARAM */

    SAROM_0 (RX)  : origin = 0fe0000h length = 008000h 	/* on-chip ROM 0 */
    SAROM_1 (RX)  : origin = 0fe8000h length = 008000h 	/* on-chip ROM 1 */
    SAROM_2 (RX)  : origin = 0ff0000h length = 008000h 	/* on-chip ROM 2 */
    SAROM_3 (RX)  : origin = 0ff8000h length = 007f00h 	/* on-chip ROM 3 */
	VECS    (RX)  : origin = 0ffff00h length = 000100h	/* on-chip ROM vectors */

    EMIF_CS0 (RW) : origin = 0050000h  length = 07B0000h	/* mSDR */
    EMIF_CS2 (RW) : origin = 0800000h  length = 0400000h	/* ASYNC1 : NAND */
    EMIF_CS3 (RW) : origin = 0C00000h  length = 0200000h	/* ASYNC2 : NAND  */
    EMIF_CS4 (RW) : origin = 0E00000h  length = 0100000h	/* ASYNC3 : NOR */
    EMIF_CS5 (RW) : origin = 0F00000h  length = 00E0000h	/* ASYNC4 : SRAM */
}


SECTIONS
{
    vectors (NOLOAD)
    .bss        : > DARAM_0    /* fill = 0 */
    vector      : > DARAM_0    ALIGN = 256
    .stack      : > DARAM_5
    .sysstack   : > DARAM_5
    .sysmem 	: > DARAM_5
    .text       : > SARAM   	ALIGN = 4
    .data       : > DARAM_2
    .cinit 		: > SARAM
    .const 		: > SARAM
    .cio		: > SARAM
    .usect   	: > SARAM
    .switch     : > SARAM
    .emif_cs0   : > EMIF_CS0
    .emif_cs2   : > EMIF_CS2
    .emif_cs3   : > EMIF_CS3
    .emif_cs4   : > EMIF_CS4
    .emif_cs5   : > EMIF_CS5

// The Bit-Reverse destination buffer data_br_buf requires an address with
// at least 4+log2(FFT_LENGTH) least significant binary zeros

    data_br_buf 	: > DARAM_1
    scratch_buf 	: > DARAM_1
    convolved_buf	: > DARAM_2
    coeffs_fft_buf	: > DARAM_2

    RcvL1			: > DARAM_3
    RcvL2			: > DARAM_3
    RcvR1			: > DARAM_4
    RcvR2			: > DARAM_4

    FilterOut	    : > DARAM_5
    OverlapL	    : > DARAM_5
    OverlapR	    : > DARAM_5

    XmitL1			: > DARAM_6
    XmitL2			: > DARAM_6
    XmitR1			: > DARAM_7
    XmitR2			: > DARAM_7

    RcvL1_copy	    : > SARAM
    RcvL2_copy	    : > SARAM
    RcvR1_copy	    : > SARAM
    RcvR2_copy	    : > SARAM
}

// To call the HWAFFT routines from ROM, uncomment the following lines then delete hwafft.asm from the CCS project (or exclude from build)
// Only ROM Addresses for VC5505 (PG1.4) are provided.

// VC5505 (PG1.4) HWAFFT Routines ROM Addresses:
/*	_hwafft_br      = 0x00ff7342; */
/*	_hwafft_8pts    = 0x00ff7356; */
/*	_hwafft_16pts   = 0x00ff7445; */
/*	_hwafft_32pts   = 0x00ff759b; */
/*	_hwafft_64pts   = 0x00ff78a4; */
/*	_hwafft_128pts  = 0x00ff7a39; */
/*	_hwafft_256pts  = 0x00ff7c4a; */
/*	_hwafft_512pts  = 0x00ff7e48; */
/*	_hwafft_1024pts = 0x00ff80c2; */

/* HWAFFT Routines ROM Addresses */
/* C5505/C5515 (PG 2.0) */
	_hwafft_br = 0x00ff6cd6;
	_hwafft_8pts = 0x00ff6cea;
	_hwafft_16pts = 0x00ff6dd9;
	_hwafft_32pts = 0x00ff6f2f;
	_hwafft_64pts = 0x00ff7238;
	_hwafft_128pts = 0x00ff73cd;
	_hwafft_256pts = 0x00ff75de;
	_hwafft_512pts = 0x00ff77dc;
	_hwafft_1024pts = 0x00ff7a56;
