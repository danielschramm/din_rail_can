/*
 * can.hpp
 *
 *  Created on: 05.05.2013
 *      Author: daniel
 */
#ifndef _CAN_HPP_
#define _CAN_HPP_

#include "stm32f1xx_hal.h"
#include "event.hpp"
#include <cstdio>


using namespace std;

// tx example: 		CANCONTROLLER.canSendExt(0x123,3,(const uint8_t *)"123");

extern "C" {
void __attribute__ ((interrupt("IRQ"))) USB_LP_CAN1_RX0_IRQHandler(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle);
}

class CanController {
private:
	CAN_HandleTypeDef CanHandle;

	uint8_t               rxData[8];
	CAN_RxHeaderTypeDef   rxHeader;

	void CanErrorHandler() {
			printf("ErrorHandler CAN\r\n");
	}

protected:
	static CanController *thisptr;

public:

	friend  void __attribute__ ((interrupt("IRQ"))) USB_LP_CAN1_RX0_IRQHandler(void);
	friend void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle);

	CanController () {
		thisptr=this;

		GPIO_InitTypeDef   GPIO_InitStruct;

		__HAL_RCC_CAN1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/* Enable AFIO clock and Remap CAN PINs to PB8 and PB9*******/
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_CAN1_2();


		/*##-2- Configure peripheral GPIO ##########################################*/
		/* CAN1 TX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pull = GPIO_PULLUP;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* CAN1 RX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pull = GPIO_PULLUP;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



		CAN_FilterTypeDef  sFilterConfig;

		/* Configure the CAN peripheral */
		CanHandle.Instance = CAN1;

		CanHandle.Init.TimeTriggeredMode = DISABLE;
		CanHandle.Init.AutoBusOff = DISABLE;
		CanHandle.Init.AutoWakeUp = DISABLE;
		CanHandle.Init.AutoRetransmission = ENABLE;
		CanHandle.Init.ReceiveFifoLocked = DISABLE;
		CanHandle.Init.TransmitFifoPriority = DISABLE;
		CanHandle.Init.Mode = CAN_MODE_NORMAL;

		/*
		CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
		CanHandle.Init.TimeSeg1 = CAN_BS1_13TQ;
		CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
		CanHandle.Init.Prescaler = 18;
		*/

		CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
		CanHandle.Init.TimeSeg1 = CAN_BS1_13TQ;
		CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
		CanHandle.Init.Prescaler = 4;

		if (HAL_CAN_Init(&CanHandle) != HAL_OK)
		{
			/* Initialization Error */
			CanErrorHandler();
		}

		/* Configure the CAN Filter */
		sFilterConfig.FilterBank = 0;
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterIdHigh = 0x0000;
		sFilterConfig.FilterIdLow = 0x0000;
		sFilterConfig.FilterMaskIdHigh = 0x0000;
		sFilterConfig.FilterMaskIdLow = 0x0000;
		sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.SlaveStartFilterBank = 14;

		if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)	{
			CanErrorHandler();
		}

		if (HAL_CAN_Start(&CanHandle) != HAL_OK) {
			CanErrorHandler();
		}

		if (HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
			CanErrorHandler();
		}
	}



	void canSendExt(const uint32_t id, uint8_t len, uint8_t *data) {
		if (len>8) {
			len=8;
		}

		CAN_TxHeaderTypeDef   txHeader;
		txHeader.StdId = 0;
		txHeader.ExtId = id;
		txHeader.RTR = CAN_RTR_DATA;
		txHeader.IDE = CAN_ID_EXT;
		txHeader.DLC = len;
		txHeader.TransmitGlobalTime = DISABLE;

		uint32_t              txMailbox;
		if (HAL_CAN_AddTxMessage(&CanHandle, &txHeader, data, &txMailbox) != HAL_OK) {
			CanErrorHandler();
		}
	}
	void canSendStd(const uint32_t id, uint8_t len, uint8_t *data) {
		if (len>8) {
			len=8;
		}

		CAN_TxHeaderTypeDef   txHeader;
		txHeader.StdId = id;
		txHeader.ExtId = 0x01;
		txHeader.RTR = CAN_RTR_DATA;
		txHeader.IDE = CAN_ID_STD;
		txHeader.DLC = len;
		txHeader.TransmitGlobalTime = DISABLE;

		uint32_t              txMailbox;
		if (HAL_CAN_AddTxMessage(&CanHandle, &txHeader, data, &txMailbox) != HAL_OK) {
			CanErrorHandler();
		}
	}

	CAN_HandleTypeDef *getCanHandle() {
		return &CanHandle;
	}

	bool pollRx(uint32_t *id, bool *extId, uint8_t *len, uint8_t data[]) {
		if(HAL_CAN_GetRxMessage(&CanHandle,  CAN_RX_FIFO0, &rxHeader, data) == HAL_OK) {
			if(rxHeader.IDE == CAN_ID_EXT) {
				*extId=true;
				*id=rxHeader.ExtId;
			} else {
				*extId=false;
				*id=rxHeader.StdId;
			}
			*len=rxHeader.DLC;
			return(true);
		}
		return(false);
	}

protected:

};

#endif
