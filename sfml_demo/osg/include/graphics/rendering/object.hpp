#ifndef RENDERING_OBJECT
#define RENDERING_OBJECT

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/PositionAttitudeTransform>
#include <vector>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Group;

      class Object {
        friend class Group;

        template< typename T >
        struct NamedNodeLocator : public osg::NodeVisitor {
          std::vector< osg::ref_ptr< T > > foundNodes;
          const std::string& target;

          NamedNodeLocator( const std::string& target )
            : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ),
              target( target ) {}

          void apply( T& node ) {
            if( node.getName() == target ) {
              foundNodes.emplace_back( &node );
            }

            traverse( node );
          }
        };

        template< typename T >
        struct TypedNodeLocator : public osg::NodeVisitor {
          std::vector< osg::ref_ptr< T > > foundNodes;

          TypedNodeLocator() : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {}

          void apply( T& node ) {
            foundNodes.emplace_back( &node );

            traverse( node );
          }
        };

      protected:
        osg::ref_ptr< osg::PositionAttitudeTransform > root;
        osg::ref_ptr< osg::Node > node;

        virtual void onAddedToContainer( GroupNode parent );
        virtual void onRemovedFromContainer();

        template< typename T >
        std::vector< osg::ref_ptr< T > > findNodesByID( const std::string& nodeID ) {
          NamedNodeLocator< T > locator( nodeID );
          node->accept( locator );

          return locator.foundNodes;
        }

        template< typename T >
        std::vector< osg::ref_ptr< T > > findNodesByType() {
          TypedNodeLocator< T > locator;
          node->accept( locator );

          return locator.foundNodes;
        }

      public:
        Object();

        void setPosition( const Vec3& position );
        Vec3 getPosition();

        void setAttitude( const Quat& attitude );
        Quat getAttitude();

        void setScale( const Vec3& scale );
        Vec3 getScale();

        void setShader(
          const std::string& vertPath = "system/shaders/default_vertex.glsl",
          const std::string& fragPath = "system/shaders/default_fragment.glsl"
        );
      };

    }
  }
}

#endif
