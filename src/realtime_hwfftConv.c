/*
 * realtime_hwfftConv.c
 *
 *  Created on: June 16, 2012
 *      Author: BLEE
 *
 *      Description: This is the test program for FFT-iFFT experiment
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#include <stdio.h>
#include "tistdtypes.h"
#include "dma.h"
#include "dmaBuff.h"
#include "icomplex.h"    		/* Integer complex.h header file */
#include "popcorn_pop_coefs.h"
#include "timer.h"

/* Define DSP system memory map */
#pragma DATA_SECTION(XmitL1, "XmitL1");
#pragma DATA_SECTION(XmitL2, "XmitL2");
#pragma DATA_SECTION(XmitR1, "XmitR1");
#pragma DATA_SECTION(XmitR2, "XmitR2");
#pragma DATA_ALIGN(XmitL1,2);
#pragma DATA_ALIGN(XmitL2,2);
#pragma DATA_ALIGN(XmitR1,2);
#pragma DATA_ALIGN(XmitR2,2);

Int16 XmitL1[FFT_PTS];
Int16 XmitR1[FFT_PTS];
Int16 XmitL2[FFT_PTS];
Int16 XmitR2[FFT_PTS];

#pragma DATA_SECTION(RcvL1, "RcvL1");
#pragma DATA_SECTION(RcvR1, "RcvR1");
#pragma DATA_SECTION(RcvL2, "RcvL2");
#pragma DATA_SECTION(RcvR2, "RcvR2");
#pragma DATA_ALIGN(RcvL1,2);
#pragma DATA_ALIGN(RcvR1,2);
#pragma DATA_ALIGN(RcvL2,2);
#pragma DATA_ALIGN(RcvR2,2);

Int16 RcvL1[FFT_PTS];
Int16 RcvR1[FFT_PTS];
Int16 RcvL2[FFT_PTS];
Int16 RcvR2[FFT_PTS];

// FFT related variables
#pragma DATA_SECTION(x, "data_br_buf");
#pragma DATA_ALIGN(x, 2*FFT_PTS);
complex x[FFT_PTS];

#pragma DATA_SECTION(temp, "scratch_buf");
#pragma DATA_ALIGN(temp, 2*FFT_PTS);
complex temp[FFT_PTS];

#pragma DATA_SECTION(overLapL, "OverlapL");
#pragma DATA_ALIGN(overLapL, 2);
#pragma DATA_SECTION(overLapR, "OverlapR");
#pragma DATA_ALIGN(overLapR, 2);
Int16 overLapL[FFT_PTS-FLT_LEN];
Int16 overLapR[FFT_PTS-FLT_LEN];

#pragma DATA_SECTION(convolved, "convolved_buf");
#pragma DATA_ALIGN(convolved, 2*FFT_PTS);
complex convolved[FFT_PTS];

#pragma DATA_SECTION(coeffs_fft, "coeffs_fft_buf");
#pragma DATA_ALIGN(coeffs_fft, 2*FFT_PTS);
complex coeffs_fft[FFT_PTS];


void hwFFTConv(void);
void hwFFTConv_init(void);
void fftConv(Int16 *, Int16 *, complex *, complex *, Int16 *);
void olap_add(complex *, Int16 *, Uint16, Uint16);

extern Int16 hwafft_1024pts(Int32 *, Int32 *, Uint16, Uint16);
extern void hwafft_br(Int32 *, Int32 *, Uint16);


#define FFT_FLAG        0        /* HWA to perform FFT */
#define IFFT_FLAG       1        /* HWA to perform IFFT */
#define SCALE_FLAG      0        /* HWA to scale butterfly output */
#define NOSCALE_FLAG    1        /* HWA not to scale butterfly output */

#define LED_OUT1 *((ioport volatile Uint16*) 0x1C0A )
Uint16 tmr;

void hwFFTConv_init(void) {
	Int16 i;
	for (i=0; i<FFT_PTS; i++) {
		XmitL1[i]=0;
		XmitL2[i]=0;
		XmitR1[i]=0;
		XmitR2[i]=0;
		RcvL1[i] = 0;
		RcvL2[i] = 0;
		RcvR1[i] = 0;
		RcvR2[i] = 0;
	}

	/* zero Overlap buffers */
	for(i=0; i< (FFT_PTS-FLT_LEN); i++) {
		overLapL[i] = 0;
		overLapR[i] = 0;
	}

	/* load coeffs and zero-pad to FFT_PTS */
	for(i=0; i<FLT_LEN; i++) {
	    temp[i].re = coeffs[i];
	    temp[i].im = 0;
	}
	for(; i<FFT_PTS; i++) {
	 	temp[i].re = 0;
	   	temp[i].im = 0;
	}

	/* FFT and store in coeffs_fft[] */
	hwafft_br((Int32 *)temp, (Int32 *)x, FFT_PTS);  // bit-reverse zero-padded coefficients
	hwafft_1024pts((Int32 *)x, (Int32 *)coeffs_fft, FFT_FLAG, NOSCALE_FLAG); // perform FFT
}

void hwFFTConv(void) {
    Uint16 i = 0;
    LED_OUT1 |= (1<<(15)); // set yellow led off

    Timer0Init();
    StartTimer0();

    while (1) {

        tmr = *CPU_TIM0_CHWR; // debug variable to watch
        if (*CPU_TIM0_CHWR == 0){
            LED_OUT1 &= ~(1<<(15)); // set led on
        } else {
            LED_OUT1 |= (1<<(15)); // set led off
        }

        if(leftChannel == 1) {
        	leftChannel = 0;

            if (CurrentRxL_DMAChannel == 2) {
            	fftConv(RcvL1, XmitL1, x, temp, overLapL);
            } else {
            	fftConv(RcvL2, XmitL2, x, temp, overLapL);
            }
        }
        if(rightChannel == 1) { // output right channel input
        	rightChannel= 0;

            if (CurrentRxR_DMAChannel == 2) {
                for(i=0; i<DATA_LEN; i++) {
                    XmitR1[i] = RcvR1[i];
               }
            }
            else {
                for(i=0; i<DATA_LEN; i++) {
                    XmitR2[i] = RcvR2[i];
                }
            }
        }
    }
}

void fftConv(Int16 *inDataPtr, Int16* outDataPtr, complex *x, complex *temp, Int16 *ol) {
    Uint16 i;

    for (i=0; i<DATA_LEN; i++) {
        x[i].re = *inDataPtr++;
        x[i].im = 0;
    }
    for (; i<FFT_PTS; i++) {
        x[i].re = 0;
        x[i].im = 0;
    }

    // Start FFT
    hwafft_br((Int32 *)x, (Int32 *)temp, FFT_PTS);
    hwafft_1024pts((Int32 *)temp, (Int32 *)x, FFT_FLAG, SCALE_FLAG);

    // Filtering (cross-correlation)
    freqflt(x,coeffs_fft,FFT_PTS);

    // Start IFFT
    hwafft_br((Int32 *)x, (Int32 *)temp, FFT_PTS);
    hwafft_1024pts((Int32 *)temp, (Int32 *)x, IFFT_FLAG, NOSCALE_FLAG);

    // Overlap and add
    olap_add(x,ol,DATA_LEN,FFT_PTS);
    for (i=0; i<DATA_LEN; i++) {
        outDataPtr[i] = x[i].re;
    }

    // detect single peak
    for (i=0; i<DATA_LEN; i++) {
        if (x[i].re > 1000) { // detect where correlation matches strongly
            Timer0Init();
            StartTimer0();
            LED_OUT1 = LED_OUT1 ^ (1<<(14)); // toggle led to visualize peak
            break;
        }
    }
}
