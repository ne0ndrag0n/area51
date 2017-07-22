#include "graphics/gui/drawables/window.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        Window::Window(
          const std::string titlebarTitle,
          Containers::Rect< unsigned int > dimensions,
          Containers::Color< unsigned char > titlebarColor,
          Containers::Color< unsigned char > windowFillColor
        ) :
          Rectangle::Rectangle( dimensions ),
          titlebarTitle( titlebarTitle ), titlebarColor( titlebarColor ), windowFillColor( windowFillColor )  {}

        void Window::draw( DrawableContext* context ) {
          nvgBeginPath( context );
          nvgRect( context, dimensions.x, dimensions.y, dimensions.width, dimensions.height );
          nvgFillColor( context, toColor( windowFillColor ) );
          nvgFill( context );
          nvgClosePath( context );

          nvgBeginPath( context );
          nvgRect( context, dimensions.x, dimensions.y, dimensions.width, dimensions.y + 50 );
          nvgFillColor( context, toColor( titlebarColor ) );
          nvgFill( context );
          nvgClosePath( context );

          nvgBeginPath( context );
          nvgRect( context, dimensions.x, dimensions.y, dimensions.width, dimensions.y + 10 );
          nvgFillColor( context, nvgRGBA( 0, 0, 0, 128 ) );
          nvgFill( context );
          nvgClosePath( context );

          nvgFontSize( context, 24.0f );
          nvgFontFace( context, "default" );
          nvgFillColor( context, nvgRGBA( 255, 255, 255, 255 ) );
          nvgTextAlign( context, NVG_ALIGN_LEFT );
          nvgText( context, dimensions.x + 5, dimensions.y + 45, titlebarTitle.c_str(), NULL );
        }

        NVGcolor Window::toColor( const Containers::Color< unsigned char >& color ) {
          return nvgRGBA( color.r, color.g, color.b, color.a );
        }

      }
    }
  }
}
