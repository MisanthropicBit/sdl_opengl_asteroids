#include "RandomTimer.hpp"
#include "Math.hpp"

RandomTimer::RandomTimer() {
	min   = 0;
	max   = 0;
	value = 0;
}

RandomTimer::RandomTimer(int min, int max) {
	SetRange(min, max);
	value = Math::Random(min, max);
}

RandomTimer::~RandomTimer() {
}

void RandomTimer::SetRange(int min, int max) {
	this->min = min;
	this->max = max;
	value     = Math::Random(min, max);
}

void RandomTimer::Reset() {
	value = Math::Random(min, max);
}

bool RandomTimer::HasExpired(int dt) {
	value -= dt;

	if (value <= 0) {
		Reset();
		return true;
	}

	return false;
}
