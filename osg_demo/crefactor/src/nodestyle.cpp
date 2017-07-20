#include "graphics/gui/widget/style/nodestyle.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        namespace Style {

          stx::any NodeStyle::getValue( const std::string& key ) const {
            // Local rules take precedence over everything else
            auto it = localRules.find( key );
            if( it != localRules.end() ) {
              return it->second;
            }

            // WidgetEngine should insert these in the following order:
            // - matching id
            // - matching class
            // - the global default RuleMap for all elements
            for( RuleMap* rules : matchingQueries ) {
              auto it = rules->find( key );

              if( it != rules->end() ) {
                return it->second;
              }
            }

            throw InvalidRuleException();
          }

          void NodeStyle::setValue( const std::string& key, stx::any value ) {
            localRules[ key ] = value;
          }

        }
      }
    }
  }
}
