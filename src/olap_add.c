/*
 * olap_add.c
 *
 *  Created on: Oct 14, 2012
 *      Author: BLEE
 *
 *  Description: Perform overlap and add operation
 *
 *  For the book "Real Time Digital Signal Processing:
 *                Fundamentals, Implementation and Application, 3rd Ed"
 *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
 *                Publisher: John Wiley and Sons, Ltd
 */

#include "tistdtypes.h"
#include "icomplex.h"

void olap_add(complex *, Int16 *, Uint16, Uint16);

void olap_add(complex *x, Int16 *o, Uint16 data_len, Uint16 fft_len)
{
    Int16 i,k;

    k = fft_len - data_len;

	for (i=0; i<k; i++)			/* Add the previous overlap to output */
	{
		x[i].re += o[i];
		x[i].im = 0;
		o[i] = x[i+data_len].re;
	}
	for (i=k; i<data_len; i++)
	{
		x[i].im = 0;
	}
}


