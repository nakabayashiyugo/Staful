#include <string>
#include "Engine/GameObject.h"

const int FPS = 60;

class Timer
{
	float count_time_;
	float current_time_;
	float limit_time_;

	bool isTimeUpped_;
public:
	Timer(float _limitTime);

	//‰Šú‰»
	void Initialize();

	//XV
	void Update();

	bool isTimeUpped() { return isTimeUpped_; }

	int GetCurTime() { return current_time_; }
	int GetLimitTime() { return limit_time_; }

	void TimeReset();
};