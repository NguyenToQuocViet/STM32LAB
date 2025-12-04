/*
 * scheduler.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Ashborn
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"
#include "stddef.h"
#include "main.h"

#define SCH_MAX_TASKS   10
#define NO_TASK_ID      0xFF

//task struct
typedef struct sTasks {
	void (*pTask)(void);
	uint32_t delay;
	uint32_t period;
	uint8_t taskID;
	struct sTasks *next;
} sTasks;

//core function
void SCH_Init();
void SCH_Update();
void SCH_Dispatch_Tasks();

//task management
uint8_t SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period);
void SCH_Delete_Task(uint8_t taskID);

//sub function
void SCH_Go_To_Sleep();

#endif /* INC_SCHEDULER_H_ */
