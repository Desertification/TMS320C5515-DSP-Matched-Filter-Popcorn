/*
 * dmaBuff.h
 *
 *  Created on: Oct 14, 2012
 *      Author: BLEE
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#ifndef DMABUFF_H_
#define DMABUFF_H_

// Select one filter for experiment, only one (must) be selected for experiment
#define USE_FLT511		1
#define USE_FLT255		0
#define USE_FLT127		0
#define USE_FLT63		0

// This is the filter length, which is determined from the filter length
#if 	USE_FLT511
#define FLT_LEN 		 511
#elif   USE_FLT255
#define FLT_LEN 		 255
#elif   USE_FLT127
#define FLT_LEN 		 127
#elif   USE_FLT63
#define FLT_LEN 		 63
#endif


#define	FFT_PTS 		1024				// This is for FFT size

#define DATA_LEN     (FFT_PTS-FLT_LEN+1)	// Data window size

extern Int16  XmitL1[];
extern Int16  XmitR1[];
extern Int16  XmitL2[];
extern Int16  XmitR2[];

extern Int16  RcvL1[];
extern Int16  RcvR1[];
extern Int16  RcvL2[];
extern Int16  RcvR2[];

#endif /* DMABUFF_H_ */
