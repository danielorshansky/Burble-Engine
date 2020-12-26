#pragma once

#include "../graphics/Window.h"

class Clock {
public:
	void Update() {
		last_time = Window::GetTime();
	}

	double GetDelta() {
		return Window::GetTime() - last_time;
	}

private:
	double last_time;
};
