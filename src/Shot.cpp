#include "SDL_opengl.h"
#include "Shot.hpp"
#include "Graphics.hpp"
#include <iostream>

Shot::Shot() {
	position.Zero();
	speed.Zero();
	dir.Zero();
}

Shot::Shot(float x, float y, const Vector2& speed) {
	position.x = x;
	position.y = y;
	this->speed = speed;
	dir.x = SHOT_LENGTH * Math::Cos(speed.GetAngle());
	dir.y = SHOT_LENGTH * Math::Sin(-speed.GetAngle());
}

Shot::~Shot() {
}

void Shot::Update(int dt) {
	position += speed * dt;
}

void Shot::Draw() {
	glLoadIdentity();

	// Necessary?
	glLineWidth(5.f);

	Graphics::BeginLines();
		glColor3f(0.f, 1.f, 0.f);
			glVertex2f(position.x, position.y);
			glVertex2f(position.x + dir.x, position.y + dir.y);
	Graphics::End();

	// Necessary?
	glLineWidth(1.f);
}

Vector2 Shot::GetDirection() {
	return dir;
}