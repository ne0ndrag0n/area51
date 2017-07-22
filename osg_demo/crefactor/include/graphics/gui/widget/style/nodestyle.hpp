#ifndef NODE_STYLE
#define NODE_STYLE

#include "graphics/gui/widget/style/types.hpp"
#include <string>
#include <any>
#include <vector>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class WidgetEngine;

        namespace Style {

          class NodeStyle {
            std::vector< const RuleMap* > matchingQueries;
            RuleMap localRules;

          public:
            stx::any getValue( const std::string& key ) const;
            void setValue( const std::string& key, stx::any value );

            void clearMatchingQueries();
            void pushMatchingQuery( const RuleMap* ruleMap );
          };

        }
      }
    }
  }
}


#endif
