#ifndef RENDERING_TYPES
#define RENDERING_TYPES

#include <osg/Node>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/Matrixd>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      using Vec3 = osg::Vec3d;
      using Vec4 = osg::Vec4d;
      using Quat = osg::Quat;

      using RootNode = osg::ref_ptr< osg::PositionAttitudeTransform >;
      using GroupNode = osg::ref_ptr< osg::Group >;
      using Node = osg::ref_ptr< osg::Node >;

      struct Decomposition {
        Vec3 translation;
        Quat rotation;
        Vec3 scale;

        Decomposition() = default;

        Decomposition( const osg::Matrixd& matrix ) {
          osg::Quat unused;

          matrix.decompose( translation, rotation, scale, unused );
        }

        osg::Matrixd toMatrix() {
          osg::Matrixd result;

          result.postMultTranslate( translation );
          result.postMultRotate( rotation );
          result.postMultScale( scale );

          return result;
        }
      };
    }
  }
}


#endif
