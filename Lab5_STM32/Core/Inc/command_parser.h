/*
 * command_parser.h
 *
 *  Created on: Dec 4, 2025
 *      Author: Ashborn
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include "main.h"

#define MAX_CMD_SIZE	30

extern uint8_t command_flag;
extern uint8_t command_data[];
extern uint8_t command_idx;
extern uint8_t temp;

void command_parser_fsm();

#endif /* INC_COMMAND_PARSER_H_ */
