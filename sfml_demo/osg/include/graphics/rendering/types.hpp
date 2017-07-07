#ifndef RENDERING_TYPES
#define RENDERING_TYPES

#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      using Vec3 = osg::Vec3;
      using Vec4 = osg::Vec4;
      using Quat = osg::Quat;

      using RootType = osg::ref_ptr< osg::PositionAttitudeTransform >;

    }
  }
}


#endif
