#ifndef DRAWABLE_RECTANGLE
#define DRAWABLE_RECTANGLE

#include "graphics/gui/drawable.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        class Rectangle : public Drawable {
          virtual void draw( DrawableContext* context ) override;
        };

      }
    }
  }
}

#endif
