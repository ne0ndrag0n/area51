#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/sceneview_camera.hpp"
#include "graphics/rendering/group.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      SceneView::SceneView( unsigned int width, unsigned int height ) :
        rootGroup( new osg::Group() ) {
        viewer.setUpViewerAsEmbeddedInWindow( 0, 0, width, height );
        viewer.setSceneData( rootGroup );

        camera = std::make_shared< SceneView::Camera >( viewer.getCamera(), width, height );
        camera->setBackground( osg::Vec4( 0.0f, 0.2f, 0.0f, 1.0f ) );

        viewer.realize();
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

    }
  }
}
