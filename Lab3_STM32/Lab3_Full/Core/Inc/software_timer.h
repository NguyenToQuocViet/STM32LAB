/*
 * software_timer.h
 *
 *  Created on: Oct 5, 2025
 *      Author: Ashborn
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define MAX_SOFTWARE_TIMERS 6
#define TIMER_CYCLE_MS 		10

extern int timer_flag[MAX_SOFTWARE_TIMERS];

void setTimer(int index, int duration_ms);
void timerRun();

int getTimerValue_sec(int index);

#endif /* INC_SOFTWARE_TIMER_H_ */
