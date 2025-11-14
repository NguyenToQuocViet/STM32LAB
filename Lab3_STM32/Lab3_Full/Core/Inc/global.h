/*
 * global.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "software_timer.h"
#include "button.h"
#include "main.h"

#define INIT				1
#define R1_GREEN_R2_RED		2
#define R1_YELLOW_R2_RED	3
#define R1_RED_R2_GREEN		4
#define R1_RED_R2_YELLOW 	5

#define NORMAL		10

#define MAN_RED		12
#define MAN_YELLOW	13
#define MAN_GREEN	14
#define MODE_ERROR  15

extern int mode_status;
extern int traffic_status;

extern int T_RED_Config;
extern int T_YELLOW_Config;
extern int T_GREEN_Config;

extern int temp_value_red;
extern int temp_value_green;
extern int temp_value_yellow;

void setRed_1();
void setGreen_1();
void setYellow_1();
void setRed_2();
void setGreen_2();
void setYellow_2();
void resetAll();

//Timer 0 -> Automatic Mode 1
//Timer 1 -> Blink
//Timer 2 -> Time out
//Timer 3 -> Automatic Mode 2

//Timer 4 -> PA5 test
//Timer 5 -> ERROR

#endif /* INC_GLOBAL_H_ */
