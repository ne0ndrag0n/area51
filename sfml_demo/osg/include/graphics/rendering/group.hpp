#ifndef RENDERING_GROUP
#define RENDERING_GROUP

#include "graphics/rendering/object.hpp"
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {
      class SceneView;

      class Group : public Object {
        friend class SceneView;

      protected:
        Group();

      public:
        virtual ~Group();

        static std::shared_ptr< Group > create();
        static std::shared_ptr< Group > clone( std::shared_ptr< Group > other );

        void add( std::shared_ptr< Object > object );
        void remove( std::shared_ptr< Object > object );
      };

    }
  }
}

#endif
