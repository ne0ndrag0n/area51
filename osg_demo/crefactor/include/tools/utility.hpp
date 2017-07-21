#ifndef UTILITY
#define UTILITY

#include <vector>
#include <string>

namespace BlueBear {
	namespace Tools {
		class Utility {

			public:
				static std::vector<std::string> split(const std::string &text, char sep);
		};
	}
}


#endif
