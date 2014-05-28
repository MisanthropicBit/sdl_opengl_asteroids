#include "SDL_opengl.h"
#include "Graphics.hpp"
#include "Math.hpp"
#include "Conversion.hpp"
#include <cmath>

SDL_Surface* Graphics::screen = NULL;

bool Graphics::SetVideoMode(int width, int height, int bitsPerPixel, Uint32 flags) {
	screen = SDL_SetVideoMode(width, height, bitsPerPixel, flags);

	if (screen)
		return true;

	return false;
}

void Graphics::SetClearColor(float r, float g, float b, float a) {
	glClearColor(Math::Clamp(r, 0.f, 1.f),
				 Math::Clamp(g, 0.f, 1.f),
				 Math::Clamp(b, 0.f, 1.f),
				 Math::Clamp(a, 0.f, 1.f));
}

void Graphics::SetClearColor(Color& clearColor) {
	glClearColor(Math::Clamp(clearColor.R, 0.f, 1.f),
				 Math::Clamp(clearColor.G, 0.f, 1.f),
				 Math::Clamp(clearColor.B, 0.f, 1.f),
				 Math::Clamp(clearColor.A, 0.f, 1.f));
}

void Graphics::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::SwapBuffers() {
	SDL_GL_SwapBuffers();
}

void Graphics::Set2dMode(int width, int height) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);		 // Set the matrix mode to projection
	glLoadIdentity();					 // Reset the matrix mode
	glOrtho(0, width, height, 0, 1, -1); // Set the orthogonal matrix to flatten the world (3D -> 2D)
	glMatrixMode(GL_MODELVIEW);          // Set the matrix mode to modelview
	glLoadIdentity();
}

void Graphics::BeginPoints() {
	glBegin(GL_POINTS);
}

void Graphics::BeginLines() {
	glBegin(GL_LINES);
}

void Graphics::BeginLineStrip() {
	glBegin(GL_LINE_STRIP);
}

void Graphics::BeginLineLoop() {
	glBegin(GL_LINE_LOOP);
}

void Graphics::BeginTriangles() {
	glBegin(GL_TRIANGLES);
}

void Graphics::BeginTriangleStrip() {
	glBegin(GL_TRIANGLE_STRIP);
}

void Graphics::BeginTriangleFan() {
	glBegin(GL_TRIANGLE_FAN);
}

void Graphics::BeginQuads() {
	glBegin(GL_QUADS);
}

void Graphics::BeginQuadStrip() {
	glBegin(GL_QUAD_STRIP);
}

void Graphics::BeginPolygons() {
	glBegin(GL_POLYGON);
}

void Graphics::BeginCircles() {
	glBegin(GL_LINE_STRIP);
}

void Graphics::End() {
	glEnd();
}

std::string Graphics::GetOpenGLContextVersion() {
	std::string version((char*)(glGetString(GL_VERSION)));

	return version.substr(0, version.find_first_of(' '));
}

std::string Graphics::GetSupportedExtensions() {
	std::string extensions((char*)(glGetString(GL_EXTENSIONS)));
	int position = extensions.find(' ');

	// Replace all spaces with the newline for printing convinience
	while (position != string::npos) {
		extensions.replace(position, 1, "\n");
		position = extensions.find(' ', position + 1);
	}

	return extensions;
}

bool Graphics::NPOT_TexturesSupported() {
	float version          = Conversion::String2Float(GetOpenGLContextVersion());
	std::string extensions = GetSupportedExtensions();

	return (version >= 2.0f || extensions.find("GL_ARB_texture_non_power_of_two") != std::string::npos);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, Color& color) {
	glLoadIdentity();

	glBegin(GL_LINE);
		glColor4f(color.R, color.G, color.B, color.A);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void Graphics::DrawRectangle(float x, float y, int w, int h, float angle, const Color& color) {
	glLoadIdentity();

	glBegin(GL_QUADS);
		glColor4f(color.R, color.G, color.B, color.A);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
	glEnd();
}

void Graphics::DrawTriangle(float x, float y, int height, int baseline, float angle, Color& color) {
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		glColor4f(color.R, color.G, color.B, color.A);
		glVertex2f(x, y - height/2);
		glVertex2f(x + baseline/2, y + height/2);
		glVertex2f(x - baseline/2, y + height/2);
	glEnd();
}

void Graphics::DrawCircle(float cx, float cy, float radius, const Color& color, float roundness) {
	glLoadIdentity();

	glBegin(GL_TRIANGLE_FAN);
		glColor4f(color.R, color.G, color.B, color.A);
		for (float i = 0; i < 360.f; i += roundness)
			glVertex2f(cx + radius * Math::Cos(i), cy + radius * Math::Sin(i));
	glEnd();
}
