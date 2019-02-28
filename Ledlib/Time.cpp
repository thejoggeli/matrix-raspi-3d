#include "Time.h"
#include "Log.h"
#include <time.h>
#include <iostream>
#include <math.h>

namespace Ledlib {

uint32_t Time::startTimeOffsetInt = 0;
float Time::startTimeOffsetFloat = 0;
float Time::deltaTime = 0;
float Time::deltaTimeUnscaled;
float Time::currentTime = 0;
float Time::lastTime = 0;
float Time::startTime = 0;
float Time::sinceStart = 0;
float Time::sinceStartUnscaled = 0;
float Time::timeScale = 1.0f;

void Time::Start(){
	// time offset
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	startTimeOffsetInt = static_cast<uint32_t>(spec.tv_sec);
	startTimeOffsetFloat = static_cast<float>(spec.tv_nsec/1e6)/1.0e3f;

	// start
	startTime = GetOffsetTimeSecondsFloat();
	currentTime = startTime;
	lastTime = startTime;
}

void Time::Update(){
	lastTime = currentTime;
	currentTime = GetOffsetTimeSecondsFloat();
	deltaTimeUnscaled = currentTime - lastTime;
	deltaTime = deltaTimeUnscaled * timeScale;
	sinceStartUnscaled = currentTime - startTime;
	sinceStart += deltaTime;
}

float Time::GetOffsetTimeSecondsFloat(){
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return static_cast<float>(spec.tv_sec-startTimeOffsetInt) + static_cast<float>(spec.tv_nsec) / 1.0e9f - startTimeOffsetFloat;
}

float Time::GetRealTimeSecondsFloat(){
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return static_cast<float>(spec.tv_sec) + static_cast<float>(spec.tv_nsec) / 1.0e9f;
}

uint32_t Time::GetRealTimeSecondsInt(){
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return static_cast<uint32_t>(spec.tv_sec);
}

uint64_t Time::GetRealTimeMillisecondsInt(){
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	return static_cast<uint64_t>(spec.tv_sec)*1000 + static_cast<uint64_t>(spec.tv_nsec/1.0e6f);
}

}

