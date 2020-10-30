/*
 * canWatchdog.hpp
 *
 *  Created on: 02.02.2015
 *      Author: daniel
 */

#ifndef CPP_LIB_CANWATCHDOG_HPP_
#define CPP_LIB_CANWATCHDOG_HPP_


#include "CanThread.hpp"
#include "stm32f1xx_hal.h"

using namespace std;

/**
 * Send CAN Message with low priority on static address without local echo.
 * Every system on the bus will do the same
 * Receiving message and reset the watchdog counter. - Meaning, at least one other bus member is active and can bus not blocked.
 *
 * Also ID assignment to the Module is done here.
 * The Alive message sent is 4 byte long contains the CRC of the uid of the module
 * Configuration messages are 8 bytes long and also contain the assigned CanID
 *
 */

class CanWatchdog {
private:
	CanThread *canThread;
	bool msgReceived;
	const uint32_t canId=0x7ff;
	WWDG_HandleTypeDef   WwdgHandle;
	CanObject object;

	UniqueId *uid;

public:
	CanWatchdog(CanThread *_canThread, UniqueId *_uid) : canThread(_canThread), uid(_uid) {

		WwdgHandle.Instance = WWDG;

		WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
		WwdgHandle.Init.Window    = 10000;
		WwdgHandle.Init.Counter   = 10000;
		HAL_WWDG_Init(&WwdgHandle);
		//HAL_WWDG_Start(&WwdgHandle);

		/*
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//		IWDG_SetPrescaler(IWDG_Prescaler_64); // 40kHz base
//		IWDG_SetReload(6250); // P64, 6250 = 10sec
		IWDG_SetPrescaler(IWDG_Prescaler_256);
		IWDG_SetReload(10000);
		IWDG_ReloadCounter();
		IWDG_Enable();
		 */

		uint32_t crc=uid->getCRC();

		object.id=canId;
		object.len=4;
		object.data[0]=crc&0xff;
		crc>>=8;
		object.data[1]=crc&0xff;
		crc>>=8;
		object.data[2]=crc&0xff;
		crc>>=8;
		object.data[3]=crc&0xff;

		msgReceived=false;
		canThread->newRxMessage.connect(this, &CanWatchdog::remoteEvent);
	}
	virtual void remoteEvent(CanObject *rxObject) {
		if(rxObject->id==canId) {
			msgReceived=true;  // only reset counter if IRQ and Main loop are working
			if( (rxObject->len == 8)
					&&(rxObject->data[0] == object.data[0])
					&&(rxObject->data[1] == object.data[1])
					&&(rxObject->data[2] == object.data[2])
					&&(rxObject->data[3] == object.data[3])) {
				uint32_t newNodeId=0;
				newNodeId=rxObject->data[4];
				newNodeId<<=8;
				newNodeId|=rxObject->data[5];
				newNodeId<<=8;
				newNodeId|=rxObject->data[6];
				newNodeId<<=8;
				newNodeId|=rxObject->data[7];
				uid->setCanId(newNodeId);
			}
		}
	}
	virtual void update() {  // Called from scheduler as soft task 1/s
		if(msgReceived) {  // only reset counter if IRQ and Main loop are working
			//IWDG_ReloadCounter() ;
			HAL_WWDG_Refresh(&WwdgHandle);
			msgReceived=false;

			printf("Watchdog Message Received\r\n");
		}

		canThread->enqueue(object, 1);
	}
};



#endif /* CPP_LIB_CANWATCHDOG_HPP_ */
