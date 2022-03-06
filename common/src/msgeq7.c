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
#include "filter.h"

#define OFFSET_ADC	300uL

typedef enum {
   eInit = 0u, eStart, eStrobe, eAdcStart, eAdcFin, eFin,
} gS_t;

typedef struct mT_s {
   time_t initTime;
   time_t cycleTime;
   uint32_t cycleTarget;
   time_t strobeTime;
   uint32_t strobeTarget;
   uint32_t adcChan1[eMax];
   uint32_t adcChan2[eMax];
   uint32_t adcChan12[eMax];
   uint32_t adcChan22[eMax];
   fltCtx_t *cc1[eMax];
   fltCtx_t *cc2[eMax];
   volatile uint8_t actChan;
   volatile uint8_t chan2;
   volatile msgeq7Freq cycle;
   gS_t gS;
} mT_t;

static mT_t mT = { .cc1 = { &cc1_64, &cc1_160, &cc1_400, &cc1_1k, &cc1_2k5, &cc1_6k25, &cc1_16k }, .cc2 = { &cc2_64, &cc2_160, &cc2_400, &cc2_1k, &cc2_2k5, &cc2_6k25, &cc2_16k} };

static void sInit(void) {
   assrt(mT.gS == eInit);
   mT.cycleTarget = 40uL;
   mT.strobeTarget = 1uL;
   tStart(&mT.initTime);
   HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_SET);
}

static void exInit(void) {
   assrt(mT.gS == eInit);
   tReset(&mT.initTime);
   HAL_GPIO_WritePin(MS_RESET_GPIO_Port, MS_RESET_Pin, GPIO_PIN_RESET);
}

static void sStrobe(void) {
   assrt(mT.gS == eStrobe);
   tStart(&mT.strobeTime);
   HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_SET);
}

static void exStrobe(void) {
   assrt(mT.gS == eStrobe);
   tStart(&mT.strobeTime);
   HAL_GPIO_WritePin(MS_STROBE_GPIO_Port, MS_STROBE_Pin, GPIO_PIN_RESET);
}

static void sStart(void) {
   assrt(mT.gS == eStart);
   tStart(&mT.initTime);
   tStart(&mT.cycleTime);
   mT.cycle = e63Hz;
}

static void sAdc(void) {
   assrt(mT.gS == eAdcStart);
   uint32_t adcState = HAL_ADC_GetState(&hadc2);
   bool rdy = ((adcState & HAL_ADC_STATE_READY) != 0uL);
   assrt(rdy);
   adcState = HAL_ADC_GetState(&hadc3);
   rdy = ((adcState & HAL_ADC_STATE_READY) != 0uL);
   assrt(rdy);
   mT.actChan = 1u;
   tReset(&mT.strobeTime);
   HAL_ADC_Start_IT(&hadc2);
   HAL_ADC_Start_IT(&hadc3);
}

void msgeq_ticker(void) {

   switch (mT.gS) {
   // min reset pulse width 100nS
   case eInit:
      if (tElapsed(&mT.initTime) >= 1uL) {
         exInit();
         mT.gS = eStart;
      } else {
         if (!mT.initTime.started)
            sInit();
      }
      break;
      // reset to strobe delay 72uS
   case eStart:
      if (tElapsed(&mT.initTime) >= 1uL) {
         mT.gS = eStrobe;
      } else {
         if (!mT.initTime.started)
            sStart();
      }

      break;
      // min strobe pulse width 18uS
      // min stobe to strobe delay 72us (falling edge to falling edge)
   case eStrobe:
      if (tElapsed(&mT.strobeTime) >= 1uL) {
         exStrobe();
         mT.gS = eAdcStart;
      } else {
         if (!mT.strobeTime.started)
            sStrobe();
      }
      break;
      // min output settling time 36uS
   case eAdcStart:
      if (tElapsed(&mT.strobeTime) > 2uL) {
         sAdc();
         mT.gS = eAdcFin;
      }
      break;
   case eAdcFin:
      if (mT.actChan == 3uL) {
         ++mT.cycle;
         if (mT.cycle == eMax) {
            mT.gS = eFin;
         } else
            mT.gS = eStrobe;
      }
      break;
   case eFin:
      if (tElapsed(&mT.cycleTime) >= mT.cycleTarget) {
         mT.gS = eStart;
         tReset(&mT.initTime);
      }

      break;
   default:
      break;
   }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {

   uint32_t t;
   if (hadc == &hadc2) {
      fltCtx_t *ctx = mT.cc1[mT.cycle];
      t = HAL_ADC_GetValue(&hadc2);
      mT.adcChan1[mT.cycle] = (t > OFFSET_ADC) ? (t - OFFSET_ADC) : 0uL;
      if(ctx != NULL)
         mT.adcChan12[mT.cycle] =  ctx->f(ctx, mT.adcChan1[mT.cycle]);
   } else if (hadc == &hadc3) {
      fltCtx_t *ctx = mT.cc2[mT.cycle];
      t = HAL_ADC_GetValue(&hadc2);
      mT.adcChan2[mT.cycle] = (t > OFFSET_ADC) ? (t - OFFSET_ADC) : 0uL;
      if(ctx != NULL)
               mT.adcChan22[mT.cycle] =  ctx->f(ctx, mT.adcChan2[mT.cycle]);
   } else {
      if (hadc == &hadc1)
         ADC_ConvCpltCallback(hadc);
      return;
   }

   ++mT.actChan;
}

uint32_t getLChanVal(msgeq7Freq freq) {
   uint32_t ret = mT.adcChan1[freq];
   return ret;
}
uint32_t getRChanVal(msgeq7Freq freq) {
   uint32_t ret = mT.adcChan2[freq];
   return ret;
}
uint32_t getLChanVal2(msgeq7Freq freq) {
   uint32_t ret = mT.adcChan12[freq];
   return ret;
}
uint32_t getRChanVal2(msgeq7Freq freq) {
   uint32_t ret = mT.adcChan22[freq];
   return ret;
}

uint32_t getLSum(msgeq7Freq freq) {
   (void) freq;
   return (getLChanVal2(e63Hz) + getLChanVal2(e160Hz) + getLChanVal2(e400Hz) + getLChanVal2(e1kHz) + getLChanVal2(e2_5kHz) + getLChanVal2(e6_25kHz) + getLChanVal2(e16kHz));
}
uint32_t getRSum(msgeq7Freq freq) {
   (void) freq;
   return (getRChanVal2(e63Hz) + getRChanVal2(e160Hz) + getRChanVal2(e400Hz) + getRChanVal2(e1kHz) + getRChanVal2(e2_5kHz) + getRChanVal2(e6_25kHz) + getRChanVal2(e16kHz));
}
