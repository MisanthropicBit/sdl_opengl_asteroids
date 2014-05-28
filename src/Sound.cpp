#include "Sound.hpp"

Sound::Sound(const std::string& file) {
	sound = NULL;

	if (!file.empty())
		sound = Mix_LoadWAV(file.c_str());

	if (!sound) {
		Mix_FreeChunk(sound);
		sound = NULL;
	}
}

Sound::~Sound() {
	if (sound)
        Mix_FreeChunk(sound);

	sound = NULL;
}

Mix_Chunk* Sound::GetData() const {
    return sound ? sound : NULL;
}

bool Sound::IsValid() const {
	return (sound != NULL);
}
