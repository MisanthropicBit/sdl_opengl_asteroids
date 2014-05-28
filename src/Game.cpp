#include "Game.hpp"
#include "GameStateIntro.hpp"
#include "Conversion.hpp"
#include "Menu.hpp"
#include <string>
#include <iostream>
#include <fstream>

bool Game::Running            = false;
int  Game::Width              = 640; // Default width
int  Game::Height             = 480; // Default height
int  Game::noInitialAsteroids = 1;
int  Game::lol                = 0;

SDL_Event Game::event;
Timer Game::gameTimer;
vector<GameState*> Game::States;
AudioManager Game::audioManager;

int Game::Run() {
	if(!Initialize())
		return -1;

	while (Running) {
		while (SDL_PollEvent(&event))
			HandleEvent();

		Think();
		Draw();
	}

	CleanUp();
	return 0;
}

bool Game::Initialize() {
    // Initialize SDL's video
	SDL_Init(SDL_INIT_EVERYTHING);

	Width  = 960;
	Height = 640;
	srand(SDL_GetTicks());

	if(!Graphics::SetVideoMode(Width, Height, 0, SDL_OPENGL))
		return false;

	SetCaption("Asteroids " + VERSION);

    // Set up the audio manager
	audioManager.SetAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 800);
	audioManager.AllocateChannels(4);

	if (!audioManager.LoadSound("./Sounds/Fireball.wav", "Shot") ||
        !audioManager.LoadSound("./Sounds/Explosion.wav", "Explosion") ||
        !audioManager.LoadSound("./Sounds/RainAndThunder.ogg", "Thunder") ||
        !audioManager.LoadSound("./Sounds/Click.wav", "click")) {
            std::cerr << "Error: Failed to load one or more sounds" << std::endl;
            return false;
    }

	audioManager.SetSpecificSoundVolume("Shot", MIX_MAX_VOLUME);
	audioManager.SetSpecificSoundVolumePercentage("Explosion", 0.5f);
    audioManager.SetSpecificSoundVolumePercentage("click", 0.6f);

    // Read a binary file containing saved settings
    if (!ReadSettingsFile("./settings")) {
        std::cerr << "Error: Unable to read settings file, setting defaults" << std::endl;
        audioManager.EnableSounds();
        noInitialAsteroids = 1;

        if (!WriteSettingsFile("./settings", true)) {
            std::cerr << "Error: Unable to write default settings to file" << std::endl;
            return false;
        }
    }

	Graphics::SetClearColor(0.f, 0.f, 0.f, 0.f);
	Graphics::Set2dMode(Width, Height);

	if (Graphics::NPOT_TexturesSupported())
		std::cout << "NPOT textures are supported (v";
	else
		std::cout << "NPOT textures are not supported (v";

	std::cout << Graphics::GetOpenGLContextVersion() << ")" << std::endl;

    // Tell OpenGL to anti-alias lines
	glEnable(GL_LINE_SMOOTH);

	States.clear();
	PushState(new GameStateIntro());

	if(States.back())
		Running = true;

    // The above loading will result in a large
    // delta time when the first update call is made.
    // Instead we make a single call to 'Think(...)'
    // so that the first delta time actually used to
    // update the game is within reason. If we didn't
    // the first update would take a very big step and
    // ruin the smoothness of the intro.
    Think();

	return Running;
}

void Game::HandleEvent() {
	if(States.back())
		States.back()->OnEvent(event);
}

void Game::Think() {
    int dt = gameTimer.GetDeltaTime();

//    if (s_dt > FrameTime) {
//        s_dt = FrameTime;
//        dt = FrameTime * 1000;
//    }

    if (Game::lol < 5) {
        if (Game::lol == 0)
            dt = FrameTime * 1000;

        std::cout << "Delta-time: " << dt << std::endl;
        ++Game::lol;
    }

    float s_dt = static_cast<float>(dt) / 1000.f;

	if(States.back())
		States.back()->Update(dt);

    // Are we ahead of or on schedule? Then let the OS get some rest
    if (FrameTime - s_dt >= 0.f) {
        SDL_Delay(FrameTime - s_dt);
    }
}

void Game::Draw() {
	Graphics::ClearScreen();

    if (States.back())
        States.back()->Draw();

	Graphics::SwapBuffers();
}

void Game::CleanUp() {
	for(int i = 0; i < States.size(); ++i) {
		if(States[i]) {
		    States[i]->UnLoad();
			delete States[i];
			States[i] = NULL;
		}
	}

    if (!WriteSettingsFile("./settings", false)) {
        std::cerr << "Error: Unable to write to settings file, saving defaults" << std::endl;
        audioManager.EnableSounds();
        noInitialAsteroids = 1;

        std::ofstream fileStream("./settings");

        if (!fileStream.is_open())
            std::cerr << "Error: Unable to save defaults" << std::endl;

        const char* defaultValues = "11";
        fileStream.write(defaultValues, sizeof(defaultValues));
        fileStream.close();
    }

	States.clear();
	SDL_Quit();
}

void Game::SetCaption(const std::string& caption) {
	SDL_WM_SetCaption(caption.c_str(), NULL);
}

bool Game::ReadSettingsFile(const std::string& file) {
    if (file.empty())
        return false;

    std::ifstream fileStream(file.c_str());

    if (!fileStream.is_open())
        return false;

    int soundEnabled = 0;

    fileStream.read(reinterpret_cast<char*>(&soundEnabled), sizeof(soundEnabled));
    fileStream.read(reinterpret_cast<char*>(&noInitialAsteroids), sizeof(noInitialAsteroids));

    if (soundEnabled == 1)
        audioManager.EnableSounds();
    else
        audioManager.DisableSounds();

    fileStream.close();

    return true;
}

bool Game::WriteSettingsFile(const std::string& file, bool defaults) {
    if (file.empty())
        return false;

    std::ofstream fileStream(file.c_str());

    if (!fileStream.is_open())
        return false;

    int soundEnabled = (audioManager.IsSoundEnabled() ? 1 : 0);

    if (!defaults) {
        fileStream.write(reinterpret_cast<const char*>(&soundEnabled), sizeof(soundEnabled));
        fileStream.write(reinterpret_cast<const char*>(&noInitialAsteroids), sizeof(noInitialAsteroids));
    } else {
        const char* defaultValues = "11";
        fileStream.write(defaultValues, sizeof(defaultValues));
    }

    fileStream.close();

    return true;
}

void Game::PushState(GameState* gamestate) {
	if(gamestate) {
	    if (!States.empty() && States.back())
            States.back()->Pause();

		States.push_back(gamestate);

		if(!States.back()->Load()) { // Invalid state, quit application
			std::cerr << "Error: Game State failed to load correctly when pushed onto stack" << endl << endl;
			Quit();
		}
	}
}

void Game::PopState() {
	if(!States.empty()) {
		if(States.back()) {
			States.back()->UnLoad();
			delete States.back();
			States.pop_back();
		}

		if (!States.empty() && States.back())
            States.back()->Resume();
	}
}

void Game::Quit() {
	Running = false;
}

int main(int argc, char* args[]) {
	return Game::Run();
}
