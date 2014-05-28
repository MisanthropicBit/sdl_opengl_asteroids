#include "Game.hpp"
#include "GameStateIntro.hpp"
#include "Menu.hpp"
#include "Graphics.hpp"
#include "Math.hpp"
#include <iostream>

//=========================================================================================================================

GameStateIntro::GameStateIntro() {
}

//=========================================================================================================================

bool GameStateIntro::Load() {
	whichLogo = 1;
	Reset();

	SDL_ShowCursor(0);
	Game::audioManager.PlaySound("Thunder", 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	actor.position.x = -20.f;
	actor.position.y = Game::Height / 2.f;
	actor.speed.x = 0.15f;
	actor.Rotate(0.f);
	randomTimer.SetRange(300, 1200);

	if(StormLogo.Load("./Graphics/Images/StormGamesLogoResized.png") &&
	   APILogo.Load("./Graphics/Images/Logos.png") &&
	   Fader.Load("./Graphics/Images/Fader.png"))
	{
		StormLogo.ScaleTo(Game::Width, Game::Height);
		Fader.ScaleTo(Game::Width, Game::Height);
		Fader.SetAlpha(0.f);

		return true;
	}

	return false;
}

//=========================================================================================================================

void GameStateIntro::OnEvent(SDL_Event& event) {
	Event::OnEvent(event);
}

//=========================================================================================================================

void GameStateIntro::Update(int dt) {
	if (whichLogo == 2) {
		actor.Update(dt);

		if (randomTimer.HasExpired(dt)) {
			actor.FireShot();
			Game::audioManager.PlaySound("Shot", 0);
		}
	}

	if(state == FadingIn) {
		fadeInTime -= dt;

		if(fadeInTime <= 0) {
		    //alpha = Math::Lerp(0.f, 1.f, (FadeInTime - fadeInTime) / FadeInTime);
		    //std::cout << "Alpha: " << alpha << std::endl;
			alpha += 0.003f * (float)dt;
			fadeInTime = UpdateInterval;

			if(alpha >= 1.f) {
				alpha = 1.f;
				Fader.SetAlpha(1.f);
				state = Displaying;
			} else
                Fader.SetAlpha(alpha);
		}
	} else if(state == Displaying) {
		displayTime -= dt;

		if(displayTime <= 0)
			state = FadingOut;
	} else if(state == FadingOut) {
		fadeOutTime -= dt;

		if(fadeOutTime <= 0) {
		    //alpha = Math::Lerp(1.f, 0.f, (FadeOutTime - fadeOutTime) / FadeOutTime);
			alpha -= 0.003f * (float)dt;
			fadeOutTime = UpdateInterval;

			if(alpha <= 0.f) {
				alpha = 0.f;
				Fader.SetAlpha(0.f);

				if(whichLogo == 1) {
					Reset();
					++whichLogo;
				} else {
					Game::PopState();
					Game::PushState(new Menu());
				}
			} else
                Fader.SetAlpha(alpha);
		}
	}
}

//=========================================================================================================================

void GameStateIntro::Draw() {
	if(whichLogo == 1)
		StormLogo.Draw(0, 0);
	else {
		APILogo.Draw(0, 0);
		actor.Draw();
	}

	Fader.Draw(0, 0);
}

//=========================================================================================================================

void GameStateIntro::UnLoad() {
	SDL_ShowCursor(1);
	glDisable(GL_TEXTURE_2D);

	// Release textures
	StormLogo.Release();
	APILogo.Release();
	Fader.Release();
}

//=========================================================================================================================

void GameStateIntro::Pause() {
}

//=========================================================================================================================

void GameStateIntro::Resume() {
}

//=========================================================================================================================

void GameStateIntro::OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode) {
	if(whichLogo == 1) {
		++whichLogo;
		Game::audioManager.AllChannelsFadeOut(500);
		Fader.SetAlpha(0.f);
		Reset();
	} else {
		Game::PopState();
		Game::PushState(new Menu());
	}
}

//=========================================================================================================================

void GameStateIntro::OnLeftMouseButtonDown(int mx, int my) {
	if(whichLogo == 1) {
		++whichLogo;
		Game::audioManager.AllChannelsFadeOut(500);
		Fader.SetAlpha(0.f);
		Reset();
	} else {
		Game::PopState();
		Game::PushState(new Menu());
	}
}

//========================================================================================================================

void GameStateIntro::Reset() {
	alpha = 0.f;
	fadeInTime  = UpdateInterval;
	displayTime = DisplayTime;
	fadeOutTime = UpdateInterval;
	state = FadingIn;
}

//========================================================================================================================
