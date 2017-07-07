#include "graphics/rendering/light.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      Tools::TombstoneCounter Light::counter( { 0 } );

      void Light::DeleteObserver::objectDeleted( osg::Light* observed ) {
        Light::counter.free( observed->getLightNum() );
      }

      Light::DeleteObserver Light::deleteObserver;

      Light::Light() {
        root = new osg::PositionAttitudeTransform();

        light = new osg::Light();
        // Each instance might need their own delete observer? then will the memory leak?
        light->addObserver( &Light::deleteObserver );

        light->setLightNum( Light::counter.getNextValue() );
        light->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
      }
    }
  }
}
