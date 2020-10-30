/*
 * JalousieAppEsp.hpp
 *
 *  Created on: 17.07.2013
 *      Author: daniel
 */

#ifndef JalousieAppEsp_HPP_
#define JalousieAppEsp_HPP_


#include "peripherals/can.hpp"
#include "hardware/buttonEsp.hpp"
#include "hardware/StatusLeds.hpp"
#include "hardware/JalousieMotor.hpp"
#include "peripherals/unique_id.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"

#include "CanThread.hpp"
#include "canWatchdog.hpp"

using namespace cpp_freertos;


class JalousieAppEsp : public Thread {
private:
	CanThread *canThread;
	UniqueId *uid;
	CanWatchdog *canWatchdog;
	uint32_t nodeId=0x500;


	ButtonEsp *buttons[12] = {
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr
	};

	JalousieMotor *motor[6] = {
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr
	};


public:
	JalousieAppEsp(CanThread *_canThread, UniqueId *_uid, CanWatchdog *_canWatchdog) :
		Thread("Jalousie", 1024, 3),
		canThread(_canThread), uid(_uid), canWatchdog(_canWatchdog) {

		canThread->newRxMessage.connect(this, &JalousieAppEsp::onNewCanMessage);
		if(! Start() ) {
			printf("Thread start fail\r\n");
		}

	}

	void Run() {


		buttons[0]=new ButtonEsp("Button1",canThread, GPIOA,GPIO_PIN_8,1,nodeId+0x01);
		buttons[1]=new ButtonEsp("Button2",canThread, GPIOC,GPIO_PIN_9,1,nodeId+0x02);
		buttons[2]=new ButtonEsp("Button3",canThread, GPIOC,GPIO_PIN_8,1,nodeId+0x03);
		buttons[3]=new ButtonEsp("Button4",canThread, GPIOC,GPIO_PIN_7,1,nodeId+0x04);
		buttons[4]=new ButtonEsp("Button5",canThread, GPIOC,GPIO_PIN_6,1,nodeId+0x05);
		buttons[5]=new ButtonEsp("Button6",canThread, GPIOB,GPIO_PIN_15,1,nodeId+0x06);
		buttons[6]=new ButtonEsp("Button7",canThread, GPIOB,GPIO_PIN_14,1,nodeId+0x07);
		buttons[7]=new ButtonEsp("Button8",canThread, GPIOB,GPIO_PIN_13,1,nodeId+0x08);
		buttons[8]=new ButtonEsp("Button9",canThread, GPIOB,GPIO_PIN_12,1,nodeId+0x09);
		buttons[9]=new ButtonEsp("Button10",canThread, GPIOC,GPIO_PIN_10,1,nodeId+0x0a);
		buttons[10]=new ButtonEsp("Button11",canThread, GPIOC,GPIO_PIN_0,1,nodeId+0x0b);
		buttons[11]=new ButtonEsp("Button12",canThread, GPIOC,GPIO_PIN_1,1,nodeId+0x0c);

		//		motor[0] = new JalousieMotor(GPIOC,GPIO_PIN_2,GPIOC,GPIO_PIN_3);
		//		motor[1] = new JalousieMotor(GPIOA,GPIO_PIN_0,GPIOA,GPIO_PIN_1);
		//		motor[2] = new JalousieMotor(GPIOA,GPIO_PIN_2,GPIOA,GPIO_PIN_3);
		//		motor[3] = new JalousieMotor(GPIOA,GPIO_PIN_4,GPIOA,GPIO_PIN_5);
		//		motor[4] = new JalousieMotor(GPIOA,GPIO_PIN_6,GPIOA,GPIO_PIN_7);
		//		motor[5] = new JalousieMotor(GPIOC,GPIO_PIN_4,GPIOC,GPIO_PIN_5);

		//STATUSLEDS.greenOn();
		printf("JalousieAppEsp Start\r\n");


		//canThread->CanThreadRxEventSender.insertSubscriber(CanThreadRxEventSourceSlot::bind(this, &JalousieAppEsp::onNewCanMessage));



		CanObject tx;


		while(1) {
			if(nodeId!=uid->getCanId()) {
				nodeId=uid->getCanId();
				for(uint8_t i=0; i<12; i++) {
					buttons[i]->setId(nodeId +1 +i);
				}
			}

			canWatchdog->update();
			Delay(1000);
		}
	}

	void onNewCanMessage(CanObject *rxObject) {
		if( (rxObject->id >= nodeId+0x10) && (rxObject->id < nodeId+0x16) ) {
			if(rxObject->len==1) {
				/*
				switch (rxObject->data[0]) {
				case 0:
						motor[rxObject->id - nodeId - 0x10]->driveStop();
					break;
				case 1:
						motor[rxObject->id - nodeId - 0x10]->driveUp();
					break;
				case 2:
						motor[rxObject->id - nodeId - 0x10]->driveDown();
					break;
				}
				 */
			}
		}

	}
};



#endif /* JalousieAppEsp_HPP_ */
