#ifndef SCENEVIEW_CAMERA
#define SCENEVIEW_CAMERA

#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Camera>
#include <array>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      class SceneView::Camera {
        double width;
        double height;
        osg::ref_ptr< osg::Camera > camera;
        unsigned int rotation = 0;
        double zoom = 1.0;
        osg::Vec3 position = osg::Vec3( 10.0, 10.0, -10.0 );

        struct RotationMode {
          bool flipX;
          bool flipY;
          double rotationAngle;
        };
        static const std::array< RotationMode, 4 > ROTATION_MODES;

        double getAdjustedX( double input );
        double getAdjustedY( double input );

        void updateViewMatrix();
        void updateProjectionMatrix();

      public:
        Camera( osg::ref_ptr< osg::Camera > camera, double width, double height );

        void setPosition( const Vec3& position );
        Vec3 getPosition();

        void setRotation( unsigned int rotation );
        void setBackground( const Vec4& colour );

        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();

        void zoomIn();
        void zoomOut();
      };

    }
  }
}

#endif
