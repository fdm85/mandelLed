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

lLogicContainer(main, 619);
lRawContainer(main, 619);
lChainDesc(main, htim3, TIM_CHANNEL_3, lRawOn, lRawOff);

lLogicContainer(matrix, 256);
lRawContainer(matrix, 256);
lChainDesc(matrix, htim4, TIM_CHANNEL_2, lRawOn, lRawOff);


