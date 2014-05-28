#include "Settings.hpp"
#include "Conversion.hpp"
#include <iostream>
#include <fstream>

Settings::Settings() {
	file = "";
	sound = true;
	noInitialAsteroids = 1;
}

bool Settings::ReadFile(const std::string& file) {
	if (file.empty())
		return false;

	this->file = file;
	std::ifstream fileStream(file.c_str(), std::ios_base::in | std::ios_base::binary);

	fileStream.seekg(0, std::ios::end);
	int size = fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);

	if (size == 0) {
	    std::cerr << "Attempt to read \"" << file << "\": File is empty" << std::endl;
	    return false;
	}

    int soundEnabled, noAsteroids = 0;

	if (fileStream.is_open()) {
	    fileStream.read(reinterpret_cast<char*>(&soundEnabled), sizeof(soundEnabled));
	    fileStream.read(reinterpret_cast<char*>(&noAsteroids), sizeof(noAsteroids));

        sound = ((int)(soundEnabled) == 1 ? true : false);
        noInitialAsteroids = (int)noAsteroids;

        fileStream.close();

        return true;
	}

	return false;
}

bool Settings::SaveToFile() {
	return SaveToFile(this->file);
}

bool Settings::SaveToFile(const std::string &file) {
	if (file.empty())
		return false;

	std::ofstream fileStream(file.c_str(), std::ios_base::trunc | std::ios_base::binary);

	if (fileStream.is_open()) {
	    int soundEnabled = (IsSoundEnabled() ? 1 : 0);
	    int noAs = GetNoInitialAsteroids();
	    fileStream.write(reinterpret_cast<const char*>(&soundEnabled), sizeof(soundEnabled));
	    fileStream.write(reinterpret_cast<const char*>(&noAs), sizeof(noAs));

	    fileStream.close();

		return true;
	}

	return false;
}

void Settings::SetSound(bool flag) {
	sound = flag;
}

void Settings::SetNoInitialAsteroids(int value) {
	if (value > 0 && value <= 12)
		noInitialAsteroids = value;
}

bool Settings::IsSoundEnabled() const {
	return sound;
}

int Settings::GetNoInitialAsteroids() const {
	return noInitialAsteroids;
}
