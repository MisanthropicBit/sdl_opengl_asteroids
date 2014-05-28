#include "StarField.hpp"
#include "Math.hpp"
#include "Graphics.hpp"
#include "Game.hpp"
#include "SDL_opengl.h"
#include <iostream>

Star::Star(Vector2 position) {
	this->position = position;
	blinking = false;
	visible = true;
	blinkIntervalTimer = Math::Random(250.f, 800.f);
	blinkPeriodTimer = Math::Random(1000.f, 1500.f);
	alpha = Math::Random(0.6f, 1.f);
}

void Star::Update(int dt) {
	if(blinking) {
		blinkPeriodTimer -= dt;

		if(blinkPeriodTimer <= 0) {
			blinkPeriodTimer = BLINK_PERIOD;
			blinking = false;
			visible = true;
		} else {
			blinkIntervalTimer -= dt;

			if(blinkIntervalTimer <= 0) {
				blinkIntervalTimer = BLINK_INTERVAL;
				visible = !visible;
			}
		}
	}
}

StarField::StarField() {
	changeTimer = 0;
	stars.clear();
}

void StarField::Populate(int noStars) {
	stars.clear();

	for(int i = 0; i < noStars; ++i)
		stars.push_back(Star(Vector2(Math::Random(0, Game::Width), Math::Random(0, Game::Height))));
}

void StarField::Update(int dt) {
	changeTimer -= dt;

	if(changeTimer <= 0) {
		changeTimer = Math::Random(1000, 1500);

		for(int i = 0; i < 5; ++i) {
			int random = Math::Random(0, stars.size() - 1);
			stars[random].blinking = true;
			stars[random].blinkIntervalTimer = Math::Random(1000, 1500);
		}
	}

	for(int i = 0; i < stars.size(); ++i)
		stars[i].Update(dt);
}

void StarField::Draw() {
	glLoadIdentity();

	for(int i = 0; i < stars.size(); ++i) {
		if(stars[i].visible) {
			Graphics::BeginQuads();
				Graphics::DrawRectangle(stars[i].position.x, stars[i].position.y, 2, 2, 0.f, Color(1.f, 1.f, 1.f, stars[i].alpha));
			Graphics::End();
		}
	}
}