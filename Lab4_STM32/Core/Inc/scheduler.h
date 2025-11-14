/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2025
 *      Author: Ashborn
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"
#define SCH_MAX_TASKS	10
#define ERROR_SCH_TOO_MANY_TASKS	1

typedef struct {
	void (*pTask)();
	uint32_t 	delay;
	uint32_t 	period;
	uint8_t		runMe;
	uint32_t	taskID;
} sTasks;

extern int Error_Code;

void SCH_Init();
void SCH_Add_Task(void(*pFunction)(), uint32_t delay, uint32_t period);
void SCH_Update();
void SCH_Dispatch_Tasks();
void SCH_Delete_Task(uint8_t taskID);
void SCH_Go_To_Sleep();

#endif /* INC_SCHEDULER_H_ */
