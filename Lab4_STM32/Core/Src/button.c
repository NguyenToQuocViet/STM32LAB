/*
 * button.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#include "button.h"

static int keyReg0[NO_OF_BUTTONS];
static int keyReg1[NO_OF_BUTTONS];
static int keyReg2[NO_OF_BUTTONS];

static int keyReg3[NO_OF_BUTTONS];

static int timer_for_long_press_counter[NO_OF_BUTTONS];
static int button_is_pressed_flag[NO_OF_BUTTONS];
static int button_is_long_pressed_flag[NO_OF_BUTTONS];

void button_init() {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg0[i] = NORMAL_STATE;
        keyReg1[i] = NORMAL_STATE;
        keyReg2[i] = NORMAL_STATE;

        keyReg3[i] = NORMAL_STATE;
        timer_for_long_press_counter[i] = DURATION_FOR_LONG_PRESS;
        button_is_pressed_flag[i] = 0;
        button_is_long_pressed_flag[i] = 0;
    }
}

void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg0[i] = keyReg1[i];
        keyReg1[i] = keyReg2[i];

        //read button
        switch (i) {
            case 0:
                keyReg2[i] = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
                break;
            case 1:
                keyReg2[i] = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);
                break;
            case 2:
                keyReg2[i] = HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin);
                break;
            default:
                break;
        }

        //after debouncing
        if ((keyReg0[i] == keyReg1[i]) && (keyReg1[i] == keyReg2[i])) {

        	//nhan tha
            if (keyReg3[i] != keyReg2[i]) {
                keyReg3[i] = keyReg2[i];

                if (keyReg2[i] == PRESSED_STATE) {
                    button_is_pressed_flag[i] = 1;

                    //reset counter
                    timer_for_long_press_counter[i] = DURATION_FOR_LONG_PRESS;
                }
            }
            //nhan de
            else {
                if (keyReg3[i] == PRESSED_STATE) {
                    timer_for_long_press_counter[i]--;

                    if (timer_for_long_press_counter[i] <= 0) {
                        button_is_long_pressed_flag[i] = 1;

                        //reset counter
                        timer_for_long_press_counter[i] = DURATION_FOR_LONG_PRESS;
                    }
                }
            }
        }
    }
}

int isButtonPressed(int index) {
    if (index >= NO_OF_BUTTONS)
    	return 0;

    if (button_is_pressed_flag[index] == 1) {
        button_is_pressed_flag[index] = 0;

        return 1;
    }

    return 0;
}

int isButtonLongPressed(int index) {
    if (index >= NO_OF_BUTTONS)
    	return 0;

    if (button_is_long_pressed_flag[index] == 1) {
        button_is_long_pressed_flag[index] = 0;

        return 1;
    }

    return 0;
}
