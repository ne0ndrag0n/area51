#include "graphics/rendering/model.hpp"
#include <osgAnimation/AnimationManagerBase>
#include <osgDB/ReadFile>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      Model::AnimFinder::AnimFinder() : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {}

      void Model::AnimFinder::apply( osg::Node& node ) {
        if( animationManager.valid() ) {
          return;
        }

        if( osgAnimation::AnimationManagerBase* b = dynamic_cast< osgAnimation::AnimationManagerBase* >( node.getUpdateCallback() ) ) {
          animationManager = new osgAnimation::BasicAnimationManager( *b );
          return;
        }

        traverse( node );
      }

      Model::Model( const std::string& path ) {
        model = osgDB::readRefNodeFile( path );

        if( !model ) {
          throw InvalidModelException();
        }

        buildAnimationMap();

        // All models are children of transforms
        root = new osg::PositionAttitudeTransform();
        root->addChild( model );
      }

      /**
       * @private
       * Build a map of animations that can later be used to play the animations
       */
      void Model::buildAnimationMap() {
        AnimFinder finder;
        model->accept( finder );

        if( finder.animationManager.valid() ) {
          model->setUpdateCallback( finder.animationManager.get() );

          const osgAnimation::AnimationList& modelAnims = finder.animationManager->getAnimationList();
          for( osg::ref_ptr< osgAnimation::Animation > animation : modelAnims ) {
            animationMap[ animation->getName() ] = animation;
          }
        }
      }

      void Model::setPosition( const Vec3& position ) {
        root->setPosition( position );
      }

      Vec3 Model::getPosition() {
        return root->getPosition();
      }

      void Model::setAttitude( const Quat& attitude ) {
        root->setAttitude( attitude );
      }

      Quat Model::getAttitude() {
        return root->getAttitude();
      }

      void Model::setScale( const Vec3& scale ) {
        root->setScale( scale );
      }

      Vec3 Model::getScale() {
        return root->getScale();
      }

    }
  }
}
