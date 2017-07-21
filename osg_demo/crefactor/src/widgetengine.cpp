#include "graphics/gui/widget/widgetengine.hpp"
#include "graphics/gui/widget/node.hpp"
#include "graphics/gui/widget/container.hpp"
#include "tools/utility.hpp"
#include <vector>
#include <regex>
#include <string.h>
#include <cstring>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        WidgetEngine::WidgetEngine() {
          root = std::make_shared< Container >();

          auto test = tokenToTuple( "div#id.ddd.d" );
        }

        /**
         * Re-do the style on all widgets.
         */
        void WidgetEngine::refreshStylesheet() {
          std::vector< std::shared_ptr< Node > > oldMatches = { root };
          std::vector< std::shared_ptr< Node > > newMatches;

          for( auto& pair : styleSheet ) {
            std::vector< std::string > tokens;

            // Tokenise the selector
            char* copy = strdup( pair.first.c_str() );
            char* token = std::strtok( copy, " \t" );
            while( token != NULL ) {
                tokens.emplace_back( token );
                token = std::strtok( NULL, " \t" );
            }
            free( copy );

            for( const std::string& token : tokens ) {
              auto tuple = tokenToTuple( token );


            }
          }
        }

        /**
         * Some regexes courtesy of Sizzle
         */
        std::tuple< std::string, std::string, std::string > WidgetEngine::tokenToTuple( const std::string& token ) {
          static const std::string IDENTIFIER = "(?:\\\\.|[\\w-])+";
          static const std::regex TAG( "^(" + IDENTIFIER + "|[*])", std::regex::ECMAScript );
          static const std::regex ID( "#(" + IDENTIFIER + ")", std::regex::ECMAScript );
          static const std::regex CLASS( "\\.(" + IDENTIFIER + ")", std::regex::ECMAScript );

          std::string tag;
          std::string id;
          std::string clss;

          std::smatch matches;

          if( std::regex_search( token, matches, TAG ) ) {
            tag = matches[ 1 ].str();
            std::cout << tag << std::endl;
          }

          if( std::regex_search( token, matches, ID ) ) {
            id = matches[ 1 ].str();
            std::cout << id << std::endl;
          }

          if( std::regex_search( token, matches, CLASS ) ) {
            clss = matches[ 1 ].str();
            std::cout << clss << std::endl;
          }

          return std::make_tuple( tag, id, clss );
        }

      }
    }
  }
}
