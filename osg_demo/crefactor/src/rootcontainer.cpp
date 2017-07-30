#include "graphics/gui/widget/rootcontainer.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        bool RootContainer::fireSignal( const std::string& signalId, const stx::any& data ) {
          bool result = false;

          for( std::shared_ptr< Node > child : children ) {
            result = child->fireSignal( signalId, data ) || result;
          }

          return result;
        }

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
