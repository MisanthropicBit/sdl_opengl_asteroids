#include "Conversion.hpp"
#include <sstream>

int Conversion::String2Int(const std::string& str) {
	int i = 0;
	istringstream iss(str);
	iss >> i;

	return i;
}

std::string Conversion::Int2String(int i) {
	stringstream ss;
	ss << i;

	return ss.str();
}

float Conversion::String2Float(const std::string& str) {
	float f = 0.f;
	istringstream iss(str);
	iss >> f;

	return f;
}

std::string Conversion::Float2String(float f) {
	stringstream ss;
	ss << f;

	return ss.str();
}
