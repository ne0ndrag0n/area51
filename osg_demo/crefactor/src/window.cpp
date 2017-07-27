#include "graphics/gui/drawables/window.hpp"
#include "graphics/gui/widget/node.hpp"
#include "containers/rect.hpp"
#include "containers/color.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Drawables {

        Window::Window( std::weak_ptr< Widget::Node > parent ) : parent( parent ) {}

        void Window::draw( DrawableContext* context ) {
          if( std::shared_ptr< Widget::Node > node = parent.lock() ) {
            Containers::Color< unsigned char > windowFillColor = node->getStyleValue< Containers::Color< unsigned char > >( "fill-color" );
            Containers::Color< unsigned char > titlebarColor = node->getStyleValue< Containers::Color< unsigned char > >( "primary-color" );
            Containers::Color< unsigned char > primaryTextColor = node->getStyleValue< Containers::Color< unsigned char > >( "default-text-color" );
            Containers::Rect< int > dimensions{
              node->getStyleValue< int >( "left" ),
              node->getStyleValue< int >( "top" ),
              ( int ) node->getStyleValue< double >( "width" ),
              ( int ) node->getStyleValue< double >( "height" )
            };
            std::string titlebarTitle = node->getAttributeValue< std::string >( "title" );

            nvgScissor( context, dimensions.x, dimensions.y, dimensions.width, dimensions.height );

            nvgBeginPath( context );
            nvgRect( context, dimensions.x, dimensions.y, dimensions.width, dimensions.height );
            nvgFillColor( context, toColor( windowFillColor ) );
            nvgFill( context );
            nvgClosePath( context );

            auto fill = nvgLinearGradient(
              context,
              dimensions.x / 2, dimensions.y + 60, dimensions.x / 2, dimensions.y + 70,
              nvgRGBA( 140, 140, 140, 255 ),
              nvgRGBA( 140, 140, 140, 0 )
            );
            nvgBeginPath( context );
            nvgRect( context, dimensions.x, dimensions.y, dimensions.width, 70 );
            nvgFillPaint( context, fill );
            nvgFill( context );
            nvgClosePath( context );

            nvgBeginPath( context );
            nvgRect( context, dimensions.x, dimensions.y, dimensions.width, 60 );
            nvgFillColor( context, toColor( titlebarColor ) );
            nvgFill( context );
            nvgClosePath( context );

            nvgBeginPath( context );
            nvgRect( context, dimensions.x, dimensions.y, dimensions.width, 20 );
            nvgFillColor( context, nvgRGBA( 0, 0, 0, 96 ) );
            nvgFill( context );
            nvgClosePath( context );

            nvgFontSize( context, 16.0f );
            nvgFontFace( context, "icon" );
            nvgFillColor( context, toColor( primaryTextColor ) );
            nvgTextAlign( context, NVG_ALIGN_LEFT );
            nvgText( context, dimensions.x + dimensions.width - 15, dimensions.y + 15, "\uf00d", NULL );

            nvgFontSize( context, 24.0f );
            nvgFontFace( context, node->getStyleValue< const char* >( "font" ) );
            nvgFillColor( context, toColor( primaryTextColor ) );
            nvgTextAlign( context, NVG_ALIGN_LEFT );
            nvgText( context, dimensions.x + 5, dimensions.y + 45, titlebarTitle.c_str(), NULL );

            nvgResetScissor( context );
          }
        }

        NVGcolor Window::toColor( const Containers::Color< unsigned char >& color ) {
          return nvgRGBA( color.r, color.g, color.b, color.a );
        }

      }
    }
  }
}
