#include "Polygon.hpp"
#include "Graphics.hpp"
#include "SDL_opengl.h"

#include <iostream>

Polygon::Polygon() {
	position.Zero();
	vertices.clear();
	angle = 0.f;
	area  = 0.f;
	color = Color::White;
}

Polygon::Polygon(float x, float y) {
	position.x = x;
	position.y = y;
	vertices.clear();
	angle = 0.f;
	area  = 0.f;
	color = Color::White;
}

Polygon::~Polygon() {
	vertices.clear();
}

void Polygon::Draw() {
	if (vertices.size() < 3)
		return;

	glLoadIdentity();
	glTranslatef(position.x, position.y, 0);
	glRotatef(-angle, 0, 0, 1.f);
	glTranslatef(-position.x, -position.y, 0);

	//////////////////////////////////////////////////////////////////////////////////
	// We use BeginTriangleFan() here instead of BeginPolygons() (uses GL_POLYGONS)
	// because the latter only supports concave polygons, and in some instances of
	// the game, we might encounter convex polygons. Additionally, modern graphics
	// hardware is optimized to draw triangles.
	//////////////////////////////////////////////////////////////////////////////////
	Graphics::BeginTriangleFan();
		glColor4f(color.R, color.G, color.B, color.A);

        for (int i = 0; i < vertices.size(); ++i)
            glVertex2f(vertices[i].x, vertices[i].y);
	Graphics::End();
}

void Polygon::AddVertex(Vector2 vertex) {
	vertices.push_back(vertex);

	if (vertices.size() > 2) {
		area     = CalculateArea();
		position = GetCenter();
	}
}

void Polygon::Rotate(float angle) {
	if (angle < 0.f)
		angle += 360.f;
	else if (angle > 360.f)
		angle -= 360.f;

	this->angle = angle;
}

void Polygon::SetColor(Color& color) {
	this->color = color;
}

bool Polygon::PointInPolygon(const Vector2& point) {
	return PointInPolygon(point.x, point.y);
}

bool Polygon::PointInPolygon(float x, float y) {
	if (vertices.size() < 3)
		return false;

	bool inside = false;
	int ni      = 0;

	for (int i = 0; i < vertices.size(); ++i) {
		ni = (i + 1 == vertices.size() ? 0 : i + 1);

		if (x >= vertices[i].x && x < vertices[ni].x
			|| x >= vertices[ni].x && x < vertices[i].x) {
			if (y <= ((vertices[ni].y - vertices[i].y) / (vertices[ni].x - vertices[i].x) * (x - vertices[i].x) + vertices[i].y))
				inside = !inside;
		}
	}

	return inside;
}

Vector2 Polygon::GetCenter() {
	Vector2 vector;
	float accx = 0.f;
	float accy = 0.f;
	int ni     = 0;

	for (int i = 0; i < vertices.size(); ++i) {
		ni   = (i + 1 == vertices.size() ? 0 : i + 1);
		accx += (vertices[i].x + vertices[ni].x) * (vertices[i].x * vertices[ni].y - vertices[ni].x * vertices[i].y);
	}

	for (int i = 0; i < vertices.size(); ++i) {
		ni   = (i + 1 == vertices.size() ? 0 : i + 1);
		accy += (vertices[i].y + vertices[ni].y) * (vertices[i].x * vertices[ni].y - vertices[ni].x * vertices[i].y);
	}

	vector.x = 1.f/(6.f * area) * accx;
	vector.y = 1.f/(6.f * area) * accy;

	return vector;
}

float Polygon::GetArea() {
	return area;
}

float Polygon::CalculateArea() {
	float acc = 0.f;
	int ni    = 0;

	for (int i = 0; i < vertices.size(); ++i) {
		ni  = (i + 1 == vertices.size() ? 0 : i + 1);
		acc += (vertices[i].x * vertices[ni].y - vertices[ni].x * vertices[i].y);
	}

	return abs(0.5f * acc);
}

float Polygon::GetAngle() const {
	return angle;
}
