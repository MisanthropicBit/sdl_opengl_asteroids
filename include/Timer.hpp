#ifndef TIMER_HPP
#define TIMER_HPP

#include "SDL.h"

class Timer {
	public:
		Timer();

		void Update();

		void Start();
		void Pause();

		bool IsPaused() const;

		int GetTime() const;
		int GetDeltaTime();
		float GetDeltaTimePrecise();
		int GetLastTick() const;
		int GetFPS() const;

		static int GetElapsedTime();
		static int GetElapsedMinutes();
		static int GetElapsedSeconds();

	protected:
		bool started;
		bool paused;

		int startTick;
		int pausedTick;
		int lastTick;
		int fps;
		int frames;
};

#endif
