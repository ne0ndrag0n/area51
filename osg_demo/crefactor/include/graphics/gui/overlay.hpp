#ifndef GFX_OVERLAY
#define GFX_OVERLAY

#include "graphics/gui/drawable.hpp"
#include "graphics/gui/types.hpp"
#include <nanovg/nanovg.h>
#include <osg/ref_ptr>
#include <osg/Camera>
#include <osg/Drawable>
#include <osg/RenderInfo>
#include <osg/State>
#include <memory>
#include <vector>

namespace BlueBear {
  namespace Device {
    class Display;
  }

  namespace Graphics {
    namespace GUI {

      class Overlay {
        NVGcontext* nvgContext = nullptr;
        std::vector< std::shared_ptr< Drawable > > drawableUnits;

        void loadFonts( NVGcontext* context );
        void drawUnits( NVGcontext* context );

      public:
        Overlay();
        ~Overlay();

        void update();
        void addDrawable( std::shared_ptr< Drawable > drawable );
        void removeDrawable( std::shared_ptr< Drawable > drawable );
        void clear();
      };

    }
  }
}



#endif
