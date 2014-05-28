#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "GameState.hpp"
#include "SpaceShip.hpp"
#include "StarField.hpp"
#include "Explosion.hpp"
#include "Texture2D.hpp"

const float TURN_SPEED = 0.3f;

class GameStateGame : public GameState {
	public:
		GameStateGame();
		~GameStateGame();

		bool Load();
		void OnEvent(SDL_Event& event);
		void Update(int dt);
		void UpdatePlayer(int dt);
		void UpdateAsteroids(int dt);
		void UpdateStarField(int dt);
		void Draw();
		void UnLoad();
		void Pause();
		void Resume();

		void OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode);
		void OnKeyUp(SDLKey key, SDLMod modifier, Uint16 unicode);

	private:
		bool turnleft;
		bool turnright;
		SpaceShip player;
		StarField starField;
		Explosion *explosion;
		Texture2D victoryImage;
};

#endif
