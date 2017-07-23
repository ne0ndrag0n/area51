#ifndef SCENEVIEW
#define SCENEVIEW

#include "graphics/rendering/types.hpp"
#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <memory>
#include <unordered_set>

namespace BlueBear {
  namespace Device {
    class Display;
    class Input;
  }

  namespace Graphics {
    namespace GUI {
      class Overlay;
    }

    namespace Rendering {
      class Group;

      class SceneView {
        osgViewer::Viewer viewer;
        osg::ref_ptr< osg::Group > rootGroup;
        std::unordered_set< std::shared_ptr< Group > > tracked;

      public:
        class Camera;
        std::shared_ptr< Camera > camera;

        SceneView(
          const Device::Display& display,
          const Device::Input& input
        );

        bool update();

        void addGroup( std::shared_ptr< Group > group );
        void removeGroup( std::shared_ptr< Group > group );
      };

    }
  }
}

#endif
