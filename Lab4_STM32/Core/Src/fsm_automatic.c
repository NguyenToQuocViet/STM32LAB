/*
 * fsm_automatic.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Ashborn
 */

#include "fsm_automatic.h"
#include "7segment.h"

// Gia su Scheduler chay 10ms/tick -> 1s = 100 ticks
#define TICK_PER_SEC  100

static int cnt_road1 = 0;
static int cnt_road2 = 0;

void fsm_automatic_run() {
    // 1. cap nhat thoi gian hien thi (cho led 7 doan)
    if (cnt_road1 > 0) timeLeft_Road1 = (cnt_road1 + 99) / TICK_PER_SEC;
    else timeLeft_Road1 = 0;

    if (cnt_road2 > 0) timeLeft_Road2 = (cnt_road2 + 99) / TICK_PER_SEC;
    else timeLeft_Road2 = 0;

    update_7SEG_AutoMode();

    // 2. may trang thai
    switch (traffic_status) {
    case INIT:
        resetAll();

        // trang thai 1: Xanh 1 - Do 2
        setGreen_1();
        setRed_2();

        // nap thoi gian (doi giay sang ticks)
        cnt_road1 = T_GREEN_Config * TICK_PER_SEC;
        cnt_road2 = T_RED_Config * TICK_PER_SEC;

        traffic_status = R1_GREEN_R2_RED;
        break;

    case R1_GREEN_R2_RED:
        // tru thoi gian
        if (cnt_road1 > 0) cnt_road1--;
        if (cnt_road2 > 0) cnt_road2--;

        // chuyen trang thai: het Xanh 1 -> Vang 1
        if (cnt_road1 <= 0) {
            setYellow_1();
            cnt_road1 = T_YELLOW_Config * TICK_PER_SEC;
            traffic_status = R1_YELLOW_R2_RED;
        }
        break;

    case R1_YELLOW_R2_RED:
        if (cnt_road1 > 0) cnt_road1--;
        if (cnt_road2 > 0) cnt_road2--;

        // chuyen trang thai: het Vang 1 -> Do 1, Xanh 2
        if (cnt_road1 <= 0) {
            setRed_1();
            setGreen_2();

            cnt_road1 = T_RED_Config * TICK_PER_SEC;
            cnt_road2 = T_GREEN_Config * TICK_PER_SEC;

            traffic_status = R1_RED_R2_GREEN;
        }
        break;

    case R1_RED_R2_GREEN:
        if (cnt_road1 > 0) cnt_road1--;
        if (cnt_road2 > 0) cnt_road2--;

        // chuyen trang thai: het Xanh 2 -> Vang 2
        if (cnt_road2 <= 0) {
            setYellow_2();
            cnt_road2 = T_YELLOW_Config * TICK_PER_SEC;
            traffic_status = R1_RED_R2_YELLOW;
        }
        break;

    case R1_RED_R2_YELLOW:
        if (cnt_road1 > 0) cnt_road1--;
        if (cnt_road2 > 0) cnt_road2--;

        // chuyen trang thai: het Vang 2 -> Lap lai tu dau
        if (cnt_road2 <= 0) {
            traffic_status = INIT;
        }
        break;

    default:
        traffic_status = INIT;
        break;
    }
}
