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
          parent.drawUnits( nvgContext );
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

      OverlayHelper Overlay::getOverlayHelper() const {
        return overlay;
      }

      void Overlay::drawUnits( NVGcontext* context ) {
        // Sort units by zOrder
        std::stable_sort( drawableUnits.begin(), drawableUnits.end(), []( const std::shared_ptr< Drawable > lhs, const std::shared_ptr< Drawable > rhs ) {
          return lhs->zOrder < rhs->zOrder;
        } );

        for( std::shared_ptr< Drawable > drawable : drawableUnits ) {
          if( drawable ) {
            drawable->draw( context );
          }
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

    }
  }
}
