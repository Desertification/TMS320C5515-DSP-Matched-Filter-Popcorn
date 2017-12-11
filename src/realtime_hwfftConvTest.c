 /*
 * realtime_hwfftConvTest.c
 *
 *  Created on: Oct 14, 2012
 *      Author: BLEE
 *
 *  Description: Test program for FFT-iFFT experiment, use 1024 FFT with 4 different filter lengths
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#include <stdio.h>
#include "tistdtypes.h"
#include "i2s.h"
#include "dma.h"
#include "dmaBuff.h"


extern void AIC3204_init(Uint32, Int16, Int16);
extern void hwFFTConv(void);
extern void hwFFTConv_init(void);


#define IER0        *(volatile unsigned *)0x0000

#define SF8KHz          8000
#define DAC_GAIN		6		// 6dB range: -6dB to 29dB
#define ADC_GAIN		3		// 3dB range: 0dB to 46dB
#define LED_DIR1 *((ioport volatile Uint16*) 0x1C06 )


void main(void) {
    LED_DIR1 |= 0xC000; // init led dir to out

    setDMA_address();		// DMA address setup for each buffer

    asm(" BCLR ST1_INTM");	// Disable all interrupts
    IER0 = 0x0110;      	// Enable DMA interrupt

    set_i2s0_slave();		// Set I2S
    AIC3204_init(SF8KHz, DAC_GAIN, (Uint16)ADC_GAIN);  // Set AIC3204
    enable_i2s0();

    enable_dma_int();		// Set up and enable DMA, each time get DATA_LEN data samples
    set_dma0_ch0_i2s0_Lout(DATA_LEN);
    set_dma0_ch1_i2s0_Rout(DATA_LEN);
    set_dma0_ch2_i2s0_Lin(DATA_LEN);
    set_dma0_ch3_i2s0_Rin(DATA_LEN);

    hwFFTConv_init();            // Initialize for FFT convolution routine
    hwFFTConv();                 // FFT convolution loop
}






