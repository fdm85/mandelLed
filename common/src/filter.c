/**
 * @file      filter.c
 * @authors   Clemens Grünberger
 * @copyright 2022  Clemens Grünberger
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @brief Filter implementation
 * @ingroup Filter
 */


#include "filter.h"
#include "lm.h"
#include "fpa.h"
#include "assrt.h"

typedef struct iCtx{
   fpa_t v;
   uint32_t oM;
}iCtx_t;

iCtx_t CCRAM_PLACING c1_64 = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_64 = {.v = {.r = 0}, .oM = 0};
iCtx_t CCRAM_PLACING c1_160 = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_160 = {.v = {.r = 0}, .oM = 0};
iCtx_t CCRAM_PLACING c1_400 = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_400 = {.v = {.r = 0}, .oM = 0};
//iCtx_t CCRAM_PLACING c1_1k = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_1k = {.v = {.r = 0}, .oM = 0};
//iCtx_t CCRAM_PLACING c1_2k5 = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_2k5 = {.v = {.r = 0}, .oM = 0};
//iCtx_t CCRAM_PLACING c1_6k25 = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_6k25 = {.v = {.r = 0}, .oM = 0};
//iCtx_t CCRAM_PLACING c1_16k = {.v = {.r = 0}, .oM = 0}, CCRAM_PLACING c2_16k = {.v = {.r = 0}, .oM = 0};

static uint32_t fl_i64(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   static fpa_t CCRAM_PLACING scI = _FPA_R(0.65);
   static fpa_t CCRAM_PLACING scP = _FPA_R(0.35);
   static fpa_t CCRAM_PLACING scDU = _FPA_R(0.7);
   static fpa_t CCRAM_PLACING scDD = _FPA_R(0.3);
   static fpa_t CCRAM_PLACING scOut = _FPA_R(1.6);

   ctx->v = FPA_mult(scI, ctx->v);
   ctx->v.r += FPA_IntMultFpa(yM, scP).r;
   int32_t dif = (int32_t)yM - (int32_t)ctx->oM;
   ctx->v.r += FPA_IntMultFpa(dif, (dif > 0) ? scDU : scDD).r;
   ctx->oM = yM;

   return (uint32_t)(FPA_mult(scOut, ctx->v).i);
}
static uint32_t fl_i160(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   static fpa_t CCRAM_PLACING scI = _FPA_R(0.6);
   static fpa_t CCRAM_PLACING scP = _FPA_R(0.4);
   static fpa_t CCRAM_PLACING scDU = _FPA_R(0.5);
   static fpa_t CCRAM_PLACING scDD = _FPA_R(0.3);
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.8);

   ctx->v = FPA_mult(scI, ctx->v);
   ctx->v.r += FPA_IntMultFpa(yM, scP).r;
   int32_t dif = (int32_t)yM - (int32_t)ctx->oM;
   ctx->v.r += FPA_IntMultFpa(dif, (dif > 0) ? scDU : scDD).r;
   ctx->oM = yM;

   return (uint32_t)(FPA_mult(scOut, ctx->v).i);
}
static uint32_t fl_i400(fltCtx_t *ctx_p, uint32_t yM)
{
   (void)ctx_p;
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.7);
   return (uint32_t)FPA_IntMultFpa(yM, scOut).i;
}
static uint32_t fl_i1k(fltCtx_t *ctx_p, uint32_t yM)
{
   (void)ctx_p;
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.7);
   return (uint32_t)FPA_IntMultFpa(yM, scOut).i;
}
static uint32_t fl_i2k5(fltCtx_t *ctx_p, uint32_t yM)
{
   (void)ctx_p;
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.65);
   return (uint32_t)FPA_IntMultFpa(yM, scOut).i;
}
static uint32_t fl_i6k25(fltCtx_t *ctx_p, uint32_t yM)
{
   (void)ctx_p;
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.6);
   return (uint32_t)FPA_IntMultFpa(yM, scOut).i;
}
static uint32_t fl_i16k(fltCtx_t *ctx_p, uint32_t yM)
{
   (void)ctx_p;
   static fpa_t CCRAM_PLACING scOut = _FPA_R(0.5);
   return (uint32_t)FPA_IntMultFpa(yM, scOut).i;
}

fltCtx_t CCRAM_PLACING cc1_64 = {.f = fl_i64, .ctx = &c1_64};
fltCtx_t CCRAM_PLACING cc2_64 = {.f = fl_i64, .ctx = &c2_64};
fltCtx_t CCRAM_PLACING cc1_160 = {.f = fl_i160, .ctx = &c1_160};
fltCtx_t CCRAM_PLACING cc2_160 = {.f = fl_i160, .ctx = &c2_160};
fltCtx_t CCRAM_PLACING cc1_400 = {.f = fl_i400, .ctx = &c1_400};
fltCtx_t CCRAM_PLACING cc2_400 = {.f = fl_i400, .ctx = &c2_400};
fltCtx_t CCRAM_PLACING cc1_1k = {.f = fl_i1k, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc2_1k = {.f = fl_i1k, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc1_2k5 = {.f = fl_i2k5, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc2_2k5 = {.f = fl_i2k5, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc1_6k25 = {.f = fl_i6k25, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc2_6k25 = {.f = fl_i6k25, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc1_16k = {.f = fl_i16k, .ctx = NULL};
fltCtx_t CCRAM_PLACING cc2_16k = {.f = fl_i16k, .ctx = NULL};
