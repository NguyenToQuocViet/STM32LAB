/*
 * scheduler.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Ashborn
 */

#include "scheduler.h"

static sTasks SCH_Tasks[SCH_MAX_TASKS];
static sTasks *Head_Ptr = NULL;

static void SCH_Reset_Task(uint8_t index) {
    SCH_Tasks[index].pTask = NULL;
    SCH_Tasks[index].delay = 0;
    SCH_Tasks[index].period = 0;
    SCH_Tasks[index].taskID = index;
    SCH_Tasks[index].next = NULL;
}

void SCH_Init() {
    uint8_t index;
    Head_Ptr = NULL;
    for (index = 0; index < SCH_MAX_TASKS; index++) {
        SCH_Reset_Task(index);
    }
}

//O(1)
//chi giam delay, khong loop
void SCH_Update(void) {
	if (Head_Ptr != NULL) {
		if (Head_Ptr->delay > 0) {
			Head_Ptr->delay--;
		}

		//logic = 0 dispatcher xu ly
	}
}

//O(n)
uint8_t SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
	uint8_t index = 0;

	//duyet qua mang tim cho trong
	while (SCH_Tasks[index].pTask != NULL && index < SCH_MAX_TASKS) {
		index++;
	}

	//het bo nho
	if (index == SCH_MAX_TASKS) {
		return NO_TASK_ID;
	}

	//khoi tao thong tin task
	SCH_Tasks[index].pTask = pFunction;
	SCH_Tasks[index].delay = delay;
	SCH_Tasks[index].period = period;
	SCH_Tasks[index].taskID = index;

	__disable_irq();

	//add task vao linked list
	if (Head_Ptr == NULL) {	//rong
		Head_Ptr = &SCH_Tasks[index];
		Head_Ptr->next = NULL;
	} else if (delay < Head_Ptr->delay) {	//chen vao dau
		Head_Ptr->delay -= delay;
		SCH_Tasks[index].next = Head_Ptr;
		Head_Ptr = &SCH_Tasks[index];
	} else {	//chen vao giua
		sTasks *curr = Head_Ptr;
		sTasks *prev = NULL;
		uint32_t acc_delay = delay;

		//duyet qua toan bo linked list, neu delay > cur.delay -> di qua va tru bot delay
		while (curr != NULL && acc_delay >= curr->delay) {
			acc_delay -= curr->delay;

			prev = curr;
			curr = curr->next;
		}

		//chen task vao giua
		SCH_Tasks[index].delay = acc_delay;
		SCH_Tasks[index].next = curr;
		prev->next = &SCH_Tasks[index];

		//giam delay
		if (curr != NULL) {
			curr->delay -= acc_delay;
		}
	}

	__enable_irq();
	return index;
}

void SCH_Dispatch_Tasks() {
	uint8_t task_ran_flag = 0;

	while (1) {
		__disable_irq();

		//neu khong co task can chay
		if (Head_Ptr == NULL || Head_Ptr->delay > 0) {
			__enable_irq();
			break;
		}

		//neu co task can chay
		sTasks *task_to_run = Head_Ptr;	//luu lai task can chay
		Head_Ptr = Head_Ptr->next;		//doi head sang task tiep theo
		task_to_run->next = NULL;

		__enable_irq();

		//luu thong tin task can chay
		void (*pFunction_temp)(void) = task_to_run->pTask;
		uint32_t period_temp = task_to_run->period;
		uint8_t id_temp = task_to_run->taskID;

		//add lai task truoc khi chay
		if (task_to_run->period > 0) {
			SCH_Reset_Task(id_temp);
			SCH_Add_Task(pFunction_temp, period_temp, period_temp);
		} else {	//neu la one-shot, xoa task
			SCH_Reset_Task(id_temp);
		}

		//thuc thi task
		if (pFunction_temp != NULL) {
			pFunction_temp();
			task_ran_flag = 1;
		}
	}

	//di ngu neu khong co viec gi lam
	if (task_ran_flag == 0) {
		SCH_Go_To_Sleep();
	}
}

void SCH_Delete_Task(uint8_t taskID) {
    if (taskID >= SCH_MAX_TASKS || SCH_Tasks[taskID].pTask == NULL) {
        return;
    }

    __disable_irq();

    sTasks *curr = Head_Ptr;
    sTasks *prev = NULL;

    while (curr != NULL && curr->taskID != taskID) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        if (prev == NULL) {
            Head_Ptr = curr->next;

            if (Head_Ptr != NULL) {
                Head_Ptr->delay += curr->delay;
            }
        } else {
            prev->next = curr->next;

            if (curr->next != NULL) {
                curr->next->delay += curr->delay;
            }
        }

        SCH_Reset_Task(taskID);
    }

    __enable_irq();
}

void SCH_Go_To_Sleep() {
	__WFI();
}
