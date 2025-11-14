/*
 * fsm_automatic.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#include "fsm_automatic.h"

#define TIMER_ROAD_1 0
#define TIMER_ROAD_2 3

void fsm_automatic_run() {
    switch (traffic_status) {
    case INIT:
        //tat het den
        resetAll();

        //phase 1: xanh 1 + do 2
        setGreen_1();
        setRed_2();

        //set 2 timer doc lap
        setTimer(TIMER_ROAD_1, T_GREEN_Config * 1000);
        setTimer(TIMER_ROAD_2, T_RED_Config * 1000);

        traffic_status = R1_GREEN_R2_RED;

        break;
    case R1_GREEN_R2_RED:
        //cho timer
        if (timer_flag[TIMER_ROAD_1] == 1) {

        	//phase 2: vang 1 + do 2
            setYellow_1();
            setTimer(TIMER_ROAD_1, T_YELLOW_Config * 1000);

            traffic_status = R1_YELLOW_R2_RED;
        }

        break;
    case R1_YELLOW_R2_RED:
        //cho timer
        if (timer_flag[TIMER_ROAD_1] == 1) {

        	//phase 3: do 1 + xanh 2
            setRed_1();
            setGreen_2();

            setTimer(TIMER_ROAD_1, T_RED_Config * 1000);
            setTimer(TIMER_ROAD_2, T_GREEN_Config * 1000);

            traffic_status = R1_RED_R2_GREEN;
        }
        break;

    case R1_RED_R2_GREEN:
        //cho timer
        if (timer_flag[TIMER_ROAD_2] == 1) {

        	//phase 4: do 1 + vang 2
            setYellow_2();

            setTimer(TIMER_ROAD_2, T_YELLOW_Config * 1000);

            traffic_status = R1_RED_R2_YELLOW;
        }

        break;
    case R1_RED_R2_YELLOW:
        //cho timer
        if (timer_flag[TIMER_ROAD_2] == 1) {

        	//quay ve phase 1
            traffic_status = INIT;
        }

        break;
    default:
        traffic_status = INIT;
        break;
    }
}
