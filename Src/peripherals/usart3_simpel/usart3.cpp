/*
 * usart1.cpp
 *
 *  Created on: 27.06.2017
 *      Author: schramm
 */

#include "usart3.hpp"

UART_HandleTypeDef Usart3::huart;
bool Usart3::initialized=false;
Usart3 *Usart3::thisptr;

extern "C" {



int __io_putchar(int ch) {
	if(Usart3::initialized) {
		Usart3::thisptr->sendChar(ch);
	}
	return (ch);
}


}
