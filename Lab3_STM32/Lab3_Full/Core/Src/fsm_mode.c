/*
 * fsm_manual.c
 *
 *  Created on: Nov 5, 2025
 *      Author: Ashborn
 */

#include <fsm_mode.h>

int temp_value_red = 1;
int temp_value_green = 1;
int temp_value_yellow = 1;

void fsm_mode_run() {
    switch (mode_status) {
        case NORMAL:
        	//1. chay auto
            fsm_automatic_run();

            //2. hien thi thoi gian 2 truc duong
            update_7SEG_AutoMode();

            //3. chuyen mode (button1)
            if (isButtonPressed(0) == 1) {
                mode_status = MAN_RED;

                //init
                temp_value_red = T_RED_Config;
                temp_value_green = T_GREEN_Config;
                temp_value_yellow = T_YELLOW_Config;

                setTimer(2, 10000); 		//set time out
                setTimer(1, 250);			//set blink led

                resetAll();
            }

            break;
        case MAN_RED:
        	//1. blink led
            if (timer_flag[1] == 1) {
                HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);

                setTimer(1, 250);
            }

            //2. hien thi mode va temp value
            set7SEG_Numbers(2, temp_value_red);

            //3. chuyen mode (button1)
            if (isButtonPressed(0) == 1) {
                mode_status = MAN_GREEN;

                //init
                temp_value_green = T_GREEN_Config;
                setTimer(2, 10000);			//set time out
                setTimer(1, 250); 			//set blink led

                resetAll();
            }

            //4. tang gia tri temp (button2)
            if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
                temp_value_red++;

                if (temp_value_red >= 100)
                	temp_value_red = 1;

                setTimer(2, 10000); // Reset timeout
            }

            //5. set gia tri temp (button3)
            if (isButtonPressed(2) == 1) {
            	//hop le
            	if (temp_value_red == temp_value_green + temp_value_yellow) {
            		T_RED_Config = temp_value_red;
            		T_GREEN_Config = temp_value_green;
            		T_YELLOW_Config = temp_value_yellow;

            		mode_status = NORMAL; //quay ve auto mode
            	} else {
            	//khong hop le
            		mode_status = MODE_ERROR;
            		setTimer(5, 3000);
            	}
            }

            //6. time out
            if (timer_flag[2] == 1) {
                mode_status = NORMAL; //quay ve auto mode
                traffic_status = INIT;
            }

            break;
        case MAN_GREEN:
        	//1. blink led
        	if (timer_flag[1] == 1) {
        		HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);

        		setTimer(1, 250);
        	}

        	//2. hien thi mode va temp value
        	set7SEG_Numbers(3, temp_value_green);

        	//3. chuyen mode (button1)
        	if (isButtonPressed(0) == 1) {
        		mode_status = MAN_YELLOW;

        		//init
        		temp_value_yellow = T_YELLOW_Config;
        		setTimer(2, 10000);
        		setTimer(1, 250);

        		resetAll();
        	}

        	//4. tang gia tri temp (button2)
        	if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
        		temp_value_green++;

        		if (temp_value_green >= 100)
        			temp_value_green = 1;

        		setTimer(2, 10000); // Reset timeout
        	}

        	//5. set gia tri temp (button3)
        	if (isButtonPressed(2) == 1) {
        		//hop le
        		if (temp_value_red == temp_value_green + temp_value_yellow) {
        			T_RED_Config = temp_value_red;
        			T_GREEN_Config = temp_value_green;
        			T_YELLOW_Config = temp_value_yellow;

        			mode_status = NORMAL; //quay ve auto mode
        			traffic_status = INIT;
        		} else {
        			//khong hop le
        			mode_status = MODE_ERROR;
        			setTimer(5, 3000);
        		}
        	}

        	//6. time out
        	if (timer_flag[2] == 1) {
        		mode_status = NORMAL; //quay ve auto mode
        		traffic_status = INIT;
        	}

            break;

        case MAN_YELLOW:
        	//1. blink led
        	if (timer_flag[1] == 1) {
        		HAL_GPIO_TogglePin(YELLOW1_GPIO_Port, YELLOW1_Pin);

        		setTimer(1, 250);
        	}

        	//2. hien thi mode va temp value
        	set7SEG_Numbers(4, temp_value_yellow);

        	//3. chuyen mode (button1)
        	if (isButtonPressed(0) == 1) {
        		mode_status = NORMAL;
        		traffic_status = INIT;
        	}

        	//4. tang gia tri temp (button2)
        	if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
        		temp_value_yellow++;

        		if (temp_value_yellow >= 100)
        			temp_value_yellow = 1;

        		setTimer(2, 10000); // Reset timeout
        	}

        	//5. set gia tri temp (button3)
        	if (isButtonPressed(2) == 1) {
        		//hop le
        		if (temp_value_red == temp_value_green + temp_value_yellow) {
        			T_RED_Config = temp_value_red;
        			T_GREEN_Config = temp_value_green;
        			T_YELLOW_Config = temp_value_yellow;

        			mode_status = NORMAL; //quay ve auto mode
        			traffic_status = INIT;
        		} else {
        			//khong hop le
        			mode_status = MODE_ERROR;
        			setTimer(5, 3000);
        		}

        	}

        	//6. time out
        	if (timer_flag[2] == 1) {
        	   mode_status = NORMAL; //quay ve auto mode
        	   traffic_status = INIT;
        	}

            break;
        case MODE_ERROR:
        	//1. hien thi ma loi
            set7SEG_Numbers(99, 99);

            if (timer_flag[5] == 1) {
            	//reset
                mode_status = NORMAL;
                traffic_status = INIT;
            }

            break;
        default:
            mode_status = NORMAL;
            traffic_status = INIT;
            break;
    }
}
