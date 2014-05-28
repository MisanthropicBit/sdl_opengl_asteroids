#include "Timer.hpp"

//=========================================================================================================================

Timer::Timer() {
	started    = false;
	paused     = false;
	startTick  = 0;
	pausedTick = 0;
	lastTick   = 0;
	fps        = 0;
	frames     = 0;
}

//============================================================================================================================

void Timer::Start() {
	started   = true;
	paused    = false;
	startTick = SDL_GetTicks();
}

//============================================================================================================================

void Timer::Pause() {
	if (!paused) {
		started    = false;
		paused     = true;
		pausedTick = Timer::GetElapsedTime() - startTick;
	}
}

//============================================================================================================================

bool Timer::IsPaused() const {
	return paused;
}

//============================================================================================================================

// Gets the current time elaspsed since last pause
int Timer::GetTime() const {
	if (started)
		return Timer::GetElapsedTime() - startTick;

    return pausedTick;
}

//============================================================================================================================

// By calling this function in the main loop, we can calculate the fps when one second has passed.
void Timer::Update() {
	if (SDL_GetTicks() >= (lastTick + 1000)) {
		lastTick = SDL_GetTicks();
		fps      = frames;
		frames   = 0;
	}

	++frames;
}

//============================================================================================================================

// This function is static since it makes more sense. Elapsed time is not an individual property of each timer.
int Timer::GetElapsedTime() {
	return SDL_GetTicks();
}

//============================================================================================================================

int Timer::GetElapsedMinutes() {
	return (GetElapsedTime() / 1000) / 60;
}

//============================================================================================================================

int Timer::GetElapsedSeconds() {
	return (GetElapsedTime() / 1000) % 60;
}

//============================================================================================================================

// Gets the time elapsed since this function was called
int Timer::GetDeltaTime() {
	int deltatime = SDL_GetTicks() - lastTick; // milliseconds
	lastTick      = SDL_GetTicks();

	return deltatime;
}

//============================================================================================================================

float Timer::GetDeltaTimePrecise() {
    return (static_cast<float>(GetDeltaTime()) / 1000.f);
}

//============================================================================================================================

int Timer::GetFPS() const {
	return fps;
}

//=========================================================================================================================
