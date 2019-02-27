#include "Time.h"
#include <time.h>
#include <iostream>

namespace Ledlib {

uint32_t Time::startTimeOffset = 0;
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
	startTimeOffset = GetRealTimeSecondsInt();
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
	return static_cast<float>(spec.tv_sec-startTimeOffset) + static_cast<float>(spec.tv_nsec) / 1.0e9f;
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

