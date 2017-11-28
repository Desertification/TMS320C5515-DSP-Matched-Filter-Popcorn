;/*
; * freqflt.asmm
; *
; *  Created on: Oct 14, 2012
; *      Author: BLEE
; *
; *  Description: Perform frequency domain filtering
; *
; *  Prototype: void freqflt(complex *, complex *, unsigned int);
; *
; *  Entry: arg0: AR0 is the pointer to X[]
; *         arg1: AR1 is the pointer to H[]
; *         arg2: T0  is the bin numbers of the FFT, N
; *
; *  For the book "Real Time Digital Signal Processing:
; *                Fundamentals, Implementation and Application, 3rd Ed"
; *                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
; *                Publisher: John Wiley and Sons, Ltd
; */

    .global	_freqflt
     .sect	".text:fft_code"
 	
_freqflt:  
    pshm  ST1_55
    pshm  ST3_55
    sub   #1,T0
    mov   T0,BRC0             ; Loop trip counter = N-1
    mov   #2,T0               ; Offset of H[] update    
    bset  SMUL
    bset  SATD
    bset  FRCT            
||  rptblocal product_loop-1  ; for (i=0; i<N; i++)
    mpym  *AR0+,*AR1+,AC1     ;    AC1 = X[i].re * H[i].re
    masm  *AR0-,*AR1,AC1      ;    AC1 -= X[i].im * H[i].im
    mpym  *AR0+,*AR1-,AC0     ;    AC0 = X[i].re * H[i].im
    macm  *AR0-,*(AR1+T0),AC0 ;    AC0 += X[i].im * H[i].re
    mov   rnd(hi(AC1)),*AR0+  ;    X[i].re = AC1>>16
    mov   rnd(hi(AC0)),*AR0+  ;    X[i].im = AC0>>16
product_loop            

    popm  ST3_55
    popm  ST1_55
    ret
	
    .end
