#ifndef MENU_HPP
#define MENU_HPP

#include "GameState.hpp"
#include "StarField.hpp"
#include "Button.hpp"
#include "Asteroid.hpp"
#include <vector>

class Menu : public GameState {
	public:
		Menu();

		bool Load();
		void OnEvent(SDL_Event& event);
		void Update(int dt);
		void Draw();
		void UnLoad();
		void Pause();
		void Resume();

		void OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode);
		void OnLeftMouseButtonDown(int mx, int my);
		void OnRightMouseButtonDown(int mx, int my);
        void OnMouseMove(int mx, int my, int relx, int rely, Uint8 state);

	private:
		Texture2D titleImage;
		StarField starfield;

		Button newGameButton;
		Button noAsteroidsButton;
		Button soundButton;
		Button quitButton;
		Texture2D numbersImage;
		float sy;

		std::vector<Asteroid*> asteroids;
};

#endif
