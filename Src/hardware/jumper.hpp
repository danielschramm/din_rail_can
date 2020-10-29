/*
 * jumper.hpp
 *
 *  Created on: 03.09.2013
 *      Author: daniel
 */

#ifndef JUMPER_HPP_
#define JUMPER_HPP_

#include "config/stm32plus.h"
#include "singleton.hpp"
#include "can.hpp"
#include "signal.hpp"
using namespace Gallant;

class Jumper {
public:
  uint8_t getSolderJumper() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    uint8_t result=0;
    result |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
    result |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)<<1;
    result |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)<<2;
    result |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)<<3;
    result |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)<<4;
  }


};

typedef Singleton<Jumper> _Jumper;
#define JUMPER _Jumper::Instance()

#endif /* JUMPER_HPP_ */
