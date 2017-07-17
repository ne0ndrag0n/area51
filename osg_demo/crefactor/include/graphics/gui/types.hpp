#ifndef GRAPHICS_GUI_TYPES
#define GRAPHICS_GUI_TYPES

#include <osg/ref_ptr>
#include <osg/Camera>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
        using OverlayAdapter = osg::ref_ptr< osg::Camera >;
    }
  }
}

#endif
