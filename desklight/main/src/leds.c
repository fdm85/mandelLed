/******************************************************************************/
/*!
 * \file
 *
 * \brief info: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "leds.h"
#include "tim.h"

#define PWM_RAW		90uL
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)

lLogicContainer(main, 30);
lRawContainer(main, 30);
lChainDesc(main, htim4, TIM_CHANNEL_2, lRawOn, lRawOff);


