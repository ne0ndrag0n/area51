#ifndef RENDERING_GROUP
#define RENDERING_GROUP

#include "graphics/rendering/object.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class SceneView;

      class Group : public Object {
        friend class SceneView;

      public:
        Group();
        virtual ~Group();

        void add( Object& object );
        void remove( Object& object );
      };

    }
  }
}

#endif
