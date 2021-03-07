// ***************************************************************************
// COMPANY      HIFI - SPECIAL electronics
//              www.hifi-special.com
//
// PROJECT      CampBox2 HB60038
//
// MODULE       AudioLights.c
//
// DESCRIPTION  audiolights functions
//
//-----------------------------------------------------------------------------
// TARGET       STM32L476VGT6 Cortex-M4
// COMPILER     emIDE
//
// ---------------------------------------------------------------------------
//	History
//
//  date       | changes                                            | Name
// ------------|----------------------------------------------------|---------
//  11.04.2016 | - taken from CampBox                               | HB
// ------------|----------------------------------------------------|---------
//  15.01.2017 | - Hue Chases, timer/VU/beat triggered              | HB
// ------------|----------------------------------------------------|---------
//  04.10.2017 | - sound fast: rising values without filter to be   | HB
//             |    faster                                          |
//             | - beat detection: take hysterese on falling edge   |
// ***************************************************************************
#include "project.h"
#include "main.h"
#include "timer.h"
#include "Nonvol.h"
#include "EEData.h"
#include "adc.h"
#include "amp.h"
#include "LEDs.h"
#include "DycoLED.h"
#include "RGB_LEDs.h"
#include "housingLED.h"
#include "RDA5807.h"
#include "AudioLights.h"


// ---------------- defines
#define AL_AUTOPWROFF_LEVEL           40
#define AL_TMR_LIGHTPROCESSING_TICKS  TMR_MS2TICKS(5)
#define AL_TMR_SOUNDPROCESSING_TICKS  TMR_MS2TICKS(5)


// ---------------- graph states
typedef enum {
   ALState_PowerOn =0,                 // power on state
   ALState_Off,                        // off state
   ALState_PwrOn,                      // switch on LEDs
   ALState_On,                         // on state
   ALState_OffInit,                    // init off-state

   ALState_NumberOfStates,             // always last in list
   ALState_Unchanged = 0xFF            // nothing happened
} eALGraphState_t;

// ---------------- all internal variables in a struct
sALE_t       sALE;

sALEESetup_t sALEESetupE;
const sALEESetup_t sALDefaultEESetup = 
{
    12,                   // uint8_t u8BeatHysterese
     5,                   // uint8_t u8NoiseGate
     8,                   // uint8_t fast filter factor
    32,                   // uint8_t slow filter factor
   180,                   // uint8_t u8HledBrightness
     0,                   // uint16_t crc16
};

static struct
{
   eALGraphState_t    eGraphState;
   Tmr_t              tSoundProcessing;           // timing sound processing   
   Tmr_t              tLightProcessing;           // timing light processing

   uint32_t           ulSoundFastIIR;             // sound fast filtered, x filter factor
   uint32_t           ulSoundSlowIIR;             // sound slow filtered, x filter factor
   Tmr_t              tMaxVolDec;                 // maximum volume decrement timer
   uint32_t           ulVolMin;                   // minimum value of music
   bool               bAudioBeat;                 // audio beat recognized

   bool               bLightsOn;                  // TRUE = Lights active request
   bool               bLightsAreOn;               // actual state
} sALS;

                                                                 

// ***************************************************************************
//  AL_vInit()
// ---------------------------------------------------------------------------
//
//  parameter: --
//  return:    --
//
// ***************************************************************************
void AL_vInit(void)
{
   // ------------- init variables
   sALS.bLightsOn = FALSE;

   sALE.bBlackout = FALSE;
   sALE.ulSoundNormalized = 0;


   sALEESetupE = sALDefaultEESetup;      // re-init the setup-block

   // -------------
   TMR_START(sALS.tMaxVolDec);
   TMR_START(sALS.tSoundProcessing);
   TMR_START(sALS.tLightProcessing);
}


//-----------------------------------------------------------------------------
//  AL_bGetBeat()
// ---------------------------------------------------------------------------
//
//  get actual beat state
//
//  parameter: --
//  return:    - actual beat state
//-----------------------------------------------------------------------------
bool AL_bGetBeat(void)
{
   return sALS.bAudioBeat;
}

//-----------------------------------------------------------------------------
// AL_vSoundProcessing
//
// sound processing
//
//-----------------------------------------------------------------------------
static void AL_vSoundProcessing(void)
{
      if (TMR_ELAPSED(sALS.tSoundProcessing) >= AL_TMR_SOUNDPROCESSING_TICKS)
      {   
         TMR_START(sALS.tSoundProcessing);

         // ---------- build music fast and average, max and min value
         if (ADC_bNewSoundSample())                // read flag and clear it
         {
            uint32_t  ulSoundRange;                // difference between sALE.ulVolMax and sALS.ulVolMin (for normalized sound)
            uint32_t  ulSound;                     // actual sound - sALS.ulVolMin (for normalized sound)
            uint32_t  ulNewAudioSample;            // to normalize to AL_MAX
            uint32_t  ulOldTemp;                   // IIR filter

            ulNewAudioSample = (ADC_ulGetSoundLevel() * AL_MAX) / (uint32_t)(ADC_MAX);
   
            if (sALEESetupE.u8SoundFastFiltFact)   // avoid div by zero
            {
               if (ulNewAudioSample > sALE.ulSoundFast)  // rising: no filter!
               {
                  sALE.ulSoundFast = ulNewAudioSample;
                  sALS.ulSoundFastIIR = ulNewAudioSample * sALEESetupE.u8SoundFastFiltFact;
               }
               else                                      // falling: filter
               {
                  ulOldTemp = sALS.ulSoundFastIIR;
                  ulOldTemp -= ulOldTemp / sALEESetupE.u8SoundFastFiltFact;
                  sALS.ulSoundFastIIR = ulOldTemp + ulNewAudioSample;
                  sALE.ulSoundFast = sALS.ulSoundFastIIR / sALEESetupE.u8SoundFastFiltFact;
               }
            }
            else
            {
               sALE.ulSoundFast = ulNewAudioSample;
            }
      
            if (sALEESetupE.u8SoundSlowFiltFact)  // avoid div by zero
            {
               ulOldTemp = sALS.ulSoundSlowIIR;
               ulOldTemp -= ulOldTemp / sALEESetupE.u8SoundSlowFiltFact;
               sALS.ulSoundSlowIIR = ulOldTemp + sALE.ulSoundFast;
               sALE.ulSoundSlow = sALS.ulSoundSlowIIR / sALEESetupE.u8SoundSlowFiltFact;
            }
            else
            {
               sALE.ulSoundSlow = 64;                              // filter factor = 0: take default value
            }
      
            if (sALE.ulSoundFast > sALE.ulVolMax)                  // new sound max value
               sALE.ulVolMax = sALE.ulSoundFast;
         
            if (sALE.ulSoundFast < sALS.ulVolMin)                  // new sound min value
               sALS.ulVolMin = sALE.ulSoundFast;

            if (TMR_ELAPSED(sALS.tMaxVolDec) > TMR_MS2TICKS(1000)) // decrement max value and
            {                                                      // increment min value every sec
               if (sALE.ulVolMax)
               {
                  sALE.ulVolMax --;

                  if (sALS.ulVolMin < sALE.ulVolMax-1)
                     sALS.ulVolMin ++;
               }
               TMR_START(sALS.tMaxVolDec);
            }

            // ---------- normalized sound
            ulSoundRange = sALE.ulVolMax - sALS.ulVolMin;          // calculate normalized sound
            if (sALE.ulSoundFast > sALS.ulVolMin)
            {
               ulSound = sALE.ulSoundFast - sALS.ulVolMin;
            }
            else
            {
               ulSound = 0ul;
            }

            if (ulSoundRange)
            {
               sALE.ulSoundNormalized = (AL_MAX * ulSound) / ulSoundRange;   // AL_MAX = x 0..255
            }
            else
            {
               sALE.ulSoundNormalized = 0ul;
            }
         }
      }
}


// ----------------------------------------------------------------------------
//
//   Graph States
//
// ----------------------------------------------------------------------------
static void vA_PowerOn(void)
{
   // never called
}

// ----------------------------------------------------------------------------
//  off state
// ----------------------------------------------------------------------------
static void vA_Off(void)
{
   sALS.bLightsAreOn = FALSE;
}

// ----------------------------------------------------------------------------
//  AudioLights switch on
// ----------------------------------------------------------------------------
static void vA_PwrOn(void)
{
   sALS.bLightsAreOn = TRUE;
}

// ----------------------------------------------------------------------------
//  on state
// ----------------------------------------------------------------------------
static void vA_On(void)
{
   uint_least8_t   u8Idx;                    // lamp index
   uint32_t        ulTmp;
   uint8_t         u8VolFactor;
   

   TMR_START(sALS.tLightProcessing);

   // ---------- beat detection
   if (sALE.ulSoundFast > sALE.ulSoundSlow)
   {
      if (sALS.bAudioBeat == FALSE)
      {
         if (sALS.bLightsOn)
         {
            sALS.bTriggerNextChaseStep = TRUE;

            // ------------- trigger housing LED
            if (sALE.ulSoundFast > sALEESetupE.u8HledBrightness)
               sALS.ulHousingLED = sALEESetupE.u8HledBrightness;
            else
               sALS.ulHousingLED = sALE.ulSoundFast;
         }
         sALS.bAudioBeat = TRUE;
      }
   }
   else if ((sALE.ulSoundFast + sALEESetupE.u8BeatHysterese) < sALE.ulSoundSlow)
   {
      sALS.bAudioBeat = FALSE;
   }
}

// ----------------------------------------------------------------------------
//  init off state
// ----------------------------------------------------------------------------
static void vA_OffInit(void)
{
}


//-----------------------------------------------------------------------------
// AL_vGraph
//
// AudioLights graph
//
//-----------------------------------------------------------------------------
void AL_vGraph(void)
{
   static void (* const ActTabGr[])(void) =
   {
      vA_PowerOn,
      vA_Off,
      vA_PwrOn,
      vA_On,
      vA_OffInit,
   };
   eALGraphState_t eNewState;


   // ------------------ sound processing ------------------------------------
   AL_vSoundProcessing();

   // ------------------ Graph -----------------------------------------------
   sNonvolE.uLastTickerInstance = AL_MODULE_ID;
   eNewState = ALState_Unchanged;

   switch(sALS.eGraphState)
   {
      case ALState_PowerOn:
         eNewState = ALState_Off;
         break;

      case ALState_Off:
         if(sALS.bLightsOn)
            eNewState = ALState_PwrOn;
         break;

      case ALState_PwrOn:
         eNewState = ALState_On;
         break;

      case ALState_On:
         if(!sALS.bLightsOn)
            eNewState = ALState_OffInit;
         else if(TMR_ELAPSED(sALS.tLightProcessing) >= AL_TMR_LIGHTPROCESSING_TICKS)
            eNewState = ALState_On;
         break;

      case ALState_OffInit:
         eNewState = ALState_Off;
         break;

      default:
         Main_vFatalError(AL_MODULE_ID, E_ILLEGAL_CASE);
         return;//never taken
   }


   if(eNewState != ALState_Unchanged)
   {
      if(eNewState < numberof(ActTabGr))
         ActTabGr[eNewState]();
      else
         Main_vFatalError(AL_MODULE_ID, E_ILLEGAL_ACT);
      sALS.eGraphState = eNewState;
   }
}
