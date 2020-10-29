/*
 * usart3.hpp
 *
 *  Created on: 04.11.2016
 *      Author: schramm
 */

#ifndef USART3_Usart3_HPP_
#define USART3_Usart3_HPP_


#include "stm32f1xx.h"
#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"


// only TX used and tested for debug output.
// PB10 / PB11 used

extern "C" {

int __io_putchar(int ch);
}

//using namespace cpp_freertos;

class Usart3 {
private:
	static UART_HandleTypeDef huart;
	static bool initialized;
	static Usart3 *thisptr;

	void initUsart() {
		GPIO_InitTypeDef GPIO_InitStructure;

		__HAL_RCC_USART3_CLK_ENABLE();

		// RX
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

		// TX
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Pin = GPIO_PIN_10;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


		huart.Instance=USART3;

		huart.Init.BaudRate = 115200;
		huart.Init.WordLength   = UART_WORDLENGTH_8B;
		huart.Init.StopBits     = UART_STOPBITS_1;
		huart.Init.Parity       = UART_PARITY_NONE;
		huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
		huart.Init.Mode         = UART_MODE_TX_RX;
		huart.Init.OverSampling = UART_OVERSAMPLING_16;


		if(HAL_UART_Init(&huart) != HAL_OK)
		{
			//Error_Handler();
			while(1);
		}

//		volatile uint16_t x = USART3->DR;
	}


public:

//	friend void Usart3_IRQHandler(void);
	friend int __io_putchar(int ch);

	Usart3() {
	}

	~Usart3() {

	}

	void init() {
		initUsart();
		initialized=true;
	}

	uint32_t sendChar(uint32_t ch) {
		uint8_t temp = ch;
		HAL_UART_Transmit(&huart,&temp,1,10);
		return (ch);
	}
};


#endif /* USART3_Usart3_HPP_ */
