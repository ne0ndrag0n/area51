#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/group.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      const std::array< SceneView::OrthoRotationMode, 4 > SceneView::ROTATION_MODES = {
        SceneView::OrthoRotationMode{ false, false, 45.0 },
        SceneView::OrthoRotationMode{ true, false, -45.0 },
        SceneView::OrthoRotationMode{ true, true, -135.0 },
        SceneView::OrthoRotationMode{ false, true, -225.0 }
      };

      SceneView::SceneView( unsigned int width, unsigned int height ) :
        rootGroup( new osg::Group() ) {
        viewer.setUpViewerAsEmbeddedInWindow( 0, 0, width, height );

        viewer.setSceneData( rootGroup );

        camera = viewer.getCamera();
        camera->setClearColor( osg::Vec4( 0.0f, 0.2f, 0.0f, 1.0f ) );
        setupOrthoPerspective(
          ( ( ( double ) width / 2 ) * zoom ) / 100.0,
          ( ( ( double ) height / 2 ) * zoom ) / 100.0
        );

        cameraMetrics.translation = osg::Vec3( 10.0, 10.0, -10.0 );
        cameraMetrics.rotation =
          osg::Quat( osg::DegreesToRadians( 45.0 ), osg::Vec3( 0.0, 0.0, 1.0 ) ) *
          osg::Quat( osg::DegreesToRadians( -60.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) );
        cameraMetrics.scale = osg::Vec3( 1.0, 1.0, 1.0 );
        updateCameraMetrics();

        viewer.realize();
      }

      void SceneView::updateCameraMetrics() {
        camera->setViewMatrix( cameraMetrics.toMatrix() );
      }

      /**
       * @private
       */
      void SceneView::setupOrthoPerspective( double scaledWidthHalf, double scaledHeightHalf ) {
        camera->setProjectionMatrixAsOrtho( -scaledWidthHalf, scaledWidthHalf, -scaledHeightHalf, scaledHeightHalf, -20.0, 50.0 );
      }

      void SceneView::setBackgroundColor( const Vec4& color ) {
        camera->setClearColor( color );
      }

      Vec4 SceneView::getBackgroundColor() {
        return camera->getClearColor();
      }

      void SceneView::update() {
        viewer.frame();
      }

      void SceneView::addGroup( const Group& group ) {
        rootGroup->addChild( group.root );
      }

      void SceneView::removeGroup( const Group& group ) {
        rootGroup->removeChild( group.root );
      }

      void SceneView::setRotation( unsigned int angle ) {

      }

    }
  }
}
