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


typedef struct iCtx{
   uint32_t v;

}iCtx_t;

iCtx_t CCRAM_PLACING c1_64 = {.v = 0uL}, CCRAM_PLACING c2_64 = {.v = 0uL};
iCtx_t CCRAM_PLACING c1_160 = {.v = 0uL}, CCRAM_PLACING c2_160 = {.v = 0uL};

static uint32_t fl_i64(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   (void)yM;

   ctx->v *=  3uL;
   ctx->v /=  4uL;

   ctx->v += yM/4uL;
   return ctx->v;
}
static uint32_t fl_i160(fltCtx_t *ctx_p, uint32_t yM)
{
   iCtx_t *ctx = (iCtx_t*)(ctx_p->ctx);
   (void)yM;

   ctx->v *=  3uL;
   ctx->v /=  5uL;

   ctx->v += (2*yM)/5uL;
   return ctx->v;
}

// CCRAM_PLACING
fltCtx_t  cc1_64 = {.f = fl_i64, .ctx = &c1_64};
fltCtx_t  cc2_64 = {.f = fl_i64, .ctx = &c2_64};
fltCtx_t  cc1_160 = {.f = fl_i160, .ctx = &c1_160};
fltCtx_t  cc2_160 = {.f = fl_i160, .ctx = &c2_160};
