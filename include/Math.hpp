#ifndef MATH_HPP
#define MATH_HPP

//////////////////////////////////////////////////////////////////////////////////
// The math class defines some useful and common constants and functions.
//////////////////////////////////////////////////////////////////////////////////

class Vector2;

class Math {
	public:
		// 2D Vector math
		static Vector2 VectorLineIntersection(const Vector2& point, const Vector2& direction, const Vector2& linepoint1, const Vector2& linepoint2);
		static int PointSideOfLine(const Vector2& point, const Vector2& linepoint1, const Vector2& linepoint2);
		static bool PointInsideRectangle(Vector2& point, float left, float right, float top, float bottom);
		static Vector2 RotateVector(const Vector2& vector, float amount);

		// Primarily to be used for OpenGL 2D POT (power-of-two) texture loading
		static int NextPowerOfTwo(int i);

		// Random value generators
		static int Random(int min, int max);
		static float Random(float min, float max);
		static double Random(double min, double max);

		static float Cos(float angle);
		static float Sin(float angle);
		static double Cos(double angle);
		static double Sin(double angle);

		static int Clamp(int value, int min, int max);
		static float Clamp(float value, float min, float max);
		static double Clamp(double value, double min, double max);

		static float Round(float value);
		static double Round(double value);

		static float Lerp(float value1, float value2, float amount);
		static double Lerp(double value1, double value2, double amount);

	public:
		// Constants
		static const float Pi;
		static const float PiOver2;
		static const float PiOver4;
		static const float Degs2Rads;
		static const float Rads2Degs;
};

#endif
