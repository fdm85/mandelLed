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
	eInit = 0u, eStart, eStrobe, eAdcStart, eAdcFin, eFin,
} gS_t;

typedef struct mT_s
{
	time_t initTime;
	time_t cycleTime;
	uint32_t cycleTarget;
	time_t strobeTime;
	uint32_t strobeTarget;
	uint32_t adcChan1[eMax];
	uint32_t adcChan2[eMax];
	volatile uint8_t actChan;
	volatile uint8_t chan2;
	volatile msgeq7Freq cycle;
	gS_t gS;
} mT_t;

static mT_t mT;

static void sInit(void)
{
	assrt(mT.gS == eInit);
	mT.cycleTarget = 100uL;
	mT.strobeTarget = 1uL;
	tStart(&mT.initTime);
	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_SET);
}

static void exInit(void)
{
	assrt(mT.gS == eInit);
	tReset(&mT.initTime);
	HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_RESET);
}

static void sStrobe(void)
{
	assrt(mT.gS == eStrobe);
	tStart(&mT.strobeTime);
	HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_SET);
}

static void exStrobe(void)
{
	assrt(mT.gS == eStrobe);
	tStart(&mT.strobeTime);
	HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_RESET);
}

static void sStart(void)
{
	assrt(mT.gS == eStart);
	tStart(&mT.initTime);
	tStart(&mT.cycleTime);
	mT.cycle = e63Hz;
}

static void sAdc(void)
{
	assrt(mT.gS == eAdcStart);
	uint32_t adcState = HAL_ADC_GetState(&hadc1);
	bool rdy = ((adcState & HAL_ADC_STATE_READY) != 0uL);
	assrt(rdy);
	mT.actChan = 1u;
	tReset(&mT.strobeTime);
	HAL_ADC_Start_IT(&hadc1);
}


void msgeq_ticker(void)
{
//	uint32_t adcState = HAL_ADC_GetState(&hadc1);
//	static const uint32_t triggerTimeMs = 50uL;
//	static uint32_t lastToggle = 0uL;
//
//	if (adcState & HAL_ADC_STATE_READY)
//	{
//		if ((HAL_GetTick() - lastToggle) > triggerTimeMs)
//		{
//			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_SET);
//			lastToggle = HAL_GetTick();
//			HAL_ADC_Start_IT(&hadc1);
//			HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin,
//					GPIO_PIN_RESET);
//		}
//
//	}

	switch (mT.gS)
	{
	// min reset pulse width 100nS
	case eInit:
		if (tElapsed(&mT.initTime) >= 1uL)
		{
			exInit();
			mT.gS = eStart;
		}
		else
		{
			if(!mT.initTime.started)
				sInit();
		}
		break;
	// reset to strobe delay 72uS
	case eStart:
		if (tElapsed(&mT.initTime) >= 1uL)
		{
			mT.gS = eStrobe;
		}
		else
		{
			if(!mT.initTime.started)
				sStart();
		}

		break;
	// min strobe pulse width 18uS
	// min stobe to strobe delay 72us (falling edge to falling edge)
	case eStrobe:
		if (tElapsed(&mT.strobeTime) >= 1uL)
		{
			exStrobe();
			mT.gS = eAdcStart;
		}
		else
		{
			if(!mT.strobeTime.started)
				sStrobe();
		}
		break;
	// min output settling time 36uS
	case eAdcStart:
		if (tElapsed(&mT.strobeTime) > 2uL)
		{
			sAdc();
			mT.gS = eAdcFin;
		}
		break;
	case eAdcFin:
		if (mT.actChan == 3uL)
		{
			++mT.cycle;
			if(mT.cycle == eMax)
			{
				mT.gS = eFin;
			}
			else
				mT.gS = eStrobe;
		}
		break;
	case eFin:
		if(tElapsed(&mT.cycleTime) >= mT.cycleTarget)
		{
			mT.gS = eStart;
			tReset(&mT.initTime);
		}

		break;
	default:
		break;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	(void) hadc;
	if(mT.actChan == 1u)
	{
		mT.adcChan1[mT.cycle] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Start_IT(&hadc1);
	}
	else
		mT.adcChan2[mT.cycle] = HAL_ADC_GetValue(&hadc1);

	++mT.actChan;
}

