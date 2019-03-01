#include "Timer.h"
#include "../Time.h"

namespace Ledlib {

Timer::Timer() {}

void Timer::Start(float duration){
	this->duration = duration;
	startTime = Time::currentTime;
	endTime = startTime + duration;
}
void Timer::Restart(bool seamless){
	if(seamless){
		startTime += duration;
		endTime += duration;
	} else {
		Start(duration);
	}
}
void Timer::Reset(){
	startTime = endTime = duration = 0;
}
bool Timer::IsFinished(){
	return Time::currentTime >= endTime;
}
bool Timer::IsRunning(){
	return Time::currentTime < endTime;
}
float Timer::GetAbsoluteRemainingTime(){
	float remaining = endTime - Time::currentTime;
	return remaining <= 0.0f ? 0.0f : remaining;
}
float Timer::GetRelativeRemainingTime(){
	float remaining = endTime - Time::currentTime;
	return remaining <= 0.0f ? 0.0f : remaining/duration;
}
float Timer::GetAbsolutePassedTime(){
	float passed = Time::currentTime - startTime;
	return passed >= duration ? duration : passed;
}
float Timer::GetRelativePassedTime(){
	float passed = Time::currentTime - startTime;
	return passed >= duration ? duration : passed/duration;
}

}
