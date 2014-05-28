#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "SDL_image.h"
#include "SDL_opengl.h"
#include "Graphics.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include <string>

//======================================================
// The following class represents a 2D texture object.
//======================================================

class Texture2D {
	public:
		Texture2D();
		Texture2D(const std::string& imageFile);
		~Texture2D();

		bool Load(const std::string& imageFile);
		bool Load(const std::string& imageFile, const Color& colorKey);
		bool LoadPOT(const std::string& imagefile);
		bool LoadPOT(const std::string& imagefile, const Color& colorKey);
		void Release();
		void Draw(float x, float y);
		void DrawClipped(float x, float y, int sx, int sy, int sw, int sh);

		void Rotate(float angle);
		void Scale(float factor);
		void ScaleTo(float width, float height);
		void SetAlpha(float alpha);
		void SetTint(const Color& tint);
		void FlipOnX();
		void FlipOnY();
		void AdjustUVCoords(float u, float v);
		void AdjustUCoord(float u);
		void AdjustVCoord(float v);
		void ResetFlip();
		void Reset();
		int GetNearestPowerOfTwo(int i);

		GLuint ID() const;
		float GetAlpha()  const;
		float GetAngle()  const;
		float GetScaleX() const;
		float GetScaleY() const;
		float GetUCoord() const;
		float GetVCoord() const;

		float Width;
		float Height;

	private:
		GLuint textureID;
		Color tint;
		Rect<int> clipRect;
		float angle;
		float scaleX;
		float scaleY;
		bool  flippedOnX;
		bool  flippedOnY;
		float u;
		float v;
};

#endif
