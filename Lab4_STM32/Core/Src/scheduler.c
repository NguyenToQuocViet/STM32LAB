/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Ashborn
 */

#include "scheduler.h"

sTasks SCH_Tasks [SCH_MAX_TASKS];
int Error_Code = 0;
sTasks *Head_Task = NULL;

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
	SCH_Tasks[id].next   	= NULL;

	__disable_irq();

	//TH Add vao dau list: 1. Head rong 2. Task moi duoc them vao thoi gian thuc hien ngan hon Head
	if ((Head_Task == NULL) || (delay < Head_Task->delay)) {
		SCH_Tasks[id].next = Head_Task;

		//cap nhat lai thoi gian task sau
		if (Head_Task != NULL) {
			Head_Task->delay -= delay;
		}

		//cap nhat task moi gio la head task
		Head_Task = &SCH_Tasks[id];
	} else {
		//TH Add vao giua list: duyet qua toan bo list, so sanh delay va doi ve sau
		sTasks *cur_ptr 	= Head_Task;
		sTasks *prev_ptr 	= NULL;

		while ((cur_ptr != NULL) && (delay >= cur_ptr->delay)) {
			//tru delay cua minh di
			delay -= cur_ptr->delay;

			//di chuyen
			prev_ptr = cur_ptr;
			cur_ptr = cur_ptr->next;
		}

		//cap nhat task sau khi tim duoc vi tri thich hop
		SCH_Tasks[id].next = cur_ptr;
		prev_ptr->next = &SCH_Tasks[id];
		SCH_Tasks[id].delay = delay;

		//cap nhat lai thoi gian task sau
		if (cur_ptr != NULL) {
			cur_ptr->delay -= delay;
		}
	}

	__enable_irq();
}

void SCH_Update() {
/*	uint8_t id;

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
	}*/

	if (Head_Task != NULL) {
		if (Head_Task->delay > 0) {
			Head_Task->delay--;
		}

		sTasks * temp = Head_Task;
		while ((temp != NULL) && (temp->delay == 0)) {
			temp->runMe += 1;

			if (temp->period > 0) {
			}

			temp = temp->next;
		}
	}
}

void SCH_Dispatch_Tasks() {
	/*uint8_t id;

	for (id = 0; id < SCH_MAX_TASKS; id++) {
		if (SCH_Tasks[id].runMe > 0) {	//toi gio chay
			(*SCH_Tasks[id].pTask)();	//thuc thi ham do
			SCH_Tasks[id].runMe -= 1;	//reset flag

			if (SCH_Tasks[id].period == 0) {
				SCH_Delete_Task(id);	//chay 1 lan xong xoa luon (one-shot)
			}
		}
	}*/

	//chi duyet task dau vi list da duoc sort
	while ((Head_Task != NULL) && (Head_Task->runMe > 0)) {
		//thuc thi task
		(*Head_Task->pTask)();
		Head_Task->runMe -= 1;

		//xoa task
		sTasks *task_just_ran = Head_Task;	//luu lai truoc khi xoa
		Head_Task = Head_Task->next;		//nhuong cho thang tiep theo

		//luu tam thong tin
		void (*pTask_temp)() = task_just_ran->pTask;
		uint32_t period_temp = task_just_ran->period;

		task_just_ran->pTask = NULL;	//xoa (mem leak)
		task_just_ran->next = NULL;

		if (task_just_ran->period > 0) {	//neu co period -> add lai
			SCH_Add_Task(pTask_temp, period_temp, period_temp);
		}
	}

	//het viec -> di ngu
	SCH_Go_To_Sleep();
}

void SCH_Delete_Task(uint8_t taskID) {
	if (Head_Task == NULL || taskID >= SCH_MAX_TASKS || SCH_Tasks[taskID].pTask == 0) {
	    return;
	}

	sTasks *cur_ptr = Head_Task;
	sTasks *prev_ptr = NULL;

	while (cur_ptr != NULL && cur_ptr->taskID != taskID) {
		prev_ptr = cur_ptr;
		cur_ptr = cur_ptr->next;
	}

	if (cur_ptr == NULL) {
	    return;
	}

	//xoa o dau hang
	if (prev_ptr == NULL) {
		Head_Task = cur_ptr->next;

		if (Head_Task != NULL) {
			Head_Task->delay += cur_ptr->delay;
		}
	} else {	//xoa o giua
		prev_ptr->next = cur_ptr->next;

		//cong them thoi gian vao
		if (cur_ptr->next != NULL) {
			cur_ptr->next->delay += cur_ptr->delay;
		}
	}

	//reset
	SCH_Tasks[taskID].pTask 	= 0;
	SCH_Tasks[taskID].delay 	= 0;
	SCH_Tasks[taskID].period 	= 0;
	SCH_Tasks[taskID].runMe 	= 0;
	SCH_Tasks[taskID].next 		= NULL;
}

void SCH_Go_To_Sleep() {
	__WFI();
}
