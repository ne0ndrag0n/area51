#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/group.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      SceneView::SceneView( unsigned int width, unsigned int height ) :
        rootGroup( new osg::Group() ) {
        viewer.setUpViewerAsEmbeddedInWindow( 0, 0, width, height );

        viewer.setSceneData( rootGroup );
        setDynamicLighting( false );

        camera = viewer.getCamera();
        camera->setClearColor( osg::Vec4( 0.0f, 0.2f, 0.0f, 1.0f ) );
        setupOrthoPerspective(
          ( ( ( double ) width / 2 ) * zoom ) / 100.0,
          ( ( ( double ) height / 2 ) * zoom ) / 100.0
        );

        osg::Matrixd viewMatrix;
        viewMatrix.postMultTranslate( osg::Vec3( 10.0, 10.0, -10.0 ) );
        viewMatrix.postMultRotate(
          osg::Quat( osg::DegreesToRadians( 45.0 ), osg::Vec3( 0.0, 0.0, 1.0 ) ) *
          osg::Quat( osg::DegreesToRadians( -60.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) )
        );
        camera->setViewMatrix( viewMatrix );

        viewer.realize();
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

      void SceneView::setDynamicLighting( bool setting ) {
        rootGroup->getOrCreateStateSet()->setMode( GL_LIGHTING, setting ? osg::StateAttribute::ON : osg::StateAttribute::OFF );
      }

    }
  }
}
