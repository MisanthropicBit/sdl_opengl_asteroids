#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Polygon.hpp"
#include <list>

const float MIN_AREA     = 2000.f;
const float INITIAL_SIZE = 80.f;

class Asteroid : public Polygon {
	public:
		Asteroid();
		Asteroid(float x, float y, float angle, float size);
		~Asteroid();

		void Set(float x, float y, float angle, float size);
		void Update(int dt);
		void Draw();

		static void ToggleDebugLines();
		static void ToggleWireframes();

		void SetImmortality(bool flag);
		bool IsDying() const;
		bool IsDead() const;
		bool IsInSight();
		float GetSize() const;

		Vector2 speed;

		// Since asteroids are going to be constantly sliced, we are going to have a lot of insertions/deletions
		// occurring anywhere in the container. A std::list is faster than a std::vector for this purpose, although
		// slower to traverse
		static std::list<Asteroid*> Asteroids;

		static int Count();
		static bool debug;
		static bool wireframes;

	private:
		float angularspeed;
		float alpha;
		bool dying;
		bool dead;
		float size;
		bool immortal;

		void DrawDebugLines();
};

typedef std::list<Asteroid*>::iterator AsteroidIter;

#endif
