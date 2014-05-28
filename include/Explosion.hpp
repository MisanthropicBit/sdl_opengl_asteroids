#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Vector2.hpp"
#include "Color.hpp"
#include <vector>

class Particle {
	public:
		Particle(Vector2 position, Vector2 speed, const Color& color);

		void Update(int dt);
		void Draw();

	public:
		Color color;
		float alpha;
		Vector2 position;
		Vector2 speed;
		bool dead;
};

class Explosion {
	public:
		Explosion(int size, Vector2 position, float force);
		~Explosion();

		void Update(int dt);
		void Draw();

		bool dead;

	private:
		std::vector<Particle*> particles;
};

#endif
