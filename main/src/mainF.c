/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "cmsis_compiler.h"
#include "stm32f4xx_hal.h"
#include "peripheral.h"
#include "ledData.h"
#include "animations.h"
#include "com.h"

static void maintainStatusLeds(void) {
	static const uint32_t blueLedToggleTimeMs = 200uL;
	static uint32_t lastToggle = 0uL;

	if ((HAL_GetTick() - lastToggle) > blueLedToggleTimeMs) {
		blueLedToggle();
		lastToggle = HAL_GetTick();

		static uint8_t swCount = 0u;
		if (!getModeSwitch()) {
			++swCount;
			if (swCount > 5u) {
				swCount = 0u;
				orangeLedToggle();
				led_initDataRaw();
				anim_nextMode();
			}
		}
	}
}

static bool sendLock = false;
static void cyclicReSend(void) {
	static const uint32_t triggerTimeMs = 50uL;

	static uint32_t lastToggle = 0uL;

	if (!sendLock && anim_needRepeat() && ((HAL_GetTick() - lastToggle) > triggerTimeMs)) {
		anim_CyclicCall();

		greenLedToggle();
		sendLock = true;
		led_pasteData();
		led_transmitData();

		lastToggle = HAL_GetTick();
	}

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

	sendLock = false;
//  __BKPT(0);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}

int main(void) {
	initClock();
	initPeripherals();
	anim_setMode(anim_rnd3);
	led_setBrightnessTruncation(1u, 1u);

	led_initDataRaw();
	outputEnableLvlShifter();
	__enable_irq();
	com_enableRx();
	for (;;) {
		maintainStatusLeds();
		cyclicReSend();
		com_parse();
	}
}
