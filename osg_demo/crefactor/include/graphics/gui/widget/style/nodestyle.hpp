#ifndef NODE_STYLE
#define NODE_STYLE

#include "graphics/gui/widget/style/types.hpp"
#include <string>
#include <any>
#include <vector>
#include <exception>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class WidgetEngine;

        namespace Style {

          class NodeStyle {
            friend class WidgetEngine;
            std::vector< RuleMap* > matchingQueries;
            RuleMap localRules;

          public:
            stx::any getValue( const std::string& key ) const;
            void setValue( const std::string& key, stx::any value );

            struct InvalidRuleException : public std::exception {
              const char* what() const throw() { return "Invalid rule specified!"; }
            };
          };

        }
      }
    }
  }
}


#endif
