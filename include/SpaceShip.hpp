#ifndef SPACE_SHIP_HPP
#define SPACE_SHIP_HPP

#include "Color.hpp"
#include "Asteroid.hpp"
#include "Shot.hpp"
#include <vector>

// All constants are in miliseconds
const float FIRE_DELAY         = 100.f;
const float IMMORTALITY_PERIOD = 5000.f;
const float COLOR_CHANGE_RATE  = 0.003f;

class SpaceShip {
	public:
		SpaceShip();
		SpaceShip(float x, float y, float angle);
		~SpaceShip();

		void Update(int dt);
		void Draw();

		void  Rotate(float angle);
		void  FireShot();
		float GetAngle() const;
		void  SetColor(Color& color);
		bool  CollidesWith(Asteroid* asteroid);
		bool  ShotsCollideWith(Asteroid* asteroid);
		void  GotHit();
		bool  IsHit() const;

		Vector2 position;
		Vector2 speed;
		Vector2 acceleration;
		int lives;

	private:
		float angle;
		Color color;
		std::vector<Shot*> Shots;
		bool isHit;
		float immortalityTimer;
		float colorChangeAmount;
		Color targetColor1;
		Color targetColor2;
		float fireTimer;
};

#endif
