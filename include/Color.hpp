#ifndef COLOR_HPP
#define COLOR_HPP

class Color {
	public:
		Color(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f);
		
		void SetRGB(float r, float g, float b);
		void SetRGBA(float r, float g, float b, float a);
		void ConvertToGrayscale();
		
		///////////////
		// Operators //
		///////////////
		Color& operator= (const Color&);

		Color operator+ (const Color&) const;
		Color operator- (const Color&) const;
		Color operator+= (const Color&);
		Color operator-= (const Color&);
	
		bool operator== (const Color&) const;
		bool operator!= (const Color&) const;
		
		////////////////////////////
		// Constant static colors //
		////////////////////////////
		// Grayscale
		static const Color Black;
		static const Color Gray;
		static const Color White;

		// Standard colors
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Purple;
		static const Color Orange;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Gold;
		static const Color Silver;
		static const Color Bronze;

		// Dark colors
		static const Color DarkRed;
		static const Color DarkGreen;
		static const Color DarkBlue;
		static const Color DarkCyan;

		// Light colors
		static const Color Pink; // aka LightRed
		static const Color LightGreen;
		static const Color LightBlue;

	public:
		float R;
		float G;
		float B;
		float A;
};

#endif