/*
 * led.hpp
 *
 *  Created on: 18.05.2013
 *      Author: daniel
 */

#ifndef LED_HPP_
#define LED_HPP_



class Led {
private:
  GPIO_TypeDef *gpio;
  uint16_t pin;

public:
  Led() {
    gpio=NULL;
  }

  void setLed(GPIO_TypeDef* _gpio, uint16_t _pin) {
    gpio=_gpio;
    pin=_pin;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(gpio, &GPIO_InitStructure);
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);

  }
  void on() {
    if(gpio!=NULL) {
      HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
    }
  }
  void off() {
    if(gpio!=NULL) {
      HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    }
  }
  void tgl() {
    if(gpio!=NULL) {
    	  HAL_GPIO_TogglePin(gpio, pin);
    }
  }

};


#endif /* LED_HPP_ */
