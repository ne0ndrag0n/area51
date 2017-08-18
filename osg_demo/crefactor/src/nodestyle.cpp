#include "graphics/gui/widget/style/nodestyle.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        namespace Style {

          stx::any NodeStyle::getValue( const std::string& key ) const {

            // Cmoputed rules take precedence over everything else
            {
              auto it = computedRules.find( key );
              if( it != computedRules.end() ) {
                return it->second;
              }
            }

            // Now move onto local rules
            auto it = localRules.find( key );
            if( it != localRules.end() ) {
              return it->second;
            }

            // Find the LATEST result for the queried value that applies to this node
            // Position 0 should always be the "*" rule
            stx::any result;
            for( const RuleMap* rules : matchingQueries ) {
              auto it = rules->find( key );

              if( it != rules->end() ) {
                result = it->second;
              }
            }

            return result;
          }

          void NodeStyle::setComputedValue( const std::string& key, stx::any value ) {
            computedRules[ key ] = value;
          }

          void NodeStyle::resetComputedRules() {
            computedRules.clear();
          }

          void NodeStyle::setValue( const std::string& key, stx::any value ) {
            localRules[ key ] = value;
          }

          void NodeStyle::clearMatchingQueries() {
            matchingQueries.clear();
          }

          void NodeStyle::pushMatchingQuery( const RuleMap* ruleMap ) {
            matchingQueries.emplace_back( ruleMap );
          }

        }
      }
    }
  }
}
