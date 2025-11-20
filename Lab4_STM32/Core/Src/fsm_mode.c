/*
 * fsm_manual.c
 *
 *  Created on: Nov 5, 2025
 *      Author: Ashborn
 */

#include "fsm_mode.h"
#include "global.h"
#include "7segment.h"

#define BLINK_TIME     25   // 250ms
#define TIMEOUT_TIME   1000 // 10s

static int blink_cnt = 0;
static int timeout_cnt = 0;

void fsm_mode_run() {
    switch (mode_status) {
        case NORMAL:
            fsm_automatic_run();
            // Nut 1: Chuyen sang Mode Do
            if (isButtonPressed(0) == 1) {
                mode_status = MAN_RED;
                temp_value_red = T_RED_Config; // Lay gia tri hien tai de chinh sua
                blink_cnt = BLINK_TIME;
                timeout_cnt = TIMEOUT_TIME;
                resetAll();
            }
            break;

        case MAN_RED:
            // 1. Blink LED
            blink_cnt--;
            if (blink_cnt <= 0) {
                HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
                blink_cnt = BLINK_TIME;
            }

            // 2. Hien thi: Mode 02 - Gia tri Do
            set7SEG_Numbers(2, temp_value_red);

            // 3. Timeout
            timeout_cnt--;
            if (timeout_cnt <= 0) {
                mode_status = NORMAL;
                traffic_status = INIT;
            }

            // 4. Nut 1: Chuyen Mode
            if (isButtonPressed(0) == 1) {
                mode_status = MAN_GREEN;
                temp_value_green = T_GREEN_Config; // Load gia tri Xanh
                timeout_cnt = TIMEOUT_TIME;
                resetAll();
            }

            // 5. Nut 2: Tang gia tri (Ho tro ca NHAN GIU - Long Press)
            // Tang toc do neu giu chat (spam logic)
            if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
                temp_value_red++;
                if (temp_value_red > 99) temp_value_red = 1;
                timeout_cnt = TIMEOUT_TIME;
            }

            // 6. Nut 3: Luu
            if (isButtonPressed(2) == 1) {
                if (temp_value_red == temp_value_green + temp_value_yellow) {
                    T_RED_Config = temp_value_red;
                    T_GREEN_Config = temp_value_green;
                    T_YELLOW_Config = temp_value_yellow;
                    mode_status = NORMAL;
                    traffic_status = INIT;
                }
            }
            break;

        case MAN_GREEN:
            blink_cnt--;
            if (blink_cnt <= 0) {
                HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
                blink_cnt = BLINK_TIME;
            }

            // Hien thi: Mode 03 - Gia tri Xanh
            set7SEG_Numbers(3, temp_value_green);

            timeout_cnt--;
            if (timeout_cnt <= 0) {
                mode_status = NORMAL;
                traffic_status = INIT;
            }

            if (isButtonPressed(0) == 1) {
                mode_status = MAN_YELLOW;
                temp_value_yellow = T_YELLOW_Config; // Load gia tri Vang
                timeout_cnt = TIMEOUT_TIME;
                resetAll();
            }

            if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
                temp_value_green++;
                if (temp_value_green > 99) temp_value_green = 1;
                timeout_cnt = TIMEOUT_TIME;
            }

            if (isButtonPressed(2) == 1) {
                if (temp_value_red == temp_value_green + temp_value_yellow) {
                    T_RED_Config = temp_value_red;
                    T_GREEN_Config = temp_value_green;
                    T_YELLOW_Config = temp_value_yellow;
                    mode_status = NORMAL;
                    traffic_status = INIT;
                }
            }
            break;

        case MAN_YELLOW:
            blink_cnt--;
            if (blink_cnt <= 0) {
                HAL_GPIO_TogglePin(YELLOW1_GPIO_Port, YELLOW1_Pin);
                blink_cnt = BLINK_TIME;
            }

            // Hien thi: Mode 04 - Gia tri Vang
            set7SEG_Numbers(4, temp_value_yellow);

            timeout_cnt--;
            if (timeout_cnt <= 0) {
                mode_status = NORMAL;
                traffic_status = INIT;
            }

            if (isButtonPressed(0) == 1) {
                mode_status = NORMAL;
                traffic_status = INIT;
            }

            if (isButtonPressed(1) == 1 || isButtonLongPressed(1) == 1) {
                temp_value_yellow++;
                if (temp_value_yellow > 99) temp_value_yellow = 1;
                timeout_cnt = TIMEOUT_TIME;
            }

            if (isButtonPressed(2) == 1) {
                if (temp_value_red == temp_value_green + temp_value_yellow) {
                    T_RED_Config = temp_value_red;
                    T_GREEN_Config = temp_value_green;
                    T_YELLOW_Config = temp_value_yellow;
                    mode_status = NORMAL;
                    traffic_status = INIT;
                }
            }
            break;

        default:
            mode_status = NORMAL;
            break;
    }
}
