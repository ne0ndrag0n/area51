#ifndef SCENEVIEW
#define SCENEVIEW

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Group;

      class SceneView {
        osgViewer::Viewer viewer;
        osg::ref_ptr< osg::Group > rootGroup;
        osg::ref_ptr< osg::Camera > camera;
        double zoom = 1.0;
        double rotationAngle = 45.0;

        void setupOrthoPerspective( double scaledWidthHalf, double scaledHeightHalf );

      public:
        SceneView( unsigned int width, unsigned int height );

        void setBackgroundColor( const Vec4& color );
        Vec4 getBackgroundColor();

        void setDynamicLighting( bool setting );

        void update();

        void addGroup( const Group& group );
        void removeGroup( const Group& group );
      };

    }
  }
}

#endif
