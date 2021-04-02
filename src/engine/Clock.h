#pragma once

#include "../graphics/Window.h"

class Clock {
public:
	void Update() { // update clock
		last_time = Window::GetTime();
	}

	double GetDelta() { // returns delta time
		return Window::GetTime() - last_time;
	}

private:
	double last_time;
};
