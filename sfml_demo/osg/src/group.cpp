#include "graphics/rendering/group.hpp"
#include "graphics/rendering/model.hpp"
#include "graphics/rendering/types.hpp"
#include <osg/Group>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      Group::Group() : Object::Object() {
        node = new osg::Group();

        root->addChild( node );
      }

      /**
       * @friend Object
       */
      void Group::add( Object& object ) {
        node->asGroup()->addChild( object.root );

        object.onAddedToContainer( node->asGroup() );
      }

      /**
       * @friend Object
       */
      void Group::remove( Object& object ) {
        node->asGroup()->removeChild( object.root );

        object.onRemovedFromContainer();
      }

    }
  }
}
