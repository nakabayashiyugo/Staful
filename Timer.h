#include <string>
#include "Engine/GameObject.h"
#include "Windows.h"
#include "Engine/Image.h"

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
	void SetCurTime(float _curTime) 
	{ 
		current_time_ = _curTime; 
		count_time_ = current_time_ * FPS;
	}

	void TimeReset();
};