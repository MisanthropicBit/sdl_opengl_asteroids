#include "Math.hpp"
#include "Vector2.hpp"
#include <cmath>
#include <cstdlib>

// Constants
const float Math::Pi        = 3.14159265f;
const float Math::PiOver2   = Pi / 2.f;
const float Math::PiOver4   = Pi / 4.f;
const float Math::Degs2Rads = Pi / 180.f;
const float Math::Rads2Degs = 180.f / Pi;

// Functions
Vector2 Math::VectorLineIntersection(const Vector2& point, const Vector2& direction, const Vector2& linepoint, const Vector2& linedirection) {
	float det = direction.x * linedirection.y - linedirection.x * direction.y;

	if (det != 0) {
		float t = (-direction.y * (point.x - linepoint.x) + direction.x * (point.y - linepoint.y))/det;

		Vector2 vector;
		vector.x = linedirection.x;
		vector.y = linedirection.y;
		vector   = vector * t;
		vector   += linepoint;

		return vector;
	} else
		return Vector2(-1, -1);
}

int Math::PointSideOfLine(const Vector2 &point, const Vector2& linepoint1, const Vector2& linepoint2) {
	float result = (linepoint2.x - linepoint1.x) * (point.y - linepoint1.y) - (linepoint2.y - linepoint1.y) * (point.x - linepoint1.x);

	if (result > 0.f)
		return 1;
	else if (result < 0.f)
		return -1;

    return 0;
}

bool Math::PointInsideRectangle(Vector2& point, float left, float right, float top, float bottom) {
	if (point.x > left && point.x < right && point.y > top && point.y < bottom)
		return true;

	return false;
}

Vector2 Math::RotateVector(const Vector2& vector, float amount) {
	Vector2 newVector;

	newVector.x = vector.x * Math::Cos(amount) * Math::Rads2Degs - vector.y * Math::Sin(amount) * Math::Rads2Degs;
	newVector.y = vector.x + Math::Sin(amount) * Math::Rads2Degs * vector.y + Math::Cos(amount) * Math::Rads2Degs;

	return newVector;
}

int Math::NextPowerOfTwo(int i) {
	int value = 1;

	while (value < i)
		value <<= 1;

	return value;
}

int Math::Random(int min, int max) {
	return (rand() % (max - min + 1) + min);
}

float Math::Random(float min, float max) {
	return ((float)rand() / (float)(RAND_MAX)) * (max - min) + min;
	//return (max * (float)(rand() % RAND_MAX * 2 - RAND_MAX) / RAND_MAX);
	// ((float)rand() / static_cast<float>(RAND_MAX) + 1.0f)) * (max - min) + min
	// 0 / 32767 * (1.f - 0.6.f) + 0.6f = 0.6f
	// 32767 / 32767 * (1.f - 0.6f) + 0.6f =
}

double Math::Random(double min, double max) {
	return ((double)rand() / (double)(RAND_MAX)) * (max - min) + min;
	//return (max * (double)(rand() % RAND_MAX * 2 - RAND_MAX) / RAND_MAX);
}

float Math::Cos(float angle) {
	return cos(angle * Math::Degs2Rads);
}

float Math::Sin(float angle) {
	return sin(angle * Math::Degs2Rads);
}

double Math::Cos(double angle) {
	return cos(angle * Math::Degs2Rads);
}

double Math::Sin(double angle) {
	return sin(angle * Math::Degs2Rads);
}

int Math::Clamp(int value, int min, int max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;

	return value;
}

float Math::Clamp(float value, float min, float max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;

	return value;
}

double Math::Clamp(double value, double min, double max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;

	return value;
}

float Math::Round(float value) {
	// Symmetrical round
	return (value > 0.f ? floor(value + 0.5f) : ceil(value - 0.5f));
}

double Math::Round(double value) {
	// Symmetrical round
	return (value > 0.f ? floor(value + 0.5f) : ceil(value - 0.5f));
}

float Math::Lerp(float value1, float value2, float amount) {
	amount = Math::Clamp(amount, 0.f, 1.f);

	// Linear interpolation
	return (value1 + (value2 - value1) * amount);
}

double Math::Lerp(double value1, double value2, double amount) {
	amount = Math::Clamp(amount, 0.0, 1.0);

	// Linear interpolation
	return (value1 + (value2 - value1) * amount);
}
