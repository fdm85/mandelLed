/**
 * @file      filter.h
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

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>

typedef struct fltCtx fltCtx_t;
typedef uint32_t (*_fl)(fltCtx_t *ctx, uint32_t yM);
struct fltCtx {
   _fl f;
   void* ctx;
};

extern fltCtx_t cc1_64, cc1_160;
extern fltCtx_t cc2_64, cc2_160;

#endif /* INC_FILTER_H_ */
