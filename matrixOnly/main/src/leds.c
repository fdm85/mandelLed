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

#define PWM_RAW		101uL
#define lRawOn ((2uL * PWM_RAW)/3uL)
#define lRawOff ((1uL * PWM_RAW)/3uL)

lRawContainer(matrix, 256);
lLogicContainer(matrix, 256);
lChainDesc(matrix, htim2, TIM_CHANNEL_1, lRawOn, lRawOff);


