#ifndef WIDGET_ENGINE
#define WIDGET_ENGINE

#include "graphics/gui/widget/style/types.hpp"
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <tuple>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;
        class Container;

        /**
         * The WidgetEngine sits next to the Overlay and submits drawables to it, ordering each drawable's z-order prior to
         * the re-draw of Overlay.
         */
        class WidgetEngine {
          std::vector< std::pair< std::string, Style::RuleMap > > styleSheet;
          std::shared_ptr< Container > root;

          void refreshStylesheet();
          std::tuple< std::string, std::string, std::string > tokenToTuple( const std::string& token );

        public:
          WidgetEngine();

        };

      }
    }
  }
}


#endif
