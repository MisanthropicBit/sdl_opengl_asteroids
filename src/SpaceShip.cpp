#include "SDL_opengl.h"
#include "Game.hpp"
#include "SpaceShip.hpp"
#include "Graphics.hpp"

#include <iostream>

SpaceShip::SpaceShip() {
	position.Zero();
	speed.Zero();
	acceleration.Zero();
	lives = 3;
	angle = 0.f;
	color = Color::Blue;
	Shots.clear();
	isHit = false;
	immortalityTimer = 5000.f;
	colorChangeAmount = 0.f;
	fireTimer = 0.f;
}

SpaceShip::SpaceShip(float x, float y, float angle) {
	position.x = x;
	position.y = y;
	speed.Zero();
	acceleration.Zero();
	lives = 3;
	this->angle = angle;
	color = Color::Blue;
	Shots.clear();
	isHit = false;
	immortalityTimer = 5000.f;
	colorChangeAmount = 0.f;
	fireTimer = 0;
}

SpaceShip::~SpaceShip() {
	for(int i = 0; i < Shots.size(); ++i)
	{
		if(Shots[i])
			delete Shots[i];
	}

	Shots.clear();
}

void SpaceShip::Update(int dt) {
	speed += acceleration;
	position += speed * dt;

	if(position.x < 0)
		position.x = Game::Width;
	if(position.y < 0)
		position.y = Game::Height;
	if(position.x > Game::Width)
		position.x = 0;
	if(position.y > Game::Height)
		position.y = 0;

	if(fireTimer >= 0)
		fireTimer -= dt;

	// Update shots
	for(int i = 0; i < Shots.size(); ++i) {
		if(Shots[i]) {
			if(Shots[i]->position.x < 0.f || Shots[i]->position.y < 0.f ||
			   Shots[i]->position.x > Game::Width || Shots[i]->position.y > Game::Height)
			{
				delete Shots[i];
				Shots.erase(Shots.begin() + i);
				--i;
			} else
				Shots[i]->Update(dt);
		}
	}

	// Resolve shot <-> asteroid collisions
	for(AsteroidIter aiter = Asteroid::Asteroids.begin(); aiter != Asteroid::Asteroids.end();) {
		if(!(*aiter)->IsDead() && !(*aiter)->IsDying()) {
			if(ShotsCollideWith((*aiter))) {
				delete (*aiter);
				aiter = Asteroid::Asteroids.erase(aiter);
			} else
				++aiter;
		} else
			++aiter;
	}

	// Handle 5 seconds of immortality if the player got hit
	if(isHit) {
		immortalityTimer -= dt;

		if(immortalityTimer <= 0.f) {
			color = Color::Blue;
			isHit = false;
		} else {
			colorChangeAmount += COLOR_CHANGE_RATE * dt;
			colorChangeAmount = Math::Clamp(colorChangeAmount, 0.f, 1.f);

			//////////////////////////////////////////////////////////////////////////////////
			// Since we are linear interpolating colors between red (1.f, 0.f, 0.f) and
			// blue (0.f, 0.f, 1.f), we do not need change the green component of the
			// player's color.
			//////////////////////////////////////////////////////////////////////////////////
			color.R = Math::Lerp(targetColor1.R, targetColor2.R, colorChangeAmount);
			color.B = Math::Lerp(targetColor1.B, targetColor2.B, colorChangeAmount);

			if(colorChangeAmount >= 1.f) {
				// Reset lerp weight and switch colors
				colorChangeAmount = 0.f;
				targetColor1 = (targetColor1 == Color::Blue) ? Color::Red : Color::Blue;
				targetColor2 = (targetColor2 == Color::Blue) ? Color::Red : Color::Blue;
			}
		}
	}
}

void SpaceShip::Draw() {
	glPushMatrix();
	glLoadIdentity();

	for(int i = 0; i < Shots.size(); ++i) {
		if(Shots[i])
			Shots[i]->Draw();
	}

	glLoadIdentity();

	glTranslatef(position.x, position.y, 0.f);
	glRotatef(-angle, 0.f, 0.f, 1.f);
	glTranslatef(-position.x, -position.y, 0.f);

	Graphics::BeginPolygons();
		glColor3f(color.R, color.G, color.B);
			glVertex2f(position.x + 10.f, position.y);
			glVertex2f(position.x - 10.f, position.y + 10.f);
			glVertex2f(position.x - 5.f, position.y);
			glVertex2f(position.x - 10.f, position.y - 10.f);
	Graphics::End();

	glPopMatrix();
}

void SpaceShip::Rotate(float angle) {
	if(angle < 0.f)
		angle += 360.f;
	else if(angle > 360.f)
		angle -= 360.f;

	this->angle = angle;
}

void SpaceShip::FireShot() {
	if(fireTimer <= 0.f) {
		Shot* shot = new Shot(position.x, position.y, Vector2(0.8f * Math::Cos(GetAngle()), 0.8f * Math::Sin(-GetAngle())));
		Shots.push_back(shot);
		fireTimer = FIRE_DELAY;
	}
}

float SpaceShip::GetAngle() const {
	return angle;
}

void SpaceShip::SetColor(Color& color) {
	this->color = color;
}

bool SpaceShip::CollidesWith(Asteroid* asteroid) {
	if(asteroid->PointInPolygon(position.x + 10.f, position.y) ||
	   asteroid->PointInPolygon(position.x - 10.f, position.y + 10.f) ||
	   asteroid->PointInPolygon(position.x - 5.f, position.y) ||
	   asteroid->PointInPolygon(position.x - 10.f, position.y - 10.f))
			return true;

	return false;
}

// Collision detection method taken from:
// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
//
// Note the alternative solution in the 2nd reply.
bool SpaceShip::ShotsCollideWith(Asteroid* asteroid) {
	if(!asteroid)
		return false;

	Vector2 vertex1;
	Vector2 vertex2;
	Asteroid* asteroid1 = NULL;
	Asteroid* asteroid2 = NULL;
	int ni = 0; // Next index
	int side = 0;
	int nextSide = 0;
	Vector2 point;
	Vector2 endpoint;
	Vector2 dir;

	// Loop through all shots
	for(vector<Shot*>::iterator it = Shots.begin(); it != Shots.end();) {
		if((*it)) {
			// Store start- and end point of the current shot
			point = (*it)->position;
			dir = (*it)->GetDirection();
			endpoint = point + dir;

			// If either end of the shot is inside the asteroid, we have a hit!
			if(asteroid->PointInPolygon(endpoint) || asteroid->PointInPolygon(point)) {
				// Create two new asteroids to hold the two new slices
				asteroid1 = new Asteroid();
				asteroid2 = new Asteroid();

				// Temporary to store result of vector/line intersections
				Vector2 tempVector;

				// Loop through the vertices of the asteroid and distribute them among the two slices
				for(int v = 0; v < asteroid->vertices.size(); ++v) {
					// Get next vertex index
					ni = (v + 1 == asteroid->vertices.size()) ? 0 : v + 1;

					vertex1.x = asteroid->vertices[v].x;
					vertex1.y = asteroid->vertices[v].y;
					vertex2.x = asteroid->vertices[ni].x;
					vertex2.y = asteroid->vertices[ni].y;

					side = Math::PointSideOfLine(vertex1, point, endpoint);
					nextSide = Math::PointSideOfLine(vertex2, point, endpoint);

					// Vertices are distributed arbitrarily
					if(side == 1)
						asteroid1->AddVertex(asteroid->vertices[v]);
					else
						asteroid2->AddVertex(asteroid->vertices[v]);

					// The two current points were on different sides of the 'shot-line', there must be an intersection
					if(side != nextSide) {
						tempVector = Math::VectorLineIntersection(point, dir, vertex1, vertex2 - vertex1);

						// Both slices get this vertex
						asteroid1->AddVertex(tempVector);
						asteroid2->AddVertex(tempVector);
					}
				}

				// Generate new asteroid speeds based on shot speed
				// OLD:
//				asteroid1->speed = Vector2(-(*it)->speed.y, (*it)->speed.x) * 0.1f; // Left-hand
//				asteroid2->speed = Vector2((*it)->speed.y, -(*it)->speed.x) * 0.1f; // Right-hand

                // NEW (TEST):
                Vector2 endpoint_n1 = Vector2(dir.y, -dir.x);
                Vector2 endpoint_n2 = Vector2(-endpoint_n1.x, -endpoint_n1.y);
                Vector2 as_n1 = Vector2(asteroid->speed.y, -asteroid->speed.x);
                Vector2 as_n2 = Vector2(-as_n1.x, -as_n1.y);

                asteroid1->speed = endpoint_n2 + as_n2;
                asteroid2->speed = endpoint_n1 + as_n1;
                asteroid1->speed *= 0.005f;
                asteroid2->speed *= 0.005f;
                asteroid1->speed += asteroid->speed * 0.5f;
                asteroid2->speed += asteroid->speed * 0.5f;

				Asteroid::Asteroids.push_back(asteroid1);
				Asteroid::Asteroids.push_back(asteroid2);

				// Remove shot from container
				delete (*it);
				it = Shots.erase(it);

				return true;
			} else
				++it;
		}
	}

	return false;
}

void SpaceShip::GotHit() {
	isHit = true;
	immortalityTimer = IMMORTALITY_PERIOD;
	targetColor1 = Color::Red;
	targetColor2 = Color::Blue;
	colorChangeAmount = 0.f;
	--lives;
}

bool SpaceShip::IsHit() const {
	return isHit;
}
