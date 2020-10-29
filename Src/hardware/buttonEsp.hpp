/*
 * buttonEsp.hpp
 *
 *  Created on: 25.10.2020
 *      Author: Daniel Schramm
 */


#ifndef BUTTON_ESP_HPP_
#define BUTTON_ESP_HPP_

#include "thread.hpp"
#include "CanThread.hpp"


/*
 *  class button takes port and pin and polarity in the constructor.
 *  It runs as a task polling the input every 25ms
 *  polarity 0=active high, 1=active low
 *  reaction is the following:
 *  event pressed is raised when the button is hold down immediately
 *  event released is raised when the button is released after a pressed event
 *  event longPressed is raised when the button is hold down for 1second
 *  event longReleased is raised when the button is released after a longPressed event
 *  event doubleClick is raised if the button is pressed, released and pressed again within one second
 *  event shortClick is raised if the button is only pressed once shortly
 */

using namespace cpp_freertos;

class ButtonEsp : public Thread {
private:

	enum class ButtonEvent : uint8_t {
		ButtonIdleReleased=0x00,
				ButtonPressed=0x1,
				ButtonLongPressed=0x2,
				ButtonLongReleased=0x3,
				ButtonDoubleClick=0x4,
				ButtonShortClick=0x5,
	};

	enum class State {
		idle,
		firstActive,
		gap,
		secondActive,
		waitRelease,
		waitReleaseDouble
	};

	Queue *canTxQueue;
	GPIO_TypeDef *gpio;
	uint16_t pin;
	uint8_t polarity;
	uint32_t id=0;
	uint16_t activeFor;
	uint16_t idleFor;
	uint8_t lastLevel;


	State state;
	uint8_t idleCount=0;

	void emitEvent(ButtonEvent event) {
		CanObject tx;
		tx.id=id;
		tx.extId=false;
		tx.len=1;
		tx.data[0]=static_cast< typename std::underlying_type<ButtonEvent>::type >(event);

		canTxQueue->Enqueue(&tx, 100);
	}

	bool getPin() {
		if (polarity) {
			return(! HAL_GPIO_ReadPin(gpio,pin));
		} else {
			return(HAL_GPIO_ReadPin(gpio,pin));
		}
	}

	void emitPressed() {
		emitEvent(ButtonEvent::ButtonPressed);
	}

	void emitReleased() {
		emitEvent(ButtonEvent::ButtonIdleReleased);
	}

	void emitIdle() {
		//		idleCount++;
		//		if(idleCount>=100) {
		//			idleCount=0;
		//			emitEvent(ButtonEvent::ButtonIdleReleased);
		//		}
	}

	void emitLongPressed() {
		emitEvent(ButtonEvent::ButtonLongPressed);
	}

	void emitLongReleased() {
		emitEvent(ButtonEvent::ButtonLongReleased);
		emitEvent(ButtonEvent::ButtonIdleReleased);
	}

	void emitDoubleClick() {
		emitEvent(ButtonEvent::ButtonDoubleClick);
	}

	void emitShortClick() {
		emitEvent(ButtonEvent::ButtonShortClick);
	}

public:

	// Initialize with port and pin.
	ButtonEsp(const std::string _pcName, Queue *_canTxQueue, GPIO_TypeDef* _gpio, uint16_t _pin, uint8_t _polarity, uint32_t _id=0) :
		Thread(_pcName, configMINIMAL_STACK_SIZE, 1),
		canTxQueue(_canTxQueue), gpio(_gpio),
		pin(_pin), polarity(_polarity), id(_id) {

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.Pin = pin;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(gpio, &GPIO_InitStructure);
		idleFor = 0;
		activeFor = 0;
		state=State::idle;
		lastLevel = 0;

		if(! Start() ) {
			printf("Thread start fail\r\n");
		}
	}

	virtual void Run() {
		for(;;) {
			if(getPin()) {
				if(lastLevel==0) {
					emitPressed();
				}
				lastLevel=1;
				switch (state) {
				case State::idle:
					activeFor=0;
					state=State::firstActive;
					break;
				case State::firstActive:
					if(activeFor<20) {
						activeFor++;
					} else {
						emitLongPressed();
						state=State::waitRelease;
						activeFor=0;
					}
					break;
				case State::gap:
					state=State::secondActive;
					activeFor=0;
					break;
				case State::secondActive:
					emitDoubleClick();
					state=State::waitReleaseDouble;
					activeFor=0;
					break;
				case State::waitRelease:
					// do nothing
					break;
				default:
					break;
				}
			} else {
				if(lastLevel) {
					emitReleased();
				}
				lastLevel=0;
				switch (state) {
				case State::idle:
					emitIdle();
					break;
				case State::firstActive:
					state=State::gap;
					activeFor=0;
					break;
				case State::gap:
					if(activeFor<20) {
						activeFor++;
					} else {
						state=State::idle;
						emitShortClick();
						activeFor=0;
					}
					break;
				case State::secondActive:
					// do nothing
					break;
				case State::waitRelease:
					emitLongReleased();
					state=State::idle;
					break;
				case State::waitReleaseDouble:
					state=State::idle;
					break;
				default:
					break;
				}
			}
			Delay(25);
		}
	}
};



#endif /* BUTTON_ESP_HPP_ */
