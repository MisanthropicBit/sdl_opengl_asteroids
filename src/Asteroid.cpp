#include "Asteroid.hpp"
#include "Graphics.hpp"
#include "Game.hpp"
#include "SDL_opengl.h"

std::list<Asteroid*> Asteroid::Asteroids;
bool Asteroid::debug      = false;
bool Asteroid::wireframes = false;

Asteroid::Asteroid() {
	speed.Zero();
	angularspeed = Math::Random(-0.15f, 0.15f);
	alpha        = 1.f;
	dying        = false;
	dead         = false;
	immortal     = false;
	size         = 0.f;
	color        = Color::Gray;
}

Asteroid::Asteroid(float x, float y, float angle, float size) : Polygon(x, y) {
	Set(x, y, angle, size);
}

Asteroid::~Asteroid() {
}

void Asteroid::Set(float x, float y, float angle, float size) {
	speed.x = Math::Random(0.1f, 0.15f) * Math::Cos(Math::Random(0.f, 359.f));
	speed.y = Math::Random(0.1f, 0.15f) * Math::Sin(Math::Random(0.f, 359.f));

	angularspeed = Math::Random(-0.15f, 0.15f);
	alpha        = 1.f;
	dying        = false;
	dead         = false;
	immortal     = false;
	this->size   = Math::Clamp(size, 0.f, size);
	color        = Color::Gray;

	Vector2 vertex;

	for (int i = 0; i < 6; ++i) {
		vertex.x = x + size * Math::Cos(i * 60.f + Math::Random(10.f, 30.f));
		vertex.y = y + size * Math::Sin(i * 60.f + Math::Random(10.f, 30.f));

		AddVertex(vertex);
	}
}

void Asteroid::Update(int dt) {
	angle += angularspeed;
	float x, y = 0;

	// Rotate points (using 2D rotation matrix calculations)
	for(int i = 0; i < vertices.size(); ++i) {
		x = position.x + Math::Cos(angularspeed) * (vertices[i].x - position.x) - Math::Sin(angularspeed) * (vertices[i].y - position.y);
		y = position.y + Math::Sin(angularspeed) * (vertices[i].x - position.x) + Math::Cos(angularspeed) * (vertices[i].y - position.y);

		vertices[i].x = x;
		vertices[i].y = y;
		vertices[i]  += speed * dt;
	}

	position = GetCenter();

	if (position.x + INITIAL_SIZE < 0 && speed.x < 0.f) {
		for (int i = 0; i < vertices.size(); ++i)
			vertices[i].x += (Game::Width + INITIAL_SIZE * 2);
	}

	if (position.x - INITIAL_SIZE > Game::Width && speed.x > 0.f) {
		for (int i = 0; i < vertices.size(); ++i)
			vertices[i].x -= (Game::Width + INITIAL_SIZE * 2);
	}

    if (position.y + INITIAL_SIZE < 0 && speed.y < 0.f) {
		for (int i = 0; i < vertices.size(); ++i)
			vertices[i].y += (Game::Height + INITIAL_SIZE * 2);
	}

	if (position.y - INITIAL_SIZE > Game::Height && speed.y > 0.f) {
		for (int i = 0; i < vertices.size(); ++i)
			vertices[i].y -= (Game::Height + INITIAL_SIZE * 2);
	}

	if (!IsInSight()) {
		if (speed.y >= -1.f && speed.y <= 1.f) {
			if (speed.x >= -0.3f && speed.x <= 0.3f) {
				Vector2 newDir = Vector2(Game::Width / 2.f, Game::Height / 2.f + Math::Random(-100.f, 100.f)) - position;
				newDir.Normalize();
				newDir *= speed.Length();
				speed   = newDir;
			}
		}
	}

	if (!immortal) {
		if (!dying && !dead) {
			if (GetArea() < MIN_AREA)
				dying = true;
		} else if (dying && !dead) {
			alpha -= 0.002f * dt;

            if (alpha <= 0.f) {
				dead  = true;
				dying = false;
			}
		}
	}
}

void Asteroid::Draw() {
	if (vertices.size() < 3)
		return;

	glLoadIdentity();

	//////////////////////////////////////////////////////////////////////////////////
	// We use BeginTriangleFan() here instead of BeginPolygons() (uses GL_POLYGONS)
	// because the latter only supports concave polygons, and in some instances of
	// the game, we might encounter convex polygons. Additionally, modern graphics
	// hardware is optimized to draw triangles.
	//////////////////////////////////////////////////////////////////////////////////
	(Asteroid::wireframes) ? Graphics::BeginLineLoop() : Graphics::BeginTriangleFan();
		glColor4f(color.R, color.G, color.B, alpha);

		for (int i = 0; i < vertices.size(); ++i)
				glVertex2f(vertices[i].x, vertices[i].y);
	Graphics::End();

    if (Asteroid::debug)
		DrawDebugLines();
}

void Asteroid::DrawDebugLines() {
	glLoadIdentity();

	Graphics::BeginLines();
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(0.f, 0.f);
		glVertex2f(position.x, position.y);
		glColor3f(1.f, 0.f, 0.f);

		for (int i = 0; i < vertices.size(); ++i) {
			glVertex2f(0, 0);
			glVertex2f(vertices[i].x, vertices[i].y);
		}
	Graphics::End();
}

void Asteroid::ToggleDebugLines() {
	Asteroid::debug = !Asteroid::debug;
}

void Asteroid::ToggleWireframes() {
	Asteroid::wireframes = !Asteroid::wireframes;
}

void Asteroid::SetImmortality(bool flag) {
	immortal = flag;
}

bool Asteroid::IsDying() const {
	return dying;
}

bool Asteroid::IsDead() const {
	return dead;
}

bool Asteroid::IsInSight() {
	for (int i = 0; i < vertices.size(); ++i) {
		// If just one point is on screen, the asteroid must be visible and we return true
		if (Math::PointInsideRectangle(vertices[i], 0, Game::Width, 0, Game::Height))
			return true;
	}

	return false;
}

float Asteroid::GetSize() const {
	return size;
}

int Asteroid::Count() {
	return Asteroid::Asteroids.size();
}
