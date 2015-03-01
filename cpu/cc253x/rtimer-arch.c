/*
 * Copyright (c) 2010, Loughborough University - Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Hardware-dependent functions used to support the
 *         contiki rtimer module.
 *
 *         clock_init() has set our tick speed prescaler already, so we
 *         are ticking with 500 kHz freq.
 *
 *         Contiki typedefs rtimer_clock_t as unsigned short (16bit)
 *         It thus makes sense to use the 16bit timer (Timer 1)
 *
 *         This file contains an ISR and must reside in the HOME bank
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#include "sys/rtimer.h"
#include "sfr-bits.h"
#include "cc253x.h"
#include "sys/energest.h"

#include "debug.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{
  /* Enable Sleep Timer Interrupts */
  STIF = 0;
  STIE = 1;
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
  //T must be higher than 5
  if (t <= 5) t = 6;

  //Read from ST2:ST1:ST0 the current value and add t
  
  //await for sleep timer loader to be ready
  while (STLOAD == 0);
  /* set the compare mode values so we can get an interrupt after t *
   * set ST0 last, as this will apply the compare value */
  ST2 = (unsigned char)(t >> 16);
  ST1 = (unsigned char)(t >> 8);
  ST0 = (unsigned char)t;

  /* Turn on compare mode interrupt */
  STIF = 0;
}
/*---------------------------------------------------------------------------*/
void rtimer_arch_sleep(rtimer_clock_t howlong)
{
  //await for sleep timer loader to be ready
  while (STLOAD == 0);

//read the value from sleep timer, and add howlong to it

//SLEEPCMD.MODE to the desired PM mode 0 idle, 1 pm1, 2 pm2, 3 pm3

//CLKCONCMD.OSC to 0 -- shouldn't be necesarry, value is retained

//Set pcon.idle to 1 -- enter sleep mode

}
/*---------------------------------------------------------------------------*/
/* avoid referencing bits, we don't call code which use them */
#pragma save
#if CC_CONF_OPTIMIZE_STACK_SIZE
#pragma exclude bits
#endif
void
rtimer_isr(void) __interrupt(ST_VECTOR)
{
  STIE = 0; /* Ignore Sleep Timer Interrupts */
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  rtimer_run_next();

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
  STIF = 0; /* Clear Interrupt Flag */
  STIE = 1; /* Acknowledge Sleep Timer Interrupts */
}
#pragma restore
/*---------------------------------------------------------------------------*/
