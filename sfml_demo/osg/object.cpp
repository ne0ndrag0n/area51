#include "graphics/rendering/object.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      Object::Object() {
        root = new osg::PositionAttitudeTransform();
      }

      void Object::setPosition( const Vec3& position ) {
        root->setPosition( position );
      }

      Vec3 Object::getPosition() {
        return root->getPosition();
      }

      void Object::setAttitude( const Quat& attitude ) {
        root->setAttitude( attitude );
      }

      Quat Object::getAttitude() {
        return root->getAttitude();
      }

      void Object::setScale( const Vec3& scale ) {
        root->setScale( scale );
      }

      Vec3 Object::getScale() {
        return root->getScale();
      }

      void Object::onAddedToContainer( GroupNode parent ) {
        // Empty, abstract, not always needed
      }

      void Object::onRemovedFromContainer() {
        // Empty, abstract, not always needed
      }

    }
  }
}
