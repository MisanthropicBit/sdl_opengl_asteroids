#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <string>

class Conversion {
	public:
		// Int <--> String
		static int String2Int(const std::string& str);
		static std::string Int2String(int i);

		// Float <--> String
		static float String2Float(const std::string& str);
		static std::string Float2String(float f);
};

#endif
