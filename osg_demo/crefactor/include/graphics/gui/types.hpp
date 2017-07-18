#ifndef GRAPHICS_GUI_TYPES
#define GRAPHICS_GUI_TYPES

#include <osg/ref_ptr>
#include <osg/Camera>
#include <nanovg/nanovg.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
        using OverlayHelper = osg::ref_ptr< osg::Camera >;
        using DrawableContext = NVGcontext;
    }
  }
}

#endif
