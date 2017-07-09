#include "graphics/rendering/object.hpp"
#include <osg/Program>
#include <osg/Shader>

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

      void Object::setShader( const std::string& vertPath, const std::string& fragPath ) {
        osg::ref_ptr< osg::Program > program = new osg::Program();

        osg::ref_ptr< osg::Shader > vertex = new osg::Shader( osg::Shader::VERTEX );
        vertex->loadShaderSourceFromFile( vertPath );

        osg::ref_ptr< osg::Shader > fragment = new osg::Shader( osg::Shader::FRAGMENT );
        fragment->loadShaderSourceFromFile( fragPath );

        program->addShader( fragment );
        program->addShader( vertex );

        node->getOrCreateStateSet()->setAttributeAndModes( program, osg::StateAttribute::ON );
      }
    }
  }
}
