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

}iCtx_t;

iCtx_t CCRAM_PLACING c1_64, CCRAM_PLACING c2_64;
iCtx_t CCRAM_PLACING c1_160, CCRAM_PLACING c2_160;

void fl_init(fltCtx_t **_pp)
{
   assrt(_pp != NULL);
   for (int i = 0; _pp[i]!=NULL; ++i) {
      ((iCtx_t*)_pp[i]->ctx)->v.r = 0;
   }
}

static uint32_t fl_i64(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   static fpa_t scI = _FPA_R(0.75);
   static fpa_t scP = _FPA_R(0.25);
   volatile uint32_t m = yM; (void)m;
   volatile fpa_t oldI = ctx->v;
   volatile fpa_t newI =  FPA_mult(scI, oldI);
   volatile fpa_t newP = FPA_IntMultFpa(yM, scP);
   volatile fpa_t newOut = {.r = newP.r + newI.r};
   ctx->v.r = newOut.r;

   return (uint32_t)ctx->v.i;
}
static uint32_t fl_i160(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   static fpa_t scI = _FPA_R(0.6);
   static fpa_t scP = _FPA_R(0.4);

   ctx->v = FPA_mult(scI, ctx->v);
   ctx->v.r += FPA_IntMultFpa(yM, scP).r;

   return (uint32_t)ctx->v.i;
}

// CCRAM_PLACING
fltCtx_t  cc1_64 = {.f = fl_i64, .ctx = &c1_64};
fltCtx_t  cc2_64 = {.f = fl_i64, .ctx = &c2_64};
fltCtx_t  cc1_160 = {.f = fl_i160, .ctx = &c1_160};
fltCtx_t  cc2_160 = {.f = fl_i160, .ctx = &c2_160};
