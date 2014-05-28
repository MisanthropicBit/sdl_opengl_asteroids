#include "AudioManager.hpp"
#include "Math.hpp"
#include <iostream>

AudioManager::AudioManager() {
	Sounds.clear();
	Music.clear();
	channelsAllocated = 0;
	soundsEnabled     = true;
}

AudioManager::AudioManager(int frequency, Uint16 format, int channels, int chunksize) {
	Mix_OpenAudio(frequency, format, channels, chunksize);
	Sounds.clear();
	Music.clear();
	channelsAllocated = 0;
	soundsEnabled     = true;
}

AudioManager::~AudioManager() {
    for(std::map<std::string, Sound*>::iterator iter = Sounds.begin(); iter != Sounds.end(); ++iter)
		delete (*iter).second;

	Sounds.clear();

	for(std::map<std::string, Mix_Music*>::iterator iter = Music.begin(); iter != Music.end(); ++iter)
		Mix_FreeMusic((*iter).second);

	Music.clear();
}

bool AudioManager::SetAudio(int frequency, Uint16 format, int channels, int chunksize) {
	if (Mix_OpenAudio(frequency, format, channels, chunksize) == 0)
		return true;

    return false;
}

bool AudioManager::LoadSound(const std::string& soundFile, const std::string& cueName) {
	if (soundFile.empty() || cueName.empty()) {
		std::cerr << "Error (AudioManager): Empty file name or cue name passed to LoadSound(...)" << std::endl;
		return false;
	}

	for (std::map<std::string, Sound*>::iterator siter = Sounds.begin(); siter != Sounds.end(); ++siter) {
		if (cueName == siter->first) {
			std::cerr << "Error (AudioManager): Sound cue name: " << cueName << " already taken (file: " << soundFile << ")" << std::endl;
			return false;
		}
	}

	Sound* sound = new Sound(soundFile);

	if(sound)
		Sounds[cueName] = sound;
	else {
		std::cerr << "Error (AudioManager): Error loading sound from file: " << soundFile << std::endl;
		return false;
	}

	return true;
}

bool AudioManager::LoadMusic(const std::string& musicFile, const std::string& cueName) {
	if (musicFile.empty() || cueName.empty()) {
		std::cerr << "Error (AudioManager): File name or music name passed to LoadMusic(...) was empty." << std::endl;
		return false;
	}

	for (std::map<std::string, Mix_Music*>::iterator miter = Music.begin(); miter != Music.end(); ++miter) {
		if (cueName == miter->first) {
			std::cerr << "Error (AudioManager): Music cue name: " << cueName << " already taken (file: " << musicFile << ")" << std::endl;
			return false;
		}
	}

	Music[cueName] = Mix_LoadMUS(musicFile.c_str());

	if (Music[cueName])
		return true;
	else {
		std::cerr << "Error (AudioManager): Error loading music from file: " << musicFile << std::endl;
		return false;
	}
}

bool AudioManager::LoadSoundsFromFile(const std::string& soundFile) {
	std::ifstream file(soundFile.c_str());

	if (file.is_open()) {
		std::string line, cueName, path = "";
		size_t pos = 0;

		while (!file.eof()) {
			getline(file, line);
			pos     = line.find_first_of(">");
			cueName = line.substr(0, pos);
			path    = line.substr(pos + 1);

			if (!path.empty()) {
				Sound* sound = new Sound(path);

                if (sound && !cueName.empty())
					Sounds[cueName] = sound;
				else
					std::cerr << "Error (AudioManager): Unable to load sound from file: " << path << std::endl;
			}
		}

		file.close();
		return true;
	}

    return false;
}

bool AudioManager::LoadMusicFromFile(const std::string& musicFile) {
	std::ifstream file(musicFile.c_str());

	if (file.is_open()) {
		std::string line, cueName, path = "";
		size_t pos = 0;

		while (!file.eof()) {
			getline(file, line);
			pos     = line.find_first_of(">");
			cueName = line.substr(0, pos);
			path    = line.substr(pos + 1);

			if (!cueName.empty() && !path.empty())
				Music[cueName] = Mix_LoadMUS(path.c_str());

			if (!(Music[cueName]))
				std::cerr << "Error (AudioManager): Unable to load music from file: " << path << std::endl;
		}

		file.close();
		return true;
	}

    return false;
}

bool AudioManager::HasAudioFile(const std::string& cueName) {
    if (cueName.empty())
        return false;

    if (Sounds.find(cueName) != Sounds.end() || Music.find(cueName) != Music.end())
        return true;

    return false;
}

void AudioManager::AllocateChannels(int n) {
	channelsAllocated = Mix_AllocateChannels(n);
}

int AudioManager::NoChannels() const {
	return channelsAllocated;
}

void AudioManager::PlaySound(const std::string& cueName, int loops) {
	if (!soundsEnabled && HasAudioFile(cueName))
		return;

	if (channelsAllocated == 0) {
		std::cerr << "Error (AudioManager): Cannot play sound with name " << cueName << ". No channels allocated" << std::endl;
		return;
	}

	Sound *sound = Sounds[cueName];

	if (sound->IsValid())
		Mix_PlayChannel(-1, sound->GetData(), loops); // Plays the requested sound on the first available channel
	else
		std::cerr << "Unable to play sound: " << cueName << std::endl;
}

void AudioManager::EnableSounds() {
	soundsEnabled = true;
}

void AudioManager::DisableSounds() {
	soundsEnabled = false;
}

// Set sound volume for all channels
void AudioManager::SetSoundVolume(int volume) {
	Mix_Volume(-1, volume);
}

void AudioManager::SetSoundVolumePercentage(float percentage) {
	if(percentage >= 0.f && percentage <= 1.f)
		Mix_Volume(-1, static_cast<float>(MIX_MAX_VOLUME) * percentage);
}

int AudioManager::GetSoundVolume() {
	return Mix_Volume(-1, -1);
}

int AudioManager::GetSpecificSoundVolume(const std::string& cueName) {
    if (HasAudioFile(cueName) && Sounds[cueName]->IsValid())
        return Mix_VolumeChunk(Sounds[cueName]->GetData(), -1);

    return -1;
}

void AudioManager::SetSpecificSoundVolume(const std::string& cueName, int volume) {
    if (HasAudioFile(cueName) && Sounds[cueName]->IsValid())
        Mix_VolumeChunk(Sounds[cueName]->GetData(), volume);
}

void AudioManager::SetSpecificSoundVolumePercentage(const std::string& cueName, float percentage) {
	if (HasAudioFile(cueName) && percentage >= 0.f && percentage <= 1.f) {
        if (!cueName.empty() || Sounds[cueName]->IsValid())
            Mix_VolumeChunk(Sounds[cueName]->GetData(), static_cast<float>(MIX_MAX_VOLUME) * percentage);
	}
}

bool AudioManager::IsSoundEnabled() const {
	return soundsEnabled;
}

void AudioManager::AllChannelsFadeOut(int milliseconds) {
	Mix_FadeOutChannel(-1, milliseconds);
}

void AudioManager::PlayMusic(const std::string& cueName, int loops) {
    if (!HasAudioFile(cueName) && loops >= 0)
        return;

    Mix_PlayMusic(Music[cueName], loops);
}

void AudioManager::PauseMusic() {
	Mix_PauseMusic();
}

void AudioManager::ResumeMusic() {
	Mix_ResumeMusic();
}

void AudioManager::StopMusic() {
	if (Mix_PlayingMusic() == 1)
		Mix_HaltMusic();
}

void AudioManager::FadeIn(const std::string& cueName, int loops, int milliseconds) {
	if (HasAudioFile(cueName) && milliseconds > 0 && loops >= 0) {
		if (Music[cueName]) {
			StopMusic();
			Mix_FadeInMusic(Music[cueName], loops, milliseconds);
		}
	}
}

void AudioManager::FadeOut(int milliseconds) {
	if (IsMusicPlaying() && milliseconds > 0)
		Mix_FadeOutMusic(milliseconds);
}

void AudioManager::SetMusicVolume(int volume) {
	Mix_VolumeMusic(volume);
}

void AudioManager::SetMusicVolumePercentage(float percentage) {
	if (percentage >= 0.f && percentage <= 1.f)
		Mix_VolumeMusic(static_cast<float>(MIX_MAX_VOLUME) * percentage);
}

int AudioManager::GetMusicVolume() {
	return Mix_VolumeMusic(-1);
}

bool AudioManager::IsMusicPlaying() {
    return Mix_PlayingMusic() == 1 ? true : false;
}

bool AudioManager::IsMusicPaused() {
    return Mix_PausedMusic() == 1 ? true : false;
}
