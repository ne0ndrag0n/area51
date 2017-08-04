#include "graphics/gui/widget/rootcontainer.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        std::string RootContainer::getName() const {
          return "RootContainer";
        }

        std::shared_ptr< RootContainer > RootContainer::create() {
          return std::shared_ptr< RootContainer >( new RootContainer );
        }

      }
    }
  }
}
