#ifndef RENDERING_OBJECT
#define RENDERING_OBJECT

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Group;

      class Object {
        friend class Group;

      protected:
        osg::ref_ptr< osg::PositionAttitudeTransform > root;
        osg::ref_ptr< osg::Node > node;

        virtual void onAddedToContainer( GroupNode parent );
        virtual void onRemovedFromContainer();

      public:
        Object();

        void setPosition( const Vec3& position );
        Vec3 getPosition();

        void setAttitude( const Quat& attitude );
        Quat getAttitude();

        void setScale( const Vec3& scale );
        Vec3 getScale();
      };

    }
  }
}

#endif
