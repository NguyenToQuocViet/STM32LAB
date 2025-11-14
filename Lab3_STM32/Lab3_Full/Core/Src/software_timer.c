/*
 * software_timer.c
 *
 *  Created on: Oct 5, 2025
 *      Author: Ashborn
 */

#include "software_timer.h"

static int timer_counter[MAX_SOFTWARE_TIMERS];
int timer_flag[MAX_SOFTWARE_TIMERS];

void setTimer(int index, int duration_ms) {
    if (index >= MAX_SOFTWARE_TIMERS)
    	return;

    timer_counter[index] = duration_ms / TIMER_CYCLE_MS;
    timer_flag[index] = 0;	//reset flag
}

void timerRun() {
    for (int i = 0; i < MAX_SOFTWARE_TIMERS; i++) {
    	timer_counter[i]--;

    	if (timer_counter[i] <= 0)
    		timer_flag[i] = 1;
    }
}

int getTimerValue_sec(int index) {
    if (index >= MAX_SOFTWARE_TIMERS)
    	return 0;

    if (timer_counter[index] <= 0)
    	return 0;

    int remaining_ms = timer_counter[index] * TIMER_CYCLE_MS;

    int remaining_sec = (remaining_ms + 999) / 1000;

    return remaining_sec;
}
