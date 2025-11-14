/*
 * button.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#include "button.h"

int button_flag1 = 0;

int keyReg0 = NORMAL_STATE;
int keyReg1 = NORMAL_STATE;
int keyReg2 = NORMAL_STATE;

//xu ly nhan de (sau khi da on dinh)
int keyReg3 = NORMAL_STATE;
int timerForKeyPress = 200;

void subKeyProcess() {
	button_flag1 = 1;
}

void getKeyInput() {
	keyReg0 = keyReg1;
	keyReg1 = keyReg2;
	keyReg2 = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);

	//Sau khi nut nhan da on dinh
	if (keyReg0 == keyReg1 && keyReg1 == keyReg2) {
		//Nhan tha
		if (keyReg3 != keyReg2) {
			keyReg3 = keyReg2;

			if (keyReg2 == PRESSED_STATE) {
				//TODO
				subKeyProcess();
				timerForKeyPress = 200;
			}

		}
		//Nhan de
		else {
			if (keyReg3 == PRESSED_STATE) {
				timerForKeyPress--;

				if (timerForKeyPress == 0) {
					//TODO
					subKeyProcess();
					timerForKeyPress = 200;
				}
			}
		}
	}
}
