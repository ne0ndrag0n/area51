#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/sceneview_camera.hpp"
#include "graphics/rendering/group.hpp"
#include "device/display.hpp"
#include "device/input.hpp"
#include <osg/GraphicsContext>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      SceneView::SceneView( const Device::Display& display, const Device::Input& input ) :
        rootGroup( new osg::Group() ) {
        unsigned int width = display.getWidth();
        unsigned int height = display.getHeight();

        viewer.addEventHandler( input.getAdapter() );

        viewer.setSceneData( rootGroup );

        osg::ref_ptr< osg::Camera > camPtr = viewer.getCamera();
        camPtr->setGraphicsContext( display.getContextHelper() );
        camPtr->setViewport( 0, 0, width, height );

        camera = std::make_shared< SceneView::Camera >( camPtr, width, height );
        camera->setBackground( osg::Vec4( 0.0f, 0.2f, 0.0f, 1.0f ) );

        viewer.realize();
      }

      bool SceneView::update() {
        if( viewer.done() ) {
          return false;
        }

        viewer.frame();
        return true;
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
