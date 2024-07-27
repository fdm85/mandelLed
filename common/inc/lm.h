/**
 * @file      lm.h
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
 * @brief Linker map helpers
 * @defgroup Linker_Map Helps to manage memory section placement
 * @ingroup Dev_Helper
 * @{
 */

#ifndef INC_LM_H_
#define INC_LM_H_

/*! @brief Define memory section pragma for led data */
//#define CCRAM_PLACING __attribute__((section (".ccmram")))
#define CCRAM_PLACING
#endif /* INC_LM_H_ */
/** @}*/
