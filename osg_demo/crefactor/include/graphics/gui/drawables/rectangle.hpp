#ifndef DRAWABLE_RECTANGLE
#define DRAWABLE_RECTANGLE

#include "graphics/gui/drawable.hpp"
#include "containers/rect.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        struct Rectangle : public Drawable {
          Containers::Rect< unsigned int > dimensions;

          Rectangle( Containers::Rect< unsigned int > dimensions );
        };

      }
    }
  }
}

#endif
