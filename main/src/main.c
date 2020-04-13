/*
 * main.c
 *
 *  Created on: 11.04.2020
 *      Author: cgrue
 */

#include "cmsis_compiler.h"
#include "peripheral.h"

int main(void)
{
	initClock();

	for(;;)
	{
		__NOP();
	}
}
