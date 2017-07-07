#include "graphics/rendering/group.hpp"
#include "graphics/rendering/model.hpp"
#include "graphics/rendering/types.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      void Group::addChild( const Model& model ) {
        group->addChild( model.root );
      }

      void Group::removeChild( const Model& model ) {
        group->removeChild( model.root );
      }

    }
  }
}
