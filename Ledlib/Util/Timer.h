#pragma once

namespace Ledlib {

class Timer {
public:
	float startTime = 0;
	float endTime = 0;
	float duration = 0;
	Timer();
	void Start(float duration);
	void Restart(bool seamless = false);
	void Reset();
	bool IsFinished();
	bool IsRunning();
	float GetAbsoluteRemainingTime();
	float GetRelativeRemainingTime();
	float GetAbsolutePassedTime();
	float GetRelativePassedTime();
};

}
