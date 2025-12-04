/*
 * uart_fsm.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Ashborn
 */

#include "uart_fsm.h"
#include "command_parser.h"
#include "string.h"
#include "stdio.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

enum uartState {
	WAIT_RST,
	SEND_ADC,
	WAIT_OK
};

enum uartState status_uart = WAIT_RST;

uint32_t adc_value = 0;
char str_buffer[50];
uint32_t timer_timeout = 0;

void uart_communication_fsm() {
	switch (status_uart) {
		case WAIT_RST:
			if (command_flag) {
				command_flag = 0;

				if (strcmp((char*)command_data, "RST") == 0) {
					adc_value = HAL_ADC_GetValue(&hadc1);
					status_uart = SEND_ADC;
				}
			}

			break;
		case SEND_ADC:
			sprintf(str_buffer, "!ADC=%d#\r\n", (int)adc_value);
			HAL_UART_Transmit(&huart2, (uint8_t*)str_buffer, strlen(str_buffer), 100);

			timer_timeout = HAL_GetTick();
			status_uart = WAIT_OK;

			break;
		case WAIT_OK:
			if (command_flag) {
				command_flag = 0;

				if (strcmp((char*)command_data, "OK") == 0) {
					status_uart = WAIT_RST;
				}
			} else if (HAL_GetTick() - timer_timeout >= 3000) {	//time out
				status_uart = SEND_ADC;
			}

			break;
		default:
			status_uart = WAIT_RST;

			break;
	}
}
