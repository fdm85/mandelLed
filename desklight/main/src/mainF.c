/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "peripheral.h"
#include "leds.h"
#include "animations.h"
#include "matrix.h"
#include "com.h"
#include "cmsis_compiler.h"
#include "crc.h"
#include "stm32f1xx_hal_conf.h"


static uint8_t col = 20;
static uint8_t index = 0;
static void cycleColors(mAnim_t* ctx)
{
  led_LedLogicInit(ctx->lcd_ctx);
  for (uint32_t i = 0uL; i < 6uL; ++i) {
    led_setLedToColor(ctx->lcd_ctx, i, ((index + 0) % 3) ? 0 : col, ((index + 1) % 3) ? 0 : col, ((index + 2) % 3) ? 0 : col);
  }
  ++index;
}
mAnim_t anim_main = { .fpRend = cycleColors, .lcd_ctx = &lcd_main, .triggerTimeMs = 1500uL, .puState = done};
mAnim_t anim_matrix = { .fpRend = mtrx_anim, .lcd_ctx = &lcd_matrix, .triggerTimeMs = 10uL, .puState = done};
extern void led_startTransmitData(LedChainDesc_t* lcd);
static void cyclicReSend(mAnim_t *ctx) {

  switch (ctx->state) {
  case e_render:
    ctx->a = HAL_GetTick();
    ctx->fpRend(ctx);
    ctx->b = HAL_GetTick() - ctx->a;
    ctx->state = e_StartDma;
    break;

  case e_StartDma:
//		ctx->c = HAL_GetTick();
//		ctx->d = HAL_GetTick() - ctx->c;
//		ctx->e = HAL_GetTick();
    ctx->lastToggle = HAL_GetTick();
    ctx->lcd_ctx->lRawNew->dS = e_fadeIn;
    ctx->lcd_ctx->lRawNew->rS = e_Precursor;
    led_txRaw(ctx->lcd_ctx);
    ctx->state = e_waitDmaDone;
    break;

  case e_waitDmaDone:
    if ((ctx->lcd_ctx->lRawNew->rS == e_done)
        && ((HAL_GetTick() - ctx->lastToggle) > ctx->triggerTimeMs))
      ctx->state = e_render;
    break;

  default:
    __BKPT(0);
    break;
  }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
	lcd->lRawNew->dS = e_FirstHalf;
	led_txRaw(lcd);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	LedChainDesc_t* lcd = (htim == &htim3) ? &lcd_matrix : &lcd_main;
	lcd->lRawNew->dS = e_SecondHalf;
	led_txRaw(lcd);

//	else
//	{
//		anim_main.sendLock = false;
//		anim_main.e = HAL_GetTick() - anim_main.e;
//		anim_main.f = HAL_GetTick();
//	}
//  __BKPT(0);
	/* NOTE : This function should not be modified, when the callback is needed,
	 the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
	 */
}

int main(void)
{
	initClock();
	initPeripherals();
	MX_CRC_Init();
	anim_main.AnimMode = anim_rnd3;
	anim_matrix.AnimMode = anim_msqDrv;
//	anim_setMode(&lcd_main, anim_rnd3);
	led_setBrightnessTruncation(&lcd_main, 1uL, 1uL);
//	led_setBrightnessTruncation(&lcd_matrix, 1uL, 1uL);
	led_LedLogicInit(&lcd_main);

//	led_LedLogicInit(&lcd_matrix);
//	mtrx_Init();
	__enable_irq();
	for (;;)
	{
//		maintainModeSwitch();
//		msgeq_ticker();
		cyclicReSend(&anim_main);
//		cyclicReSend(&anim_matrix);
	}
}

uint32_t hrng;

HAL_CRC_StateTypeDef HAL_RNG_GenerateRandomNumber(void * vd, uint32_t *random32bit)
{
	(void)vd;
	uint32_t temp = HAL_GetTick();

	*random32bit = HAL_CRC_Accumulate(&hcrc, &temp, 1);

	return  hcrc.State;
}
