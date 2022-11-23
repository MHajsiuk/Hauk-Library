#include "Hauk.h"


cHighResolutionTimer::cHighResolutionTimer()
{
	QueryPerformanceFrequency(&this->sPrivateTimerInfo.TimerFrequency);
	this->bIsTimerRunning = FALSE;
}




BOOL cHighResolutionTimer::StartTimer()
{
	BOOL bResult = FALSE;

	bResult = QueryPerformanceCounter(&this->sPrivateTimerInfo.StartTime);
	this->sPrivateTimerInfo.StartTime = this->CovertCounterToMicroSeconds(this->sPrivateTimerInfo.StartTime);
	this->sPrivateTimerInfo.ElapsedTime = this->sPrivateTimerInfo.StartTime;
	this->sPrivateTimerInfo.TimeSlice.QuadPart = 0;

	bIsTimerRunning = TRUE;

	return bResult;
}




BOOL cHighResolutionTimer::UpdateTimer()
{
	if (this->bIsTimerRunning == FALSE)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	LARGE_INTEGER NewTime;
	
	bResult = QueryPerformanceCounter(&NewTime);
	NewTime = this->CovertCounterToMicroSeconds(NewTime);
	
	NewTime.QuadPart -= this->sPrivateTimerInfo.StartTime.QuadPart;
	
	this->sPrivateTimerInfo.TimeSlice.QuadPart = NewTime.QuadPart - this->sPrivateTimerInfo.ElapsedTime.QuadPart;
	this->sPrivateTimerInfo.ElapsedTime.QuadPart = NewTime.QuadPart;

	return bResult;
}



LARGE_INTEGER cHighResolutionTimer::CovertCounterToMicroSeconds(LARGE_INTEGER CounterValue)
{
	LARGE_INTEGER Output;

	Output.QuadPart = CounterValue.QuadPart * 1000000;
	Output.QuadPart = Output.QuadPart / this->sPrivateTimerInfo.TimerFrequency.QuadPart;
	
	return Output;
}




TIMER_INFO cHighResolutionTimer::GetTimerInfo()
{
	return this->sPrivateTimerInfo;
}


