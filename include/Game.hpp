#ifndef GAME_HPP
#define GAME_HPP

#include "SDL.h"
#include "Timer.hpp"
#include "GameState.hpp"
#include "AudioManager.hpp"
#include <vector>

const std::string VERSION = "(v1.0)";
const float Desired_FPS   = 60.f;
const float FrameTime     = 1 / Desired_FPS;

class Game {
	public:
		static int Run();
		static bool Initialize();
		static void HandleEvent();
		static void Think();
		static void Draw();
		static void CleanUp();

		static void SetCaption(const std::string& caption);
		static bool ReadSettingsFile(const std::string& file);
		static bool WriteSettingsFile(const std::string& file, bool defaults);

		static void PushState(GameState* gamestate);
		static void PopState();
		static void Quit();

		static AudioManager audioManager;
		static int Width;
		static int Height;
        static int noInitialAsteroids;

	private:
		static bool Running;
		static SDL_Event event;
		static Timer gameTimer;
		static std::vector<GameState*> States;
		static int lol;
};

#endif
