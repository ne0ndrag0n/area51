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

        void RootContainer::draw( DrawableContext* context ) {
          // Rootcontainer has no drawable of its own

          // Rootcontainer sets and renders children as overlay positioned
          for( std::shared_ptr< Node > node : children ) {
            node->draw( context );
          }
        }

      }
    }
  }
}
