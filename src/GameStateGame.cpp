#include "GameStateGame.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Menu.hpp"
#include "Asteroid.hpp"

GameStateGame::GameStateGame() {
	turnleft  = false;
	turnright = false;
	explosion = NULL;
}

GameStateGame::~GameStateGame() {
}

bool GameStateGame::Load() {
    if (!victoryImage.Load("./Graphics/Images/Victory.png"))
        return false;

	player.position.x = Game::Width  / 2.f;
	player.position.y = Game::Height / 2.f;

	for (int i = 0; i < Game::noInitialAsteroids; ++i) {
		int x = Math::Random(0.f, Game::Width - 30.f);
		int y = Math::Random(0.f, Game::Height - 30.f);

		if (x >= Game::Width / 3.f && x <= Game::Width / 2.f)
			x = Game::Width / 3.f;
		else if (x >= Game::Width / 2.f && x <= Game::Width - Game::Width / 3.f)
			x = Game::Width - Game::Width / 3.f;

		if (y >= Game::Height / 3.f && y <= Game::Height / 2.f)
			y = Game::Height / 3.f;
		else if (y >= Game::Height / 2.f && y <= Game::Height - Game::Height / 3.f)
			y = Game::Height / 3.f;

		Asteroid::Asteroids.push_back(new Asteroid(x, y, Math::Random(0.f, 359.f), INITIAL_SIZE));
	}

	Game::audioManager.SetMusicVolumePercentage(0.2f);
	starField.Populate(100);
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void GameStateGame::OnEvent(SDL_Event& event) {
	Event::OnEvent(event);
}

void GameStateGame::Update(int dt) {
	UpdateAsteroids(dt);
	UpdatePlayer(dt);
	UpdateStarField(dt);

	// Check for game over
	if (player.lives <= 0) {
		if (!explosion)
			explosion = new Explosion(40, player.position, 0.2f);
		else if (explosion) {
			explosion->Update(dt);

			if (explosion->dead) {
				delete explosion;
				explosion = NULL;
				Game::PopState();
			}
		}
	}
}

void GameStateGame::UpdatePlayer(int dt) {
	// Handle player input
	if (turnleft)
		player.Rotate(player.GetAngle() + TURN_SPEED * dt);
	else if (turnright)
		player.Rotate(player.GetAngle() - TURN_SPEED * dt);

	Uint8* keystate = SDL_GetKeyState(NULL);

	if (keystate[SDLK_UP]) {
		Vector2 thrust(0.005f, 0.005f);
		thrust.Rotate(player.GetAngle());
		player.speed += thrust;

		if (player.speed.Length() >= 0.5f) {
			player.speed.Normalize();
			player.speed /= 2.f;
		}
	}

	// Update player (and shots fired)
	player.Update(dt);

	if (!player.IsHit()) {
		// Check for player <-> asteroid collision
		for(AsteroidIter aiter = Asteroid::Asteroids.begin(); aiter != Asteroid::Asteroids.end(); ++aiter) {
			if(player.CollidesWith(*aiter) && !(*aiter)->IsDying() && !(*aiter)->IsDead()) {
				(*aiter)->speed = player.speed / 2.f;

				// Player got hit
				player.GotHit();
				Vector2 resultingDir = (*aiter)->speed + player.speed;
				resultingDir = resultingDir / 2.f;
				player.speed = resultingDir;
			}
		}
	}
}

void GameStateGame::UpdateAsteroids(int dt) {
	for (AsteroidIter aiter = Asteroid::Asteroids.begin(); aiter != Asteroid::Asteroids.end();) { // No increment in this for loop!
		if (!(*aiter)->IsDead()) {
			(*aiter)->Update(dt);
			++aiter;
		} else {
			delete (*aiter);
			aiter = Asteroid::Asteroids.erase(aiter);
		}
	}
}

void GameStateGame::UpdateStarField(int dt) {
	starField.Update(dt);
}

void GameStateGame::Draw() {
	starField.Draw();

    if (Asteroid::Count() > 0) {
        for(AsteroidIter aiter = Asteroid::Asteroids.begin(); aiter != Asteroid::Asteroids.end(); ++aiter) {
            if ((*aiter))
                (*aiter)->Draw();
        }
    } else {
        glEnable(GL_TEXTURE_2D);
        victoryImage.Draw(Game::Width / 2.f - 250.f, Game::Height / 2.f - 90.f);
        glDisable(GL_TEXTURE_2D);
    }

	if (player.lives > 0)
		player.Draw();
	else {
		if (explosion)
			explosion->Draw();
	}
}

void GameStateGame::UnLoad() {
	SDL_ShowCursor(SDL_ENABLE);
	victoryImage.Release();

	// Clear the list of asteroids
    for (AsteroidIter it = Asteroid::Asteroids.begin(); it != Asteroid::Asteroids.end(); ++it) {
        if (*it) {
            delete *it;
            *it = NULL;
        }
    }
}

void GameStateGame::Pause() {
}

void GameStateGame::Resume() {
}

void GameStateGame::OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode) {
	switch (key) {
		case SDLK_x:
			Game::audioManager.PlaySound("Shot", 0);
			player.FireShot();
			break;

		case SDLK_d:
			Asteroid::ToggleDebugLines();
			break;

		case SDLK_w:
			Asteroid::ToggleWireframes();
			break;

		case SDLK_LEFT:
			turnleft  = true;
			turnright = false;
			break;

		case SDLK_RIGHT:
			turnright = true;
			turnleft  = false;
			break;

		case SDLK_ESCAPE:
			Game::PopState();
			break;

		default:
			break;
	}
}

void GameStateGame::OnKeyUp(SDLKey key, SDLMod modifier, Uint16 unicode) {
	switch (key) {
		case SDLK_LEFT:
			turnleft = false;
			break;

		case SDLK_RIGHT:
			turnright = false;
			break;

		default:
			break;
	}
}
