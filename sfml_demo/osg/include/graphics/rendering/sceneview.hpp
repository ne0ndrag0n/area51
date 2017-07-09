#ifndef SCENEVIEW
#define SCENEVIEW

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <array>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Group;

      class SceneView {
        osgViewer::Viewer viewer;
        osg::ref_ptr< osg::Group > rootGroup;
        osg::ref_ptr< osg::Camera > camera;
        double zoom = 1.5;
        Decomposition cameraMetrics;

        struct OrthoRotationMode {
          bool flipX;
          bool flipY;
          double rotationAngle;
        };

        static const std::array< OrthoRotationMode, 4 > ROTATION_MODES;

        void setupOrthoPerspective( double scaledWidthHalf, double scaledHeightHalf );
        void updateCameraMetrics();

      public:
        SceneView( unsigned int width, unsigned int height );

        void setBackgroundColor( const Vec4& color );
        Vec4 getBackgroundColor();

        void update();

        void addGroup( const Group& group );
        void removeGroup( const Group& group );

        void setRotation( unsigned int angle );
      };

    }
  }
}

#endif
