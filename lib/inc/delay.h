#ifndef __DELAY_H
#define __DELAY_H
/* 
 * delay utilite for STM8 family
 * COSMIC and SDCC
 * Terentiev Oleg
 * t.oleg@ymail.com
 */

#include "stm8s.h"
#ifndef HSE_VALUE
#warning HSE_VALUE is not defined!
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

void _delay_cycl( unsigned short );
void _delay_us( const unsigned short );
void _delay_ms( unsigned short );
#endif
