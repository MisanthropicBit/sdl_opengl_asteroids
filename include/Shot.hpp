#ifndef SHOT_HPP
#define SHOT_HPP

#include "Vector2.hpp"

const float SHOT_LENGTH = 10.f;

class Shot {
	public:
		Shot();
		Shot(float x, float y, const Vector2& speed);
		~Shot();

		void Update(int dt);
		void Draw();

		Vector2 GetDirection();

    public:
		Vector2 position;
		Vector2 speed;
		Vector2 dir;
};

#endif
