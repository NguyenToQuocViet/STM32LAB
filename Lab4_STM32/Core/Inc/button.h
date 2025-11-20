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

#define NO_OF_BUTTONS 			3
#define DURATION_FOR_LONG_PRESS 200

void button_init();
void button_reading(void);
int isButtonPressed(int index);
int isButtonLongPressed(int index);

#endif /* INC_BUTTON_H_ */
