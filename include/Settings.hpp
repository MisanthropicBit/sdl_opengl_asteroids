#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

class Settings {
	public:
		Settings();

		bool ReadFile(const std::string& file);
		bool SaveToFile();
		bool SaveToFile(const std::string& file);

		void SetSound(bool flag);
		void SetNoInitialAsteroids(int value);
		bool IsSoundEnabled() const;
		int GetNoInitialAsteroids() const;

	private:
		std::string file;
		bool sound;
		int noInitialAsteroids;
};

#endif