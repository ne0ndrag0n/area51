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

            // Find the LATEST result for the queried value that applies to this node
            // Position 0 should always be the "*" rule
            stx::any result;
            for( RuleMap* rules : matchingQueries ) {
              auto it = rules->find( key );

              if( it != rules->end() ) {
                result = it->second;
              }
            }

            return result;
          }

          void NodeStyle::setValue( const std::string& key, stx::any value ) {
            localRules[ key ] = value;
          }

          void NodeStyle::clearMatchingQueries() {
            matchingQueries.clear();
          }

          void NodeStyle::pushMatchingQuery( RuleMap* ruleMap ) {
            matchingQueries.emplace_back( ruleMap );
          }

        }
      }
    }
  }
}
