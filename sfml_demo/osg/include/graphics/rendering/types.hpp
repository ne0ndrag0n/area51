#ifndef RENDERING_TYPES
#define RENDERING_TYPES

#include <osg/Node>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      using Vec3 = osg::Vec3;
      using Vec4 = osg::Vec4;
      using Quat = osg::Quat;

      using RootNode = osg::ref_ptr< osg::PositionAttitudeTransform >;
      using GroupNode = osg::ref_ptr< osg::Group >;
      using Node = osg::ref_ptr< osg::Node >;
    }
  }
}


#endif
