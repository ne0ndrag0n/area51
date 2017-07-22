#ifndef WIDGET_ENGINE
#define WIDGET_ENGINE

#include "graphics/gui/widget/style/types.hpp"
#include <vector>
#include <set>
#include <string>
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
          struct Selector {
            std::string tag;
            std::string id;
            std::vector< std::string > classes;
          };

          struct StylesheetQuery {
            std::string query;
            Style::RuleMap rules;
          };

          std::vector< StylesheetQuery > styleSheet;
          std::shared_ptr< Container > root;

          void buildDefaultStylesheet();
          void refreshStylesheet();
          Selector toSelector( const std::string& token );
          std::set< std::shared_ptr< Node > > selectItems(
             const Selector& selector,
             const std::set< std::shared_ptr< Node > > from
          );

        public:
          WidgetEngine();
          ~WidgetEngine();
        };

      }
    }
  }
}


#endif
