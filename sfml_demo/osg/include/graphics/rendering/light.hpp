#ifndef BLUEBEAR_LIGHT
#define BLUEBEAR_LIGHT

#include "tools/tombstonecounter.hpp"
#include <osg/ref_ptr>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Observer>
#include <osg/PositionAttitudeTransform>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      class Light {
        osg::ref_ptr< osg::Light > light;
        osg::ref_ptr< osg::LightSource > lightSource;
        osg::ref_ptr< osg::PositionAttitudeTransform > root;

        static Tools::TombstoneCounter counter;
        class DeleteObserver : public osg::Observer {
        public:
          void objectDeleted( osg::Light* observed );
        };
        static DeleteObserver deleteObserver;

      public:
        Light();
      };

    }
  }
}

#endif
