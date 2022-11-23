#pragma once

#include "Hauk.h"

#ifndef __HAUK_MICROSOFT_WIN32_TIME_CHIGHRESOLUTIONTIMER_H_

#define __HAUK_MICROSOFT_WIN32_TIME_CHIGHRESOLUTIONTIMER_H_

#include <Windows.h>

typedef struct tagTIMER_INFO
{
	LARGE_INTEGER StartTime;
	LARGE_INTEGER TimeSlice;
	LARGE_INTEGER ElapsedTime;
	LARGE_INTEGER TimerFrequency;

} TIMER_INFO, *PTIMER_INFO;

class cHighResolutionTimer
{
public:
	cHighResolutionTimer();
	BOOL StartTimer();
	BOOL UpdateTimer();
	LARGE_INTEGER CovertCounterToMicroSeconds(LARGE_INTEGER CounterValue);
	TIMER_INFO GetTimerInfo();

private:
	TIMER_INFO sPrivateTimerInfo;
	BOOL bIsTimerRunning;
		
};

#endif