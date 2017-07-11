#include "graphics/rendering/sceneview_camera.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      const std::array< SceneView::Camera::RotationMode, 4 > SceneView::Camera::ROTATION_MODES = {
        SceneView::Camera::RotationMode{ false, false, 45.0 },
        SceneView::Camera::RotationMode{ true, false, -45.0 },
        SceneView::Camera::RotationMode{ true, true, -135.0 },
        SceneView::Camera::RotationMode{ false, true, -225.0 }
      };

      SceneView::Camera::Camera( osg::ref_ptr< osg::Camera > camera, double width, double height ) : width( width ), height( height ), camera( camera ) {
        updateViewMatrix();
        updateProjectionMatrix();
      }

      double SceneView::Camera::getAdjustedX( double input ) {
        return ROTATION_MODES[ rotation ].flipX ? -input : input;
      }

      double SceneView::Camera::getAdjustedY( double input ) {
        return ROTATION_MODES[ rotation ].flipY ? -input : input;
      }

      void SceneView::Camera::updateViewMatrix() {
        bool swap = rotation == 1 || rotation == 3;
        osg::Matrixd viewMatrix;

        viewMatrix.postMultTranslate(
          osg::Vec3(
            getAdjustedX( swap ? position.y() : position.x() ), getAdjustedY( swap ? position.x() : position.y() ), -10.0
          )
        );
        viewMatrix.postMultRotate(
          osg::Quat( osg::DegreesToRadians( ROTATION_MODES[ rotation ].rotationAngle ), osg::Vec3( 0.0, 0.0, 1.0 ) ) *
          osg::Quat( osg::DegreesToRadians( -60.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) )
        );

        camera->setViewMatrix( viewMatrix );
      }

      void SceneView::Camera::setPosition( const Vec3& position ) {
        this->position = position;

        updateViewMatrix();
      }

      Vec3 SceneView::Camera::getPosition() {
        return position;
      }

      void SceneView::Camera::updateProjectionMatrix() {
        double scaledWidthHalf = ( ( ( double ) width / 2 ) * zoom ) / 100.0;
        double scaledHeightHalf = ( ( ( double ) height / 2 ) * zoom ) / 100.0;

        camera->setProjectionMatrixAsOrtho( -scaledWidthHalf, scaledWidthHalf, -scaledHeightHalf, scaledHeightHalf, -20.0, 50.0 );
      }

      void SceneView::Camera::setRotation( unsigned int rotation ) {
        this->rotation = rotation;

        updateViewMatrix();
      }

      void SceneView::Camera::setBackground( const Vec4& colour ) {
        camera->setClearColor( colour );
      }

      void SceneView::Camera::moveLeft() {
        setPosition( getPosition() + Vec3( 0.1, -0.1, 0.0 ) );
      }

      void SceneView::Camera::moveRight() {
        setPosition( getPosition() + Vec3( -0.1, 0.1, 0.0 ) );
      }

      void SceneView::Camera::moveUp() {
        setPosition( getPosition() + Vec3( -0.1, -0.1, 0.0 ) );
      }

      void SceneView::Camera::moveDown() {
        setPosition( getPosition() + Vec3( 0.1, 0.1, 0.0 ) );
      }

      void SceneView::Camera::zoomIn() {
        if( zoom != 1.0 ) {
          zoom -= 0.5;
        }

        updateProjectionMatrix();
      }

      void SceneView::Camera::zoomOut() {
        if( zoom != 3.0 ) {
          zoom += 0.5;
        }

        updateProjectionMatrix();
      }

    }
  }
}
