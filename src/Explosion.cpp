#include "SDL_opengl.h"
#include "Explosion.hpp"
#include "Graphics.hpp"

Particle::Particle(Vector2 position, Vector2 speed, const Color& color) {
	this->color    = color;
	alpha          = 1.f;
	this->position = position;
	this->speed    = speed;
	dead           = false;
}

void Particle::Update(int dt) {
	if (!dead) {
		position += speed;
		alpha    -= 0.0006f * static_cast<float>(dt);
		color.A  = alpha;

		if (alpha <= 0.f)
			dead = true;
	}
}

void Particle::Draw() {
    Graphics::DrawRectangle(position.x, position.y, 2, 2, 0.f, color);
}

Explosion::Explosion(int size, Vector2 position, float force) {
	float angleStep = 360.f / static_cast<float>(size);

	for (int i = 0; i < size; ++i) {
		Particle* particle = new Particle(position, Vector2(1.f, 1.f) * force, Color::Orange);
		particle->speed   *= Vector2(Math::Cos(angleStep * i) * Math::Random(1.f, 2.f), Math::Sin(angleStep * i) * Math::Random(1.f, 2.f));
		particles.push_back(particle);
	}

	dead = false;
}

Explosion::~Explosion() {
	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i])
			delete particles[i];
	}

	particles.clear();
}

void Explosion::Update(int dt) {
	if (!dead) {
		for (std::vector<Particle*>::iterator it = particles.begin(); it != particles.end();) { // No increment in this for loop!
			if ((*it)->dead) {
				delete (*it);
				it = particles.erase(it);
			} else {
				if ((*it))
					(*it)->Update(dt);

				++it;
			}
		}

		if (particles.empty())
			dead = true;
	}
}

void Explosion::Draw() {
    glPushMatrix();
	glLoadIdentity();

	Graphics::BeginQuads();
        for (int i = 0; i < particles.size(); ++i)
            particles[i]->Draw();
    Graphics::End();

    glPopMatrix();
}
