#ifndef WINDOW_DRAWABLE
#define WINDOW_DRAWABLE

#include "graphics/gui/drawables/rectangle.hpp"
#include "containers/rect.hpp"
#include "containers/color.hpp"
#include <nanovg/nanovg.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        class Window : public Rectangle {
          std::string titlebarTitle;
          Containers::Color< unsigned char > titlebarColor;
          Containers::Color< unsigned char > windowFillColor;

          NVGcolor toColor( const Containers::Color< unsigned char >& color );

        public:
          Window(
            const std::string titlebarTitle,
            Containers::Rect< unsigned int > dimensions,
            Containers::Color< unsigned char > titlebarColor,
            Containers::Color< unsigned char > windowFillColor
          );
          virtual void draw( DrawableContext* context ) override;
        };

      }
    }
  }
}


#endif
