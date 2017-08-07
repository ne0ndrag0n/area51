#include "graphics/gui/widget/widgetengine_adapter.hpp"
#include "device/display.hpp"
#include <osg/GLDefines>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg/nanovg_gl.h>
#include <osg/GraphicsContext>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        WidgetEngine::Adapter::Adapter( WidgetEngine& parent ) : parent( parent ) {
          setSupportsDisplayList( false );
        }

        void WidgetEngine::Adapter::drawImplementation( osg::RenderInfo& renderInfo ) const {
          unsigned int contextID = renderInfo.getContextID();

          if( !nvgContext ) {
            // No WidgetEngine::Adapter should ever be created as a const object.
            // And OpenSceneGraph shouldn't have made drawImplementation() a const method.
            WidgetEngine::Adapter* self = const_cast< WidgetEngine::Adapter* >( this );
            self->nvgContext = nvgCreateGL3( NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
            if( !self->nvgContext ) {
              std::cout << "NanoVG context could not be created." << std::endl;
              exit( 1 );
            }

            self->loadFonts();
            self->activeContextID = contextID;
          } else if ( nvgContext && contextID == activeContextID ) {
            osg::ref_ptr< osg::State > state = renderInfo.getState();
            state->disableAllVertexArrays();
            state->disableTexCoordPointer( 0 );

            nvgBeginFrame( nvgContext, parent.display.getWidth(), parent.display.getHeight(), 1.0f );
            parent.drawUnits( nvgContext );
            nvgEndFrame( nvgContext );
          }
        }

        void WidgetEngine::Adapter::releaseGLObjects( osg::State* state ) const {
          if( state ) {
            if( osg::ref_ptr< osg::GraphicsContext > context = state->getGraphicsContext() ) {
              if( context->makeCurrent() ) {
                WidgetEngine::Adapter* self = const_cast< WidgetEngine::Adapter* >( this );

                if( self->nvgContext ) {
                  nvgDeleteGL3( self->nvgContext );
                  self->nvgContext = nullptr;
                }

                context->releaseContext();
              }
            }
          }
        }

        void WidgetEngine::Adapter::loadFonts() {
          // TODO: Load fonts from a registry when object is integrated into Concordia
          if( nvgCreateFont( nvgContext, "default", "roboto.ttf" ) == -1 ) {
            std::cout << "Error loading roboto.ttf" << std::endl;
          }

          if( nvgCreateFont( nvgContext, "icon", "fontawesome.ttf" ) == -1 ) {
            std::cout << "Error loading fontawesome.ttf" << std::endl;
          }
        }

      }
    }
  }
}
