#pragma once

#include "../graphics/Window.h"

class Clock {
public:
	void Update(Window window) {
		last_time = window.GetTime();
	}

	double GetDelta(Window window) {
		return window.GetTime() - last_time;
	}

private:
	double last_time;
};
