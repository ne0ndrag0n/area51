#include "graphics/gui/overlay.hpp"
#include "device/display.hpp"
#include <SFML/OpenGL.hpp>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>
#include <osg/StateAttribute>
#include <osg/StateSet>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/Geode>
#include <algorithm>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {

      Overlay::Overlay() {
        nvgContext = nvgCreateGL3( NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
        if( !nvgContext ) {
          std::cout << "NanoVG context could not be created." << std::endl;
          exit( 1 );
        }

        loadFonts( nvgContext );
      }

      Overlay::~Overlay() {
        nvgDeleteGL3( nvgContext );
      }

      void Overlay::loadFonts( NVGcontext* context ) {
        // TODO: Load fonts from a registry when object is integrated into Concordia
        if( nvgCreateFont( context, "default", "roboto.ttf" ) == -1 ) {
          std::cout << "Error loading roboto.ttf" << std::endl;
        }

        if( nvgCreateFont( context, "icon", "fontawesome.ttf" ) == -1 ) {
          std::cout << "Error loading fontawesome.ttf" << std::endl;
        }
      }

      void Overlay::drawUnits( NVGcontext* context ) {
        // Sort units by zOrder
        std::stable_sort( drawableUnits.begin(), drawableUnits.end(), []( const std::shared_ptr< Drawable > lhs, const std::shared_ptr< Drawable > rhs ) {
          return lhs->zOrder < rhs->zOrder;
        } );

        for( std::shared_ptr< Drawable > drawable : drawableUnits ) {
          drawable->draw( context );
        }
      }

      void Overlay::addDrawable( std::shared_ptr< Drawable > drawable ) {
        drawableUnits.emplace_back( drawable );
      }

      void Overlay::removeDrawable( std::shared_ptr< Drawable > drawable ) {
        drawableUnits.erase( std::remove( drawableUnits.begin(), drawableUnits.end(), drawable ), drawableUnits.end() );
      }

      void Overlay::clear() {
        drawableUnits.clear();
      }

      void Overlay::update() {
        drawUnits( nvgContext );
      }

    }
  }
}
