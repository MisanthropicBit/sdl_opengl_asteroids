#ifndef SOUND_HPP
#define SOUND_HPP

#include "SDL_mixer.h"
#include <string>

class Sound {
	public:
		Sound(const std::string& file);
		~Sound();

		Mix_Chunk* GetData() const;
		bool IsValid() const;

	protected:
		Mix_Chunk *sound;
};

#endif
