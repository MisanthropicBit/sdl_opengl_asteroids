#ifndef GAME_STATE_INTRO_HPP
#define GAME_STATE_INTRO_HPP

#include "GameState.hpp"
#include "Texture2D.hpp"
#include "RandomTimer.hpp"
#include "SpaceShip.hpp"

//=========================================================================================================================

const int DisplayTime    = 3000;
const int UpdateInterval = 40;

//=========================================================================================================================

enum IntroState {
	FadingIn,
	Displaying,
	FadingOut
};

//=========================================================================================================================

class GameStateIntro : public GameState {
	public:
		GameStateIntro();

		bool Load();
		void OnEvent(SDL_Event& event);
		void Update(int dt);
		void Draw();
		void UnLoad();
		void Pause();
		void Resume();

		void Reset();

		void OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode);
		void OnLeftMouseButtonDown(int mx, int my);

	private:
		Texture2D StormLogo;
		Texture2D APILogo;
		Texture2D Fader;
		SpaceShip actor;
		RandomTimer randomTimer;

		int whichLogo;
		float alpha;
		IntroState state;
		int updateInterval;
		int fadeInTime;
		int displayTime;
		int fadeOutTime;
};

//=========================================================================================================================

#endif
