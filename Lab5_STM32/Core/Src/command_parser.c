/*
 * command_parser.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Ashborn
 */

#include "command_parser.h"
#include "string.h"

uint8_t command_flag = 0;
uint8_t command_data[MAX_CMD_SIZE];
uint8_t command_idx = 0;
uint8_t temp = 0;

enum parseState {
	INIT,
	WAITING
};

enum parseState status_parser = INIT;

void command_parser_fsm() {
	switch (status_parser) {
		case INIT:
			if (temp == '!') {	//bat dau doc
				command_idx = 0;
				memset(command_data, 0, MAX_CMD_SIZE);

				status_parser = WAITING;
			}

			break;
		case WAITING:
			if (temp == '#') {	//ket thuc
				command_data[command_idx] = '\0';	//ket thuc chuoi
				command_flag = 1;	//tin hieu ket thuc

				status_parser = INIT;
			} else if (temp == '!') {	//loi reset -> lap lai
				command_idx = 0;
				memset(command_data, 0, MAX_CMD_SIZE);
			} else {	//doc goi tin
				command_data[command_idx++] = temp;

				if (command_idx >= MAX_CMD_SIZE) {
					command_idx = 0;	//reset neu max size
				}
			}

			break;
		default:
			status_parser = INIT;

			break;
	}
}
