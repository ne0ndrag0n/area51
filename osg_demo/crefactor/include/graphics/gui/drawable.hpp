#ifndef BLUEBEAR_DRAWABLE
#define BLUEBEAR_DRAWABLE

#include "graphics/gui/types.hpp"
#include <nanovg/nanovg.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {

      /**
       * The Drawable interface is used by the Overlay to represent any type
       * that can be drawn using the "draw()" method.
       */
      struct Drawable {
        virtual ~Drawable() = default;

        unsigned int zOrder = 0;

        virtual void draw( DrawableContext* context ) = 0;
      };

    }
  }
}

#endif
