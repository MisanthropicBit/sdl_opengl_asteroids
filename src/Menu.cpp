#include "Menu.hpp"
#include "Game.hpp"
#include "GameStateGame.hpp"
#include <iostream>

Menu::Menu() {
}

bool Menu::Load() {
	titleImage.Load("./Graphics/Images/Title.png");
	starfield.Populate(100);
	newGameButton.Load(0, 0, "./Graphics/Images/NewGameButton.png", 1.f, 1.15f, "click");
	noAsteroidsButton.Load(0, 0, "./Graphics/Images/InitialAsteroidsText.png", 1.f, 1.15f, "click");
	soundButton.Load(0, 0, "./Graphics/Images/SoundButton.png", 1.f, 1.15f, "click");
	quitButton.Load(0, 0, "./Graphics/Images/QuitButton.png", 1.f, 1.15f, "click");
	numbersImage.Load("./Graphics/Images/Numbers.png");
	numbersImage.ScaleTo(64.f, numbersImage.Height);
	sy = 32.f * (Game::noInitialAsteroids - 1);

	newGameButton.SetPosition(150, 150);
	noAsteroidsButton.SetPosition(150, 250);
	noAsteroidsButton.SetClipRect(0, 0, 560, 64);
	soundButton.SetPosition(150, 350);
	soundButton.SetClipRect(0, 0, 165, 64);
	quitButton.SetPosition(150, 450);
	quitButton.SetClipRect(0, 0, 110, 75);

	for (int i = 0; i < 6; ++i) {
		Asteroid *asteroid = new Asteroid(Math::Random(0, Game::Width), Math::Random(0, Game::Height), 0.f, Math::Random(30.f, 70.f));
		asteroid->speed    = Vector2(Math::Random(0.05f, 0.015f), Math::Random(0.05f, 0.015f));
		asteroid->SetImmortality(true);
		asteroids.push_back(asteroid);
	}

	return true;
}

void Menu::OnEvent(SDL_Event& event) {
	Event::OnEvent(event);
}

void Menu::Update(int dt) {
	starfield.Update(dt);
	newGameButton.Update(dt);
	noAsteroidsButton.Update(dt);
	soundButton.Update(dt);
	quitButton.Update(dt);

	for (int i = 0; i < asteroids.size(); ++i)
		asteroids[i]->Update(dt);
}

void Menu::Draw() {
	starfield.Draw();

	for (int i = 0; i < asteroids.size(); ++i)
		asteroids[i]->Draw();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	titleImage.Draw(Game::Width / 2.f - titleImage.Width / 2.f, 20);
	newGameButton.DrawClipped(0, 0, 256, 64);
	noAsteroidsButton.DrawClipped(0, 0, 560, 64);
	soundButton.DrawClipped(0, 0, 165, 64);
	quitButton.DrawClipped(0, 0, 110, 75);
	numbersImage.DrawClipped(800, 270, 0.f, sy, 32.f, 32.f);

	glDisable(GL_TEXTURE_2D);

	if (!Game::audioManager.IsSoundEnabled()) {
        glLineWidth(8.f);
	    soundButton.HasHover() ? glColor3f(1.f, 0.f, 0.f) : glColor3f(1.f, 1.f, 1.f);
	    //float offset = soundButton.GetScale() * 20.f;

        glTranslatef(soundButton.GetX() + (soundButton.GetClipRect().GetWidth() / 2.f) * soundButton.GetScale(), soundButton.GetY() + soundButton.GetHeight() / 2.f, 0.f);
        glScalef(soundButton.GetScale(), soundButton.GetScale(), 0.f);
        glTranslatef(-(soundButton.GetX() + (soundButton.GetClipRect().GetWidth() / 2.f) * soundButton.GetScale()), -(soundButton.GetY() + soundButton.GetHeight() / 2.f), 0.f);

	    Graphics::BeginLines();
            glVertex2f(soundButton.GetX() - 10.f, soundButton.GetY() + soundButton.GetHeight() / 2.f);
            glVertex2f(soundButton.GetX() + soundButton.GetClipRect().GetWidth() + 10.f, soundButton.GetY() + soundButton.GetHeight() / 2.f);
	    Graphics::End();

	    glLineWidth(1.f);
	}
}

void Menu::UnLoad() {
	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < asteroids.size(); ++i) {
		if (asteroids[i]) {
			delete asteroids[i];
			asteroids[i] = NULL;
		}
	}

	asteroids.clear();
	titleImage.Release();
	numbersImage.Release();
}

void Menu::Pause() {
    newGameButton.Reset();
    noAsteroidsButton.Reset();
    soundButton.Reset();
    quitButton.Reset();
}

void Menu::Resume() {
}

void Menu::OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode) {
	switch (key) {
		case SDLK_ESCAPE:
			Game::Quit();
			break;

		case SDLK_d:
			Asteroid::ToggleDebugLines();
			break;

		case SDLK_w:
			Asteroid::ToggleWireframes();
			break;

		default:
			break;
	}
}

void Menu::OnLeftMouseButtonDown(int mx, int my) {
	if (quitButton.HasHover())
		Game::Quit();
	else if (newGameButton.HasHover())
		Game::PushState(new GameStateGame());
    else if (noAsteroidsButton.HasHover()) {
        sy += 32.f;

        if (sy > 352.f)
            sy = 0.f;

        Game::noInitialAsteroids = (static_cast<int>(sy) / 32) + 1;
    } else if (soundButton.HasHover()) {
        if (Game::audioManager.IsSoundEnabled())
            Game::audioManager.DisableSounds();
        else
            Game::audioManager.EnableSounds();
    }
}

void Menu::OnRightMouseButtonDown(int mx, int my) {
    if (noAsteroidsButton.HasHover()) {
        sy -= 32.f;

        if (sy < 0.f)
            sy = 352.f;

        Game::noInitialAsteroids = (static_cast<int>(sy) / 32) + 1;
    }
}

void Menu::OnMouseMove(int mx, int my, int relx, int rely, Uint8 state) {
	newGameButton.OnMouseMove(mx, my);
	noAsteroidsButton.OnMouseMove(mx, my);
	soundButton.OnMouseMove(mx, my);
	quitButton.OnMouseMove(mx, my);
}
