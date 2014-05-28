#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "Math.hpp"

class Vector2 {
	public:
		Vector2();
		Vector2(float x, float y);

		void Rotate(float angle);
		void Zero();
		void Normalize();

		float GetAngle() const;
		float Length() const;
		bool IsZero();
		float DotProduct(const Vector2& v);
		static float DotProduct(const Vector2& v1, const Vector2& v2);
		
		// Operators
		void operator= (const Vector2& v);

		Vector2 operator+ (const Vector2* v);

		Vector2 operator+ (const Vector2& v);
		Vector2 operator- (const Vector2& v);
		Vector2 operator* (const Vector2& v);
		Vector2 operator/ (const Vector2& v);

		void operator+= (const Vector2& v);
		void operator-= (const Vector2& v);
		void operator*= (const Vector2& v);
		void operator/= (const Vector2& v);

		Vector2 operator+ (float f);
		Vector2 operator- (float f);
		Vector2 operator* (float f);
		Vector2 operator/ (float f);

		void operator+= (float f);
		void operator-= (float f);
		void operator*= (float f);
		void operator/= (float f);

		bool operator== (const Vector2& v);
		bool operator!= (const Vector2& v);

	public:
		float x;
		float y;
};

#endif