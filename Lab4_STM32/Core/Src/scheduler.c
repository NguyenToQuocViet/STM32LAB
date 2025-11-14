/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Ashborn
 */

#include "scheduler.h"

sTasks SCH_Tasks [SCH_MAX_TASKS];
int Error_Code;

void SCH_Init() {
	uint8_t i;

	//xoa sach toan bo mang
	for (i = 0; i < SCH_MAX_TASKS; i++) {
			SCH_Delete_Task(i);
	}
}

void SCH_Add_Task(void(*pFunction)(), uint32_t delay, uint32_t period) {
	uint8_t id = 0;

	//tim vi tri trong trong mang
	while (SCH_Tasks[id].pTask != 0 && id < SCH_MAX_TASKS) {
		id++;
	}

	//neu mang da day
	if (id == SCH_MAX_TASKS) {
		Error_Code = ERROR_SCH_TOO_MANY_TASKS;

		return;
	}

	//neu tim duoc vi tri, add task vao vi tri do
	SCH_Tasks[id].pTask 	= pFunction;
	SCH_Tasks[id].delay 	= delay;
	SCH_Tasks[id].period 	= period;
	SCH_Tasks[id].runMe		= 0;	//mac dinh
	SCH_Tasks[id].taskID	= id;
}

void SCH_Update() {
	uint8_t id;

	for (id = 0; id < SCH_MAX_TASKS; id++) {
		if (SCH_Tasks[id].pTask) {
			if (SCH_Tasks[id].delay <= 0) {	//den gio
				SCH_Tasks[id].runMe += 1;	//bat flag xu ly

				if (SCH_Tasks[id].period) {	//neu co period (lap lai)
					SCH_Tasks[id].delay = SCH_Tasks[id].period;	//nap lai delay
				}	//neu khong, khong nap lai, chi chay 1 lan
			} else {
				SCH_Tasks[id].delay --;		//chua den gio, giam dan
			}
		}
	}
}

void SCH_Dispatch_Tasks() {
	uint8_t id;

	for (id = 0; id < SCH_MAX_TASKS; id++) {
		if (SCH_Tasks[id].runMe > 0) {	//toi gio chay
			(*SCH_Tasks[id].pTask)();	//thuc thi ham do
			SCH_Tasks[id].runMe -= 1;	//reset flag

			if (SCH_Tasks[id].period == 0) {
				SCH_Delete_Task(id);	//chay 1 lan xong xoa luon (one-shot)
			}
		}
	}

	//het viec -> di ngu
	SCH_Go_To_Sleep();
}

void SCH_Delete_Task(uint8_t taskID) {
	if (taskID >= SCH_MAX_TASKS) {
		return;
	}

	//reset
	SCH_Tasks[taskID].pTask 	= 0;
	SCH_Tasks[taskID].delay 	= 0;
	SCH_Tasks[taskID].period 	= 0;
	SCH_Tasks[taskID].runMe 	= 0;
}

void SCH_Go_To_Sleep() {

}
