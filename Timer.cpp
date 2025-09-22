#include "Timer.h"

Timer::Timer(GameObject* parent)
	:GameObject(parent, "Timer"), currentTime_(0.0f)
{
}

bool Timer::TimeElapsed(const float& time)
{
	if (currentTime_ == 0)
	{
		currentTime_ = time;
	}

	currentTime_ -= 1.0f / 60.0f;

	if (currentTime_ <= 0)
	{
		currentTime_ = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool Timer::NoResetTimeElapsed()
{
	if (currentTime_ <= 0)
	{
		return true;
	}
	else
	{
		currentTime_ -= 1.0f / 60.0f;
		return false;
	}
}
