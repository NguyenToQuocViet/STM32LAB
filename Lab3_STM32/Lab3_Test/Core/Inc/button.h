/*
 * button.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE 	GPIO_PIN_SET
#define PRESSED_STATE 	GPIO_PIN_RESET

extern int button_flag1;

void getKeyInput();
void subKeyProcess();

#endif /* INC_BUTTON_H_ */
