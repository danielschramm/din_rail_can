/*
 * CanThread.hpp
 *
 *  Created on: 25.10.2020
 *      Author: daniel
 */

#ifndef CANTHREAD_HPP_
#define CANTHREAD_HPP_

#include "FreeRTOS.h"
#include "task.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "semaphore.hpp"
#include "queue.hpp"
#include "peripherals/can.hpp"

using namespace cpp_freertos;

struct CanObject {
	bool extId=false;
	uint32_t id=0;
	uint8_t len=0;
	uint8_t data[8]={0,0,0,0,0,0,0,0};
};



typedef wink::signal<wink::slot<void (CanObject *)> > canSignal;

class CanThread : public Thread {
private:
	CanController canController;
	Queue *txQueue;
	Queue *rxQueue;
	CanObject rxObject;



public:

	canSignal newRxMessage;

	CanThread() : Thread("Can", 1024, 3)  {
		txQueue = new Queue(10, sizeof(CanObject));

		Start();
	}

	virtual void Run() {
		CanObject txObject;
		CanObject rxObject;
		for(;;) {
			if(txQueue->Dequeue(&txObject, 1)) {
				if(txObject.extId) {
					canController.canSendExt(txObject.id, txObject.len, txObject.data);
				} else {
					canController.canSendStd(txObject.id, txObject.len, txObject.data);
				}
			}

			if( canController.pollRx(&rxObject.id, &rxObject.extId, &rxObject.len, rxObject.data) ) {
				newRxMessage(&rxObject);
			}


			//if(rxQueue->Dequeue(&rxObject, 1)) {
//				CanThreadRxEventSender.raiseEvent(&rxObject);
//			}
		}
	}


	Queue* getTxQueue() {
		return txQueue;
	}
};


#endif /* CANTHREAD_HPP_ */
