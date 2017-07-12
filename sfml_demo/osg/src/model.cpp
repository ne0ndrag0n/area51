#include "graphics/rendering/model.hpp"
#include <osg/Object>
#include <osg/Image>
#include <osg/Geometry>
#include <osgAnimation/AnimationManagerBase>
#include <osgDB/ReadFile>
#include <osgAnimation/RigTransformHardware>
#include <osgAnimation/RigGeometry>
#include <algorithm>
#include <iostream>

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

      Model::RigTransformImplSetter::RigTransformImplSetter() : osg::NodeVisitor( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ) {}

      void Model::RigTransformImplSetter::apply( osg::Geode& geode ) {
        for( unsigned int i = 0; i < geode.getNumDrawables(); i++ ) {
          apply( geode.getDrawable( i ) );
        }
      }

      void Model::RigTransformImplSetter::apply( osg::Drawable* drawable ) {
        if( osgAnimation::RigGeometry* rig = dynamic_cast< osgAnimation::RigGeometry* >( drawable ) ) {
          rig->setRigTransformImplementation( new osgAnimation::RigTransformHardware() );
        }
      }

      Model::Texture::Texture( const std::string& path ) {
        image = osgDB::readImageFile( path );

        if( !image ) {
          throw InvalidTextureException();
        }
      }

      Model::Texture::Texture( osg::ref_ptr< osg::Image > image ) : image( image ) {}

      void Model::Texture::applyTo( Model& model, const std::string& nodeID, unsigned int unit ) const {
        auto nodes = model.findNodesByID< osg::Geometry >( nodeID );

        if( nodes.size() == 1 ) {
          osg::ref_ptr< osg::StateSet > stateSet = nodes[ 0 ]->getOrCreateStateSet();

          if( osg::ref_ptr< osg::Texture2D > texture = dynamic_cast< osg::Texture2D* >( stateSet->getTextureAttribute( unit, osg::StateAttribute::TEXTURE ) ) ) {
            texture->setImage( image );
          } else {
            // TODO: Warning
          }
        } else {
          // TODO: Warning
        }
      }

      Model::Model( const std::string& path ) : Object::Object() {
        node = osgDB::readRefNodeFile( path );

        if( !node ) {
          throw InvalidModelException();
        }

        buildAnimationMap();
        auto geometries = findNodesByType< osg::Geometry >();
        std::for_each( geometries.begin(), geometries.end(), []( osg::ref_ptr< osg::Geometry > geometry ) {
          if( osg::ref_ptr< osg::Texture2D > texture = dynamic_cast< osg::Texture2D* >( geometry->getOrCreateStateSet()->getTextureAttribute( 0, osg::StateAttribute::TEXTURE ) ) ) {
            texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::NEAREST );
            texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::NEAREST );
          }
        } );

        root->addChild( node );
      }

      Model::Model( const Model& model ) : Object::Object( model ) {
        buildAnimationMap();
      }

      void Model::playAnimation( const std::string& animationID ) {
        if( animationManager ) {
          auto it = animationMap.find( animationID );
          if( it != animationMap.end() ) {
            animationManager->playAnimation( it->second );
          } else {
            // TODO: Log message warning this animationID isn't present.
          }
        } else {
          // TODO: Log message warning that there's no animations
        }
      }

      /**
       * @private
       * Build a map of animations that can later be used to play the animations
       */
      void Model::buildAnimationMap() {
        AnimFinder finder;
        node->accept( finder );

        if( finder.animationManager.valid() ) {
          node->setUpdateCallback( finder.animationManager.get() );

          const osgAnimation::AnimationList& modelAnims = finder.animationManager->getAnimationList();
          for( osg::ref_ptr< osgAnimation::Animation > animation : modelAnims ) {
            animation->setPlayMode( osgAnimation::Animation::ONCE );
            animationMap[ animation->getName() ] = animation;
          }

          animationManager = finder.animationManager;
        }

      }
    }
  }
}
