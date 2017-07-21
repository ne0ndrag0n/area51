#include "tools/utility.hpp"

namespace BlueBear {
  namespace Tools {

    /**
     * Tokenise a std::string based on a char value
     */
    std::vector<std::string> Utility::split(const std::string &text, char sep) {
      std::vector<std::string> tokens;
      std::size_t start = 0, end = 0;
      while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
      }
      tokens.push_back(text.substr(start));
      return tokens;
    }

  }
}
