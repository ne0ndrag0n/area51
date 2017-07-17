#include "graphics/gui/overlay.hpp"
#include "device/display.hpp"
#include <osg/GLDefines>
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

      Overlay::InternalAdapter::InternalAdapter( Overlay& parent ) : parent( parent ) {
        setSupportsDisplayList( false );
      }

      void Overlay::InternalAdapter::drawImplementation( osg::RenderInfo& renderInfo ) const {
        unsigned int contextID = renderInfo.getContextID();

        if( !nvgContext ) {
          // No Overlay::InternalAdapter should ever be created as a const object.
          // And OpenSceneGraph shouldn't have made drawImplementation() a const method.
          Overlay::InternalAdapter* self = const_cast< Overlay::InternalAdapter* >( this );
          self->nvgContext = nvgCreateGL3( NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
          if( !self->nvgContext ) {
            std::cout << "NanoVG context could not be created." << std::endl;
            exit( 1 );
          }

          self->activeContextID = contextID;
        } else if ( nvgContext && contextID == activeContextID ) {
          osg::ref_ptr< osg::State > state = renderInfo.getState();
          state->disableAllVertexArrays();
          state->disableTexCoordPointer( 0 );

          nvgBeginFrame( nvgContext, parent.parent.getWidth(), parent.parent.getHeight(), 1.0f );
          updateGL( state );
          nvgEndFrame( nvgContext );
        }
      }

      void Overlay::InternalAdapter::releaseGLObjects( osg::State* state ) const {
        if( state ) {
          if( osg::ref_ptr< osg::GraphicsContext > context = state->getGraphicsContext() ) {
            if( context->makeCurrent() ) {
              Overlay::InternalAdapter* self = const_cast< Overlay::InternalAdapter* >( this );

              if( self->nvgContext ) {
                nvgDeleteGL3( self->nvgContext );
                self->nvgContext = nullptr;
              }

              context->releaseContext();
            }
          }
        }
      }

      void Overlay::InternalAdapter::updateGL( osg::State* state ) const {
        parent.drawUnits();
      }

      Overlay::Overlay( const Device::Display& displayDevice ) : parent( displayDevice ) {
        osg::ref_ptr< InternalAdapter > adapter = new InternalAdapter( *this );

        osg::ref_ptr< osg::Geode > geode = new osg::Geode();
        geode->setCullingActive( false );
        geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
        geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        geode->addDrawable( adapter );

        overlay = new osg::Camera;
        overlay->setClearMask( GL_DEPTH_BUFFER_BIT );
        overlay->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
        overlay->setRenderOrder( osg::Camera::POST_RENDER );
        overlay->setAllowEventFocus( false );
        overlay->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
        overlay->addChild( geode );
      }

      OverlayAdapter Overlay::getOverlayAdapter() {
        return overlay;
      }

      void Overlay::drawUnits() {
        // Sort units by zOrder
        std::stable_sort( drawableUnits.begin(), drawableUnits.end() );

        for( std::unique_ptr< Drawable >& drawable : drawableUnits ) {
          drawable->draw();
        }
      }

      unsigned int Overlay::addDrawable( std::unique_ptr< Drawable > drawable ) {
        for( unsigned int i = 0; i != drawableUnits.size(); i++ ) {
          if( !drawableUnits[ i ] ) {
            drawableUnits[ i ] = std::move( drawable );
            return i;
          }
        }

        drawableUnits.emplace_back( std::move( drawable ) );
        return drawableUnits.size() - 1;
      }

      void Overlay::removeDrawable( unsigned int i ) {
        drawableUnits[ i ].reset();
      }

      void Overlay::clear() {
        drawableUnits.clear();
      }

    }
  }
}
