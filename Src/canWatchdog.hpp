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
 */

class CanWatchdog {
private:
	CanThread *canThread;
	bool msgReceived;
	const uint32_t canId=0x1ffffffe;
	WWDG_HandleTypeDef   WwdgHandle;
public:
	CanWatchdog(CanThread *_canThread) : canThread(_canThread) {

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
		msgReceived=false;
		canThread->newRxMessage.connect(this, &CanWatchdog::remoteEvent);
	}
	virtual void remoteEvent(CanObject *rxObject) {
		if(rxObject->id==canId) {
			msgReceived=true;  // only reset counter if IRQ and Main loop are working
		}
	}
	virtual void update() {  // Called from scheduler as soft task 1/s
//		if(msgReceived) {  // only reset counter if IRQ and Main loop are working
			//IWDG_ReloadCounter() ;
			HAL_WWDG_Refresh(&WwdgHandle);
			msgReceived=false;

			printf("Watchdog Message Received\r\n");
//		}
//		uint8_t data[8];
//		canController->canSendExt(canId,0,data);
	}
};



#endif /* CPP_LIB_CANWATCHDOG_HPP_ */
