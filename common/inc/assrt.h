/*
 * assrt.h
 *
 *  Created on: 01.05.2020
 *      Author: cgrue
 */

#ifndef ASSRT_H_
#define ASSRT_H_

#include "cmsis_compiler.h"
#include <stdbool.h>

#define assrt(b) if((b)==false) __BKPT(0)
#define assrtRet(b) if((b)==false) return

#endif /* ASSRT_H_ */
