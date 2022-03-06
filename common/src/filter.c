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
 * @brief // ToDo
 * @defgroup // ToDo
 * @ingroup // ToDo
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

static uint32_t fl_i64(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   static fpa_t CCRAM_PLACING scI = _FPA_R(0.65);
   static fpa_t CCRAM_PLACING scP = _FPA_R(0.35);
   static fpa_t CCRAM_PLACING scDU = _FPA_R(0.5);
   static fpa_t CCRAM_PLACING scDD = _FPA_R(0.3);
   static fpa_t CCRAM_PLACING scOut = _FPA_R(1.5);

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
   static fpa_t CCRAM_PLACING scOut = _FPA_R(1.05);

   ctx->v = FPA_mult(scI, ctx->v);
   ctx->v.r += FPA_IntMultFpa(yM, scP).r;
   int32_t dif = (int32_t)yM - (int32_t)ctx->oM;
   ctx->v.r += FPA_IntMultFpa(dif, (dif > 0) ? scDU : scDD).r;
   ctx->oM = yM;

   return (uint32_t)(FPA_mult(scOut, ctx->v).i);
}

fltCtx_t CCRAM_PLACING cc1_64 = {.f = fl_i64, .ctx = &c1_64};
fltCtx_t CCRAM_PLACING cc2_64 = {.f = fl_i64, .ctx = &c2_64};
fltCtx_t CCRAM_PLACING cc1_160 = {.f = fl_i160, .ctx = &c1_160};
fltCtx_t CCRAM_PLACING cc2_160 = {.f = fl_i160, .ctx = &c2_160};
