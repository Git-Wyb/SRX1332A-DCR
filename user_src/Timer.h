#ifndef __TIMER_H__
#define __TIMER_H__
#include "type_def.h"
/***********************************************************************/
/*  FILE        :Timer.h                                               */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8S003     Crystal: 4M HSI                          */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/

extern u16 ErrStateTimeer;
extern u16 StateReadTimer;
extern u16 Time_Tx_Out;
//============================================ Timer 4
void TIM4_Init(void);
void TIM4_UPD_OVF(void);
void system_delay(u32 n);
void system_delay_us(u32 n);

#endif
