/******************************************************************************/
/*!
 * \file
 *
 * \brief smartFIXME: add file brief
 *
 * todo: add file description
 ******************************************************************************/

#include "msgeq7.h"
#include "main.h"
#include "time.h"
#include "assrt.h"
#include "adc.h"

typedef enum
{
	eInit = 0u, eStart, eCycle, eFin,
} gS_t;

typedef struct mT_s
{
	time_t initTime;
	time_t cycleTime;
	uint32_t cycleTarget;
	time_t strobeTime;
	uint32_t strobeTarget;
	uint8_t cycle;
	gS_t gS;
} mT_t;

static mT_t mT;

static void sInit(void)
{
	mT.cycleTarget = 1000uL;
	mT.strobeTarget = 1000uL;
	tStart(&mT.initTime);
	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_SET);
}

static void exInit(void)
{
	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_RESET);
}

void msgeq_ticker(void)
{
	uint32_t adcState = HAL_ADC_GetState(&hadc1);
	static const uint32_t triggerTimeMs = 50uL;
	static uint32_t lastToggle = 0uL;

	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_RESET);
	if (adcState & HAL_ADC_STATE_READY)
	{
		if ((HAL_GetTick() - lastToggle) > triggerTimeMs)
		{
			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_SET);
			lastToggle = HAL_GetTick();
			HAL_ADC_Start_IT(&hadc1);
			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin,
					GPIO_PIN_RESET);
		}

	}

	switch (mT.gS)
	{
	case eInit:
		if (mT.initTime.started)
		{
			exInit();
			mT.gS = eStart;
		}
		else
		{
			sInit();
		}
		break;
	case eStart:

		break;
	case eCycle:

		break;
	case eFin:

		break;
	default:
		break;
	}
}

static uint32_t adcVal;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	(void) hadc;
	adcVal = HAL_ADC_GetValue(&hadc1);

}

uint32_t getAdcVal(void)
{
	return adcVal;
}
