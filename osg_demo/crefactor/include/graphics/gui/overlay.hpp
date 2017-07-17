#ifndef GFX_OVERLAY
#define GFX_OVERLAY

//#include <GL/glew.h>
#include "graphics/gui/drawable.hpp"
#include "graphics/gui/types.hpp"
#include <nanovg/nanovg.h>
#include <osg/ref_ptr>
#include <osg/Camera>
#include <osg/Drawable>
#include <osg/RenderInfo>
#include <osg/State>
#include <memory>

namespace BlueBear {
  namespace Device {
    class Display;
  }

  namespace Graphics {
    namespace GUI {

      class Overlay {
        class InternalAdapter : public osg::Drawable {
          Overlay& parent;
          NVGcontext* nvgContext = nullptr;
          unsigned int activeContextID;

        public:
          InternalAdapter( Overlay& parent );

          virtual void drawImplementation( osg::RenderInfo& renderInfo ) const override;
          virtual void releaseGLObjects( osg::State* state = 0 ) const override;
          virtual void updateGL( osg::State* state ) const;
        };

        const Device::Display& parent;
        osg::ref_ptr< osg::Camera > overlay;
        std::vector< std::unique_ptr< Drawable > > drawableUnits;

        void drawUnits();

      public:
        Overlay( const Device::Display& displayDevice );

        OverlayAdapter getOverlayAdapter();
        unsigned int addDrawable( std::unique_ptr< Drawable > drawable );
        void removeDrawable( unsigned int i );
        void clear();
      };

    }
  }
}



#endif
