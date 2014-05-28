#ifndef RECT_HPP
#define RECT_HPP

template<typename T>
class Rect {
	public:
		Rect();
		Rect(T x, T y, T width, T height);

		void Offset(T horizontalOffset, T verticalOffset);
		bool Contains(T x, T y);
		bool IntersectsWith(const Rect<T>& rect);

		void Set(T x, T y, T width, T height);
		void SetWidth(T width);
		void SetHeight(T height);

		T GetWidth() const;
		T GetHeight() const;
		T Right() const;
		T Bottom() const;

	public:
		T X;
		T Y;

	private:
		T Width;
		T Height;
};

template<typename T>
Rect<T>::Rect() {
	X      = 0;
	Y      = 0;
	Width  = 0;
	Height = 0;
}

template<typename T>
Rect<T>::Rect(T x, T y, T width, T height) {
	Set(x, y, width, height);
}

template<typename T>
void Rect<T>::Offset(T horizontalOffset, T verticalOffset) {
	X += horizontalOffset;
	Y += verticalOffset;
}

template<typename T>
bool Rect<T>::Contains(T x, T y) {
	if (x >= X && x <= Right() && y >= Y && y <= Bottom())
		return true;

	return false;
}

template<typename T>
bool Rect<T>::IntersectsWith(const Rect<T>& rect) {
	if (X >= rect.X && X <= rect.Right() &&
		Y >= rect.Y && Y <= rect.Bottom())
            return true;

	return false;
}

template<typename T>
void Rect<T>::Set(T x, T y, T width, T height) {
	X      = x;
	Y      = y;
	Width  = width;
	Height = height;
}

template<typename T>
void Rect<T>::SetWidth(T width) {
	Width = width;
}

template<typename T>
void Rect<T>::SetHeight(T height) {
	Height = height;
}

template<typename T>
T Rect<T>::GetWidth() const {
	return Width;
}

template<typename T>
T Rect<T>::GetHeight() const {
	return Height;
}

template<typename T>
T Rect<T>::Right() const {
	return X + Width;
}

template<typename T>
T Rect<T>::Bottom() const {
	return Y + Height;
}

#endif
