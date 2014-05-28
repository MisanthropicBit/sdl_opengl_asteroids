#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "Sound.hpp"
#include <fstream>
#include <map>

class AudioManager {
	public:
		AudioManager();
		AudioManager(int frequency, Uint16 format, int channels, int chunksize);
		~AudioManager();

		bool SetAudio(int frequency, Uint16 format, int channels, int chunksize);
		bool LoadSound(const std::string& soundFile, const std::string& cueName);
		bool LoadMusic(const std::string& musicFile, const std::string& cueName);
		bool LoadSoundsFromFile(const std::string& soundFile);
		bool LoadMusicFromFile(const std::string& musicFile);

		bool HasAudioFile(const std::string& cueName);

		// Sound
		void AllocateChannels(int n);
		int  NoChannels() const;
		void PlaySound(const std::string& cueName, int loops);
		void EnableSounds();
		void DisableSounds();
		void SetSoundVolume(int volume);
		void SetSoundVolumePercentage(float percentage);
		int  GetSoundVolume();
		int  GetSpecificSoundVolume(const std::string& cueName);
		void SetSpecificSoundVolume(const std::string& cueName, int volume);
		void SetSpecificSoundVolumePercentage(const std::string& cueName, float percentage);
		bool IsSoundEnabled() const;
		void AllChannelsFadeOut(int milliseconds);

		// Music
		void PlayMusic(const std::string& cueName, int loops);
		void PauseMusic();
		void ResumeMusic();
		void StopMusic();
		void FadeIn(const std::string& cueName, int loops, int milliseconds);
		void FadeOut(int milliseconds);
		void SetMusicVolume(int volume);
		void SetMusicVolumePercentage(float percentage);
		int  GetMusicVolume();
		bool IsMusicPlaying();
		bool IsMusicPaused();

	private:
		std::map<std::string, Sound*> Sounds;
		std::map<std::string, Mix_Music*> Music;
		int channelsAllocated;
		bool soundsEnabled;
};

#endif
