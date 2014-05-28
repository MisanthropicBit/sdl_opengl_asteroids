#include "Color.hpp"
#include "Math.hpp"

//=========================================================================================================================

//////////////////////////////////////////////////////////////////////////////////
// Colors in OpenGL are represented by floating numbers clamped between 0.f
// and 1.f. The static colors here are defined using their RGB equivalents.
//////////////////////////////////////////////////////////////////////////////////

// Grayscale
const Color Color::Black(0.f, 0.f, 0.f);
const Color Color::Gray(190.f / 255.f, 190.f / 255.f, 190.f / 255.f);
const Color Color::White(1.f, 1.f, 1.f);

// Standard colors
const Color Color::Red(1.f, 0.f, 0.f);
const Color Color::Green(0.f, 1.f, 0.f);
const Color Color::Blue(0.f, 0.f, 1.f);
const Color Color::Yellow(1.f, 1.f, 0.f);
const Color Color::Purple(160.f / 255.f, 32.f / 255.f, 240.f / 255.f);
const Color Color::Orange(1.f, 165.f / 255.f, 0.f);
const Color Color::Magenta(1.f, 0.f, 1.f);
const Color Color::Cyan(0.f, 1.f, 1.f);
const Color Color::Gold(1.f, 215.f / 255.f, 0.f);
const Color Color::Silver(230.f / 255.f, 232.f / 255.f, 250.f / 255.f);
const Color Color::Bronze(140.f / 255.f, 120.f / 255.f, 83.f / 255.f);

// Dark colors
const Color Color::DarkRed(139.f / 255.f, 0.f, 0.f);
const Color Color::DarkGreen(0.f, 100.f / 255.f, 0.f);
const Color Color::DarkBlue(0.f, 0.f, 205.f / 255.f);
const Color Color::DarkCyan(64.f / 255.f, 224.f / 255.f, 208.f / 255.f);

// Light colors
const Color Color::Pink(1.f, 105.f / 255.f, 180.f / 255.f);
const Color Color::LightGreen(124.f / 255.f, 252.f / 255.f, 0.f);
const Color Color::LightBlue(173.f / 255.f, 216.f / 255.f, 230.f / 255.f);

//=========================================================================================================================

Color::Color(float r, float g, float b, float a) {
	SetRGBA(r, g, b, a);
}

//=========================================================================================================================

void Color::SetRGB(float r, float g, float b) {
	R = Math::Clamp(r, 0.f, 1.f);
	G = Math::Clamp(g, 0.f, 1.f);
	B = Math::Clamp(b, 0.f, 1.f);
}

//=========================================================================================================================

void Color::SetRGBA(float r, float g, float b, float a) {
	R = Math::Clamp(r, 0.f, 1.f);
	G = Math::Clamp(g, 0.f, 1.f);
	B = Math::Clamp(b, 0.f, 1.f);
	A = Math::Clamp(a, 0.f, 1.f);
}

//=========================================================================================================================

void Color::ConvertToGrayscale() {
	R *= 0.3f;
	G *= 0.59f;
	B *= 0.11f;
}

//=========================================================================================================================

Color& Color::operator= (const Color& color) {
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;

	return (*this);
}

//=========================================================================================================================

Color Color::operator+ (const Color& color) const {
	Color result;

	result.R = Math::Clamp(R + color.R, 0.f, 1.f);
	result.G = Math::Clamp(G + color.G, 0.f, 1.f);
	result.B = Math::Clamp(B + color.B, 0.f, 1.f);

	return result;
}

//=========================================================================================================================

Color Color::operator- (const Color& color) const {
	Color result;

	result.R = Math::Clamp(R - color.R, 0.f, 1.f);
	result.G = Math::Clamp(G - color.G, 0.f, 1.f);
	result.B = Math::Clamp(B - color.B, 0.f, 1.f);

	return result;
}

//=========================================================================================================================

Color Color::operator+= (const Color& color) {
	R += Math::Clamp(color.R, 0.f, 1.f);
	G += Math::Clamp(color.G, 0.f, 1.f);
	B += Math::Clamp(color.B, 0.f, 1.f);

	return (*this);
}

//=========================================================================================================================

Color Color::operator-= (const Color& color) {
	R -= Math::Clamp(color.R, 0.f, 1.f);
	G -= Math::Clamp(color.G, 0.f, 1.f);
	B -= Math::Clamp(color.B, 0.f, 1.f);

	return (*this);
}

//=========================================================================================================================

bool Color::operator== (const Color& color) const {
	return (R == color.R && G == color.G && B == color.B);
}

//=========================================================================================================================

bool Color::operator!= (const Color& color) const {
	return (R != color.R || G != color.G || B != color.B);
}

//=========================================================================================================================
