#include "graphics/rendering/light.hpp"
#include <osg/LightSource>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      Tools::TombstoneCounter Light::counter( { 0 } );

      void Light::DeleteObserver::objectDeleted( osg::Light* observed ) {
        Light::counter.free( observed->getLightNum() );
      }

      Light::DeleteObserver Light::deleteObserver;

      Light::Light() : Object::Object() {
        // a LightSource is a Node

        light = new osg::Light();
        // Each instance might need their own delete observer? then will the memory leak?
        light->addObserver( &Light::deleteObserver );
        light->setLightNum( Light::counter.getNextValue() );
        light->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );

        osg::ref_ptr< osg::LightSource > lightSource = new osg::LightSource();
        lightSource->setLight( light );
        lightSource->setLocalStateSetModes( osg::StateAttribute::ON );

        node = lightSource;
        root->addChild( node );
      }

      osg::ref_ptr< osg::LightSource > Light::getNodeAsLightSource() {
        return ( osg::LightSource* ) node.get();
      }

      void Light::onAddedToContainer( GroupNode parent ) {
        osg::ref_ptr< osg::StateSet > groupStateSet = parent->getOrCreateStateSet();

        getNodeAsLightSource()->setStateSetModes( *parent->getOrCreateStateSet(), osg::StateAttribute::ON );
      }

      void Light::setDiffuse( const Vec4& color ) {
        light->setDiffuse( color );
      }

      void Light::setAmbient( const Vec4& color ) {
        light->setAmbient( color );
      }

      void Light::setSpecular( const Vec4& color ) {
        light->setSpecular( color );
      }

      Vec4 Light::getDiffuse() {
        return light->getDiffuse();
      }

      Vec4 Light::getAmbient() {
        return light->getAmbient();
      }

      Vec4 Light::getSpecular() {
        return light->getSpecular();
      }
    }
  }
}
