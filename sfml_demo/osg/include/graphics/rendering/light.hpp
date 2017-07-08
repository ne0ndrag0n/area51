#ifndef BLUEBEAR_LIGHT
#define BLUEBEAR_LIGHT

#include "graphics/rendering/object.hpp"
#include "graphics/rendering/types.hpp"
#include "tools/tombstonecounter.hpp"
#include <osg/ref_ptr>
#include <osg/Light>
#include <osg/Observer>
#include <osg/PositionAttitudeTransform>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      class Light : public Object {
        osg::ref_ptr< osg::Light > light;

        static Tools::TombstoneCounter counter;
        class DeleteObserver : public osg::Observer {
        public:
          void objectDeleted( osg::Light* observed );
        };
        static DeleteObserver deleteObserver;

        osg::ref_ptr< osg::LightSource > getNodeAsLightSource();

      protected:
        void onAddedToContainer( GroupNode parent );

      public:
        Light();

        void setDiffuse( const Vec4& color );
        void setAmbient( const Vec4& color );
        void setSpecular( const Vec4& color );

        Vec4 getDiffuse();
        Vec4 getAmbient();
        Vec4 getSpecular();
      };

    }
  }
}

#endif
