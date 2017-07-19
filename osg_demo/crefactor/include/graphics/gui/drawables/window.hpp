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
          Containers::Color< unsigned int > titlebarColor;
          Containers::Color< unsigned int > windowFillColor;

          NVGcolor toColor( const Containers::Color< unsigned int >& color );

        public:
          Window(
            const std::string titlebarTitle,
            Containers::Rect< unsigned int > dimensions,
            Containers::Color< unsigned int > titlebarColor,
            Containers::Color< unsigned int > windowFillColor
          );
          virtual void draw( DrawableContext* context ) override;
        };

      }
    }
  }
}


#endif
