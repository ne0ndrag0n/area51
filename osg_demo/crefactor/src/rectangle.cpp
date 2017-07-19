#include "graphics/gui/drawables/rectangle.hpp"
#include <nanovg/nanovg_gl.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        Rectangle::Rectangle( Containers::Rect< unsigned int > dimensions ) : dimensions( dimensions ) {}

      }
    }
  }
}
