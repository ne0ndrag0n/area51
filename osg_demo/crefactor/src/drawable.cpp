#include "graphics/gui/drawable.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {

      bool Drawable::operator<( const Drawable& other ) {
        return this->zOrder < other.zOrder;
      }

    }
  }
}
