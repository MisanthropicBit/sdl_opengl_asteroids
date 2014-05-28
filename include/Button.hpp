#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Texture2D.hpp"
#include "Rect.hpp"
#include "Vector2.hpp"
#include <string>

class Button {
	public:
		Button();
		~Button();

		void Load(int x, int y, const std::string& imageFile, float minScale, float maxScale, const std::string& buttonSound);
		void Update(float dt);
		void Draw();
		void DrawClipped(int sx, int sy, int sw, int sh);

		void SetPosition(int x, int y);
		void SetClipRect(const Rect<int>& rect);
		void SetClipRect(int sx, int sy, int sw, int sh);
		void Reset();

		int GetX() const;
		int GetY() const;
		int GetWidth() const;
		int GetHeight() const;
		float GetScale() const;
		float GetMinScale() const;
		float GetMaxScale() const;
		Rect<int> GetClipRect() const;

		bool HasHover() const;
		void OnMouseMove(int x, int y);

	protected:
		int x;
		int y;
		Texture2D image;
		Rect<int> clipRect;
		float scale;
		Vector2 scaleDimensions;
		std::string buttonSound;
		bool hasHover;
};

#endif
