
/*===============ASA Series Library===============*/

/*****ReleaseDate*****/
//----/--/--

/*****Feature*****/

/*===Supported Module===*/
//ASA M128

/*================================================*/

#ifndef __ASA_H__
#define __ASA_H__

#include <avr\io.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <util\delay.h>

#define M128_AllINT_enable sei()
#define M128_AllINT_disable cli()

/**M128**/
char ASA_M128_set(void);

#endif
