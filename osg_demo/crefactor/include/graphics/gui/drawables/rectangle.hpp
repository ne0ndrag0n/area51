#ifndef DRAWABLE_RECTANGLE
#define DRAWABLE_RECTANGLE

#include "graphics/gui/drawable.hpp"
#include "containers/rect.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        class Rectangle : public Drawable {
        protected:
          Containers::Rect< unsigned int > dimensions;
        public:
          Rectangle( Containers::Rect< unsigned int > dimensions );
        };

      }
    }
  }
}

#endif
