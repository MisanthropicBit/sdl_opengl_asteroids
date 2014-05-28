#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "Event.hpp"

//=========================================================================================================================

class GameState : public Event { // Abstract base class
	public:
		GameState();
		~GameState();

		virtual bool Load()                    = 0; // All
		virtual void OnEvent(SDL_Event& event) = 0; // these
		virtual void Update(int dt)            = 0; // member
		virtual void Draw()                    = 0; // functions
		virtual void UnLoad()                  = 0; // are
		virtual void Pause()                   = 0; // pure
		virtual void Resume()                  = 0; // virtual

	protected:
		bool paused;
};

//=========================================================================================================================

#endif
