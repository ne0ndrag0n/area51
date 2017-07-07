#ifndef RENDERING_GROUP
#define RENDERING_GROUP

#include <osg/ref_ptr>
#include <osg/Group>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Model;

      class Group {
        osg::ref_ptr< osg::Group > group;

      public:
        Group() = default;

        void addChild( const Model& model );
        void removeChild( const Model& model );
      };

    }
  }
}

#endif
