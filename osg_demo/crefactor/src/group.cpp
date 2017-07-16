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

        setShader();
      }

      Group::~Group() {
        // Remove all children from this group (parent destructor will remove the group from any parents *it* has)
        // This will free up any associated Model instances to be added to other groups
        osg::ref_ptr< osg::Group > group = node->asGroup();
        group->removeChildren( 0, group->getNumChildren() );
      }

      std::shared_ptr< Group > Group::create() {
        return std::shared_ptr< Group >( new Group() );
      }

      std::shared_ptr< Group > Group::clone( std::shared_ptr< Group > other ) {
        return std::shared_ptr< Group >( new Group( *other ) );
      }

      /**
       * @friend Object
       */
      void Group::add( std::shared_ptr< Object > object ) {
        if( object->root->getNumParents() < 1 ) {
          node->asGroup()->addChild( object->root );

          object->onAddedToContainer( node->asGroup() );

          tracked.insert( object );
        } else {
          // TODO: Single-parent warning
        }
      }

      /**
       * @friend Object
       */
      void Group::remove( std::shared_ptr< Object > object ) {
        if( tracked.find( object ) != tracked.end() ) {
          node->asGroup()->removeChild( object->root );

          object->onRemovedFromContainer();

          tracked.erase( object );
        } else {
          // TODO: Not added warning
        }
      }

    }
  }
}
