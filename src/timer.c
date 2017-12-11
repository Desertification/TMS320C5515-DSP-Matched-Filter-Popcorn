/**********************************************************************
**+-----------------------------------------------------------------+**
**|                            ****                                 |**
**|                            ****                                 |**
**|                            ******o***                           |**
**|                      ********_///_****                          |**
**|                      ***** /_//_/ ****                          |**
**|                       ** ** (__/ ****                           |**
**|                           *********                             |**
**|                            ****                                 |**
**|                            ***                                  |**
**|                                                                 |**
**|   Copyright (c) 2006 - 2010    Texas Instruments Incorporated   |**
**|                        ALL RIGHTS RESERVED                      |**
**|                                                                 |**
**| Permission is hereby granted to licensees of Texas Instruments  |**
**| Incorporated (TI) products to use this computer program for     |**
**| the sole purpose of implementing a licensee product based       |**
**| on TI products.No other rights to reproduce, use, or            |**
**| disseminate this computer program, whether in part or in whole, |**
**| are granted.TI makes no representation or warranties with       |**
**| respect to the performance of this computer program, and        |**
**| specifically disclaims any responsibility for any damages,      |**
**| special or consequential,connected with the use of this program.|**
**|                                                                 |**
**+-----------------------------------------------------------------+**
**********************************************************************/
#include "data_types.h"
#include "timer.h"
#include "register_cpu.h"

#define LED_OUT1 *((ioport volatile Uint16*) 0x1C0A )

void Timer0Init(void)
{

	/*  Timer0 Initialization */
// timer interval 10msec
// prescale = 0 (devide by 2)
// 100/2 = 50MHz ==> 20 nsec
// 10msec/20nsec = 500000 (0x7 A120)
// 10sec/20nsec = 500000000 (0x1DCD 6500)

	/* TIM0 EN | AutoReload disable | Prescale = 0(100/2 = 50MHz) ==> 20nsec */
	//*CPU_TIM0_CTRL = 0x8002; 	// autoReload
    *CPU_TIM0_CTRL = 0x8000; 	// disable autoReload

	*CPU_TIM0_PLWR = 0x6500;
	*CPU_TIM0_PHWR = 0x1DCD;

    //*CPU_TIM0_PLWR = 0xA120;
    //*CPU_TIM0_PHWR = 0x0007;

	*CPU_TIM0_CLWR = 0x0000;
	*CPU_TIM0_CHWR = 0x0000;
	
	/*  Clearing timer Aggregation register*/
	*CPU_TIMINT_AGGR = 0x0007;

	/*  enable timer0 int flag*/
	*CPU_TIM0_IER = 0x0001;
}

void StartTimer0(void)
{
	/* Start the Timer 0*/
	*CPU_TIM0_CTRL = *CPU_TIM0_CTRL | 0x0001; 
}


interrupt void Timer_isr(void)
{

    // clear timer int flag
    IFR0 = IFR0&0x0010; 
    
	/*  clear timer0 int flag*/
	*CPU_TIM0_IER = 0x0001;
	    
	/*	Clear Timer0 bit in Timer Aggregate register*/
	*CPU_TIMINT_AGGR = *CPU_TIMINT_AGGR | 0x0001 ;	

    //StartTimer0();

	LED_OUT1 = LED_OUT1 ^ (1<<(14));
	LED_OUT1 &= ~(1<<(15)); // set yellow led off
}

