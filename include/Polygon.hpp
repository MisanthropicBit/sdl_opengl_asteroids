#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "Vector2.hpp"
#include "Color.hpp"
#include <vector>

class Polygon {
	public:
		Polygon();
		Polygon(float x, float y);
		~Polygon();

		virtual void Draw();
		void AddVertex(Vector2 vertex);
		void Rotate(float angle);
		void SetColor(Color& color);
		bool PointInPolygon(const Vector2& point);
		bool PointInPolygon(float x, float y);

		Vector2 GetCenter();
		float GetArea();
		float GetAngle() const;

		Vector2 position;
		std::vector<Vector2> vertices;

	protected:
		float angle;
		float area;
		Color color;

		float CalculateArea();
};

#endif
