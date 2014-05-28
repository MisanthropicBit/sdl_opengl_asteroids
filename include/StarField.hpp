#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include "Vector2.hpp"
#include <vector>

const int BLINK_INTERVAL = 500;
const int BLINK_PERIOD   = 5000;

class Star {
	public:
		Star(Vector2 position);

		void Update(int dt);

		Vector2 position;
		bool  blinking;
		bool  visible;
		int   blinkIntervalTimer;
		int   blinkPeriodTimer;
		float alpha;
};

class StarField {
	public:
		StarField();

		void Populate(int noStars);
		void Update(int dt);
		void Draw();

	private:
		int changeTimer;
		std::vector<Star> stars;
};

#endif
