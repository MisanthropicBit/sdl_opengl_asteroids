#include "Button.hpp"
#include "Game.hpp"

Button::Button() {
	x = 0;
	y = 0;
	scaleDimensions.Zero();
	scale    = 0.f;
	hasHover = false;
}

Button::~Button() {
    image.Release();
}

void Button::Load(int x, int y, const std::string& imageFile, float minScale, float maxScale, const std::string& buttonSound) {
	this->x = x;
	this->y = y;
	image.Load(imageFile);
	clipRect.Set(x, y, image.Width, image.Height);
	scaleDimensions.x = minScale;
	scaleDimensions.y = maxScale;
	this->buttonSound = buttonSound;
}

void Button::Update(float dt) {
	float pdt = (float)dt / 1000.f;

	if (HasHover()) {
		if (scale != scaleDimensions.y) {
			scale += pdt;

			if (scale >= scaleDimensions.y)
				scale = scaleDimensions.y;

			image.Scale(scale);
		}
	} else {
		if (scale != scaleDimensions.x) {
			scale -= pdt;

			if (scale <= scaleDimensions.x)
				scale = scaleDimensions.x;

			image.Scale(scale);
		}
	}
}

void Button::Draw() {
	image.Draw(x, y);
}

void Button::DrawClipped(int sx, int sy, int sw, int sh) {
	image.DrawClipped(x, y, sx, sy, sw, sh);
}

void Button::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

void Button::SetClipRect(int sx, int sy, int sw, int sh) {
	clipRect.X = sx;
	clipRect.Y = sy;
	clipRect.SetWidth(sw);
	clipRect.SetHeight(sh);
}

void Button::SetClipRect(const Rect<int>& rect) {
	SetClipRect(rect.X, rect.Y, rect.GetWidth(), rect.GetHeight());
}

void Button::Reset() {
	scale    = scaleDimensions.x;
	hasHover = false;
    image.SetTint(Color::White);
    image.Scale(scale);
}

int Button::GetX() const {
	return x;
}

int Button::GetY() const {
	return y;
}

int Button::GetWidth() const {
	return image.Width;
}

int Button::GetHeight() const {
	return image.Height;
}

float Button::GetScale() const {
    return scale;
}

float Button::GetMinScale() const {
    return scaleDimensions.x;
}

float Button::GetMaxScale() const {
    return scaleDimensions.y;
}

Rect<int> Button::GetClipRect() const {
	return clipRect;
}

bool Button::HasHover() const {
	return hasHover;
}

void Button::OnMouseMove(int x, int y) {
	if (x >= this->x && x <= this->x + clipRect.GetWidth() &&
		y >= this->y && y <= this->y + clipRect.GetHeight()) {

		if (!HasHover())
			Game::audioManager.PlaySound(buttonSound, 0);

		hasHover = true;
		image.SetTint(Color::Red);
	} else {
		hasHover = false;
		image.SetTint(Color::White);
	}
}
