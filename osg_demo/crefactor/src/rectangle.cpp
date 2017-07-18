#include "graphics/gui/drawables/rectangle.hpp"
#include <nanovg/nanovg_gl.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        void Rectangle::draw( DrawableContext* context ) {
          nvgBeginPath( context );
          nvgRect( context, 300, 300, 300, 30 );
          nvgFillColor( context, nvgRGBA(128, 96, 0, 255) );
          nvgFill( context );
          nvgClosePath( context );
        }

      }
    }
  }
}
