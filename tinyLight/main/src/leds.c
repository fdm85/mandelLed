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

#define PWM_RAW		104uL
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)

lLogicContainer(main, 51);
lRawContainer(main, 51);
lChainDesc(main, htim4, TIM_CHANNEL_1, lRawOn, lRawOff);


