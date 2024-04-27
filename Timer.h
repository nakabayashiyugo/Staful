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

	//初期化
	void Initialize();

	//更新
	void Update();

	bool isTimeUpped() { return isTimeUpped_; }

	int GetCurTime() { return current_time_; }
	int GetLimitTime() { return limit_time_; }

	void TimeReset();
};