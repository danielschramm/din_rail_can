/*
 * StatusLeds.hpp
 *
 *  Created on: 17.05.2013
 *      Author: daniel
 */

#ifndef STATUSLEDS_HPP_
#define STATUSLEDS_HPP_

//#include "system/singleton.hpp"
#include "led.hpp"

class StatusLeds {
private:
  Led leds[4];

public:
  void init () {

    leds[0].setLed(GPIOB,GPIO_PIN_0);
    //leds[2].setLed(GPIOB,GPIO_PIN_10);  // uart
    leds[1].setLed(GPIOB,GPIO_PIN_1);
    //leds[3].setLed(GPIOB,GPIO_PIN_11);  // uart

  }
  virtual void redOn() {
    leds[0].on();
  }
  virtual void redOff() {
    leds[0].off();
  }
  virtual void redTgl() {
    leds[0].tgl();
  }
  virtual void yellowOn() {
    leds[1].on();
  }
  virtual void  yellowOff() {
    leds[1].off();
  }
  virtual void yellowTgl() {
    leds[1].tgl();
  }
  virtual void greenOn() {
    leds[2].on();
  }
  virtual void greenOff() {
    leds[2].off();
  }
  virtual void greenTgl() {
    leds[2].tgl();
  }
  virtual void green2On() {
    leds[3].on();
  }
  virtual void green2Off() {
    leds[3].off();
  }
  virtual void green2Tgl() {
    leds[3].tgl();
  }
};

//typedef Singleton<StatusLeds> _StatusLeds;
//#define STATUSLEDS _StatusLeds::Instance()

#endif /* STATUSLEDS_HPP_ */
