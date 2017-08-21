#ifndef CONTAINER_POSITIONER
#define CONTAINER_POSITIONER

#include "containers/rect.hpp"
#include "graphics/gui/widget/container.hpp"
#include <vector>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;

        class Container::Positioner {
          Container& parent;

          std::vector< std::shared_ptr< Node > > flowNodes;
          std::vector< std::shared_ptr< Node > > overlayNodes;

          std::vector< std::shared_ptr< Node > > autoNodes;
          std::vector< std::shared_ptr< Node > > numericNodes;

          Containers::Rect< int > flowRegion;

          void buildAutoNumericPairs( const std::string& dimension );
          void setFull( const std::string& dimension, double amount );

        public:
          Positioner( Container& parent );

          void positionLTR();
        };

      }
    }
  }
}


#endif
