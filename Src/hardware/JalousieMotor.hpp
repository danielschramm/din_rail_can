/*
 * JalousieMotor.hpp
 *
 *  Created on: 26.10.2020
 *      Author: daniel
 */

#ifndef JALOUSIEMOTOR_HPP_
#define JALOUSIEMOTOR_HPP_

#include "FreeRTOS.h"
#include "thread.hpp"


class JalousieMotor {
private:
	GPIO_TypeDef* gpioDir;
	uint16_t pinDir;
	GPIO_TypeDef* gpioEn;
	uint16_t pinEN;


public:
	JalousieMotor(GPIO_TypeDef* _gpioDir, uint16_t _pinDir,
			GPIO_TypeDef* _gpioEn, uint16_t _pinEn) :
			gpioDir(_gpioDir), pinDir(_pinDir),
			gpioEn(_gpioEn), pinEN(_pinEn) {

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.Pin = pinDir;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(gpioDir, &GPIO_InitStructure);
		GPIO_InitStructure.Pin = pinEN;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_Init(gpioEn, &GPIO_InitStructure);

		HAL_GPIO_WritePin(gpioDir, pinDir, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(gpioEn, pinEN, GPIO_PIN_RESET);

	}


	void driveUp() {
		driveStop();
		HAL_GPIO_WritePin(gpioDir, pinDir, GPIO_PIN_SET);
		HAL_GPIO_WritePin(gpioEn, pinEN, GPIO_PIN_SET);
	}
	void driveDown() {
		driveStop();
		HAL_GPIO_WritePin(gpioDir, pinDir, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(gpioEn, pinEN, GPIO_PIN_SET);
	}
	void driveStop() {
		HAL_GPIO_WritePin(gpioEn, pinEN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(gpioDir, pinDir, GPIO_PIN_RESET);
	}

};



#endif /* JALOUSIEMOTOR_HPP_ */
