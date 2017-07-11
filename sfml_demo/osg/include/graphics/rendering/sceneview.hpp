#ifndef SCENEVIEW
#define SCENEVIEW

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class Group;

      class SceneView {
        osgViewer::Viewer viewer;
        osg::ref_ptr< osg::Group > rootGroup;

      public:
        class Camera;
        std::shared_ptr< Camera > camera;

        SceneView( unsigned int width, unsigned int height );

        void update();

        void addGroup( const Group& group );
        void removeGroup( const Group& group );
      };

    }
  }
}

#endif
