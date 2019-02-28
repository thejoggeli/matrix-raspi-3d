#pragma once

#include <ctime>
#include <stdint.h>

namespace Ledlib {

class Time {
public:
	static uint32_t startTimeOffsetInt;
	static float startTimeOffsetFloat;
	static float deltaTime;
	static float deltaTimeUnscaled;
	static float currentTime;
	static float lastTime;
	static float startTime;
	static float sinceStart;
	static float sinceStartUnscaled;
	static float timeScale;
	static void Start();
	static void Update();
	static float GetRealTimeSecondsFloat();
	static uint32_t GetRealTimeSecondsInt();
	static uint64_t GetRealTimeMillisecondsInt();
	static float GetOffsetTimeSecondsFloat();
};


}
