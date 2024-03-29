#include <string>
#include "Engine/GameObject.h"

const int FPS = 60;

class Timer
{
	int count_time_;
	int current_time_;
	int limit_time_;

	bool isTimeUpped_;
public:
	Timer(int _limitTime);

	//初期化
	void Initialize();

	//更新
	void Update();

	bool isTimeUpped() { return isTimeUpped_; }

	int GetCurTime() { return current_time_; }
	int GetLimitTime() { return limit_time_; }

	void TimeReset();
};