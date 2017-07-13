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

      void SceneView::addGroup( std::shared_ptr< Group > group ) {
        if( group->root->getNumParents() < 1 ) {
          rootGroup->addChild( group->root );

          tracked.insert( group );
        } else {
          // TODO: Single-parent warning
        }
      }

      void SceneView::removeGroup( std::shared_ptr< Group > group ) {
        if( tracked.find( group ) != tracked.end() ) {
          rootGroup->removeChild( group->root );
          tracked.erase( group );
        } else {
          // TODO: Not added warning
        }
      }

    }
  }
}
