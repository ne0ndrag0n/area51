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
            Containers::Color< unsigned char > windowFillColor = node->getStyle().getValue< Containers::Color< unsigned char > >( "fill-color" );
            Containers::Color< unsigned char > titlebarColor = node->getStyle().getValue< Containers::Color< unsigned char > >( "primary-color" );
            Containers::Color< unsigned char > primaryTextColor = node->getStyle().getValue< Containers::Color< unsigned char > >( "default-text-color" );
            Containers::Rect< int > dimensions{
              node->getStyle().getValue< int >( "left" ),
              node->getStyle().getValue< int >( "top" ),
              ( int ) node->getStyle().getValue< double >( "width" ),
              ( int ) node->getStyle().getValue< double >( "height" )
            };
            std::string titlebarTitle = node->getAttributeValue< std::string >( "title" );

            auto gradFill = nvgBoxGradient(
              context,
              dimensions.x, dimensions.y, dimensions.width + 3, dimensions.height + 3,
              10.0f,
              10,
              nvgRGBA( 0, 0, 0, 64 ),
              nvgRGBA( 0, 0, 0, 0 )
            );
            nvgBeginPath( context );
            nvgRect( context, dimensions.x - 10 , dimensions.y - 10, dimensions.width + 20, dimensions.height + 30 );
            nvgRoundedRect( context, dimensions.x, dimensions.y, dimensions.width, dimensions.height, 5.0f );
            nvgPathWinding( context, NVG_HOLE );
            nvgFillPaint( context, gradFill );
            nvgFill( context );
            nvgClosePath( context );

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
            nvgFontFace( context, node->getStyle().getValue< const char* >( "font" ) );
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
