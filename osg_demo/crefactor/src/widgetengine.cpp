#include "graphics/gui/widget/widgetengine.hpp"
#include "graphics/gui/widget/node.hpp"
#include "graphics/gui/widget/container.hpp"
#include "graphics/gui/widget/rootcontainer.hpp"
#include "graphics/gui/overlay.hpp"
#include "device/display.hpp"
#include "device/input.hpp"
#include "device/eventtype/mouse.hpp"
#include "containers/color.hpp"
#include "eventmanager.hpp"
#include <functional>
#include <vector>
#include <any>
#include <regex>
#include <algorithm>
#include <deque>
#include <string.h>
#include <cstring>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        WidgetEngine::WidgetEngine( const Device::Display& display, Device::Input& input, Overlay& overlay ) : display( display ), input( input ), overlay( overlay ) {
          root = RootContainer::create();

          buildDefaultStylesheet();

          eventManager.REFLOW_REQUIRED.listen( this, std::bind( &WidgetEngine::refreshStylesheet, this ) );
        }

        WidgetEngine::~WidgetEngine () {
          eventManager.REFLOW_REQUIRED.stopListening( this );
        }

        /**
         * Build a default stylesheet as per spec in https://github.com/ne0ndrag0n/Concordia/wiki/Diamond-Markup-Engine
         */
        void WidgetEngine::buildDefaultStylesheet() {
          styleSheet.emplace_back(
            WidgetEngine::StylesheetQuery{
              "*",
              Style::RuleMap{
                { "z-order", 0 },
                { "visibility", "visible" },
                { "dark-primary-color", Containers::Color< unsigned char >( "#00796BFF" ) },
                { "primary-color", Containers::Color< unsigned char >( "#009688FF" ) },
                { "light-primary-color", Containers::Color< unsigned char >( "#B2DFDBFF" ) },
                { "accent-color", Containers::Color< unsigned char >( "#8BC34AFF" ) },
                { "primary-text-color", Containers::Color< unsigned char >( "#212121FF" ) },
                { "secondary-text-color", Containers::Color< unsigned char >( "#757575FF" ) },
                { "default-text-color", Containers::Color< unsigned char >( "#FFFFFFFF" ) },
                { "icon-color", Containers::Color< unsigned char >( "#FFFFFFFF" ) },
                { "divider-color", Containers::Color< unsigned char >( "#BDBDBDFF" ) },
                { "fill-color", Containers::Color< unsigned char >( "#FAFAFAFF" ) },
                { "font", "default" },
                { "flow", "ltr" },
                { "left", 0 },
                { "top", 0 },
                { "width", 1.0 },
                { "height", 1.0 },
                { "margin", 0 },
                { "padding", 5 },
                { "vertical-align", "middle" },
                { "horizontal-align", "middle" }
              }
            }
          );

          // Root element should still have all the same style shit
          root->pushMatchingQuery( &styleSheet[ 0 ].rules );

          // root element has a flow of "none", allowing window elements to float around (and for GUI elements to be placed arbitrarily)
          // root element is intended to be inaccessible from Lua, etc
          root->setStyleValue( "flow", "none" );
          root->setStyleValue( "width", ( double ) display.getWidth() );
          root->setStyleValue( "height", ( double ) display.getHeight() );
        }

        /**
         * Re-do the style on all widgets.
         */
        void WidgetEngine::refreshStylesheet() {
          // Expected behaviour here is get every node that's in the tree
          std::vector< std::shared_ptr< Node > > allNodes = root->getByName( "*" );
          // Clear ALL matching queries
          for( std::shared_ptr< Node > node : allNodes ) {
            node->clearStyleQueries();
          }

          // Match each stylesheet query's rules to the nodes its query matches
          for( const StylesheetQuery& sQuery : styleSheet ) {
            std::set< std::shared_ptr< Node > > matches = { root };

            // Tokenise the selector with some ass-ugly C stuff
            std::vector< std::string > tokens;
            char* copy = strdup( sQuery.query.c_str() );
            char* token = std::strtok( copy, " \t" );
            while( token != NULL ) {
                tokens.emplace_back( token );
                token = std::strtok( NULL, " \t" );
            }
            free( copy );

            // "selector" = an entire query ("div#id.class .class2")
            // "selector component" = a part of that query separated by whitespace ("div#id.class", or ".class2")
            // Refine the list of matches by taking the previous matches and applying the current selector component to that set of nodes
            for( const std::string& selectorComponent : tokens ) {
              matches = selectItems( toSelector( selectorComponent ), matches );
            }

            // In the list of all matches for this selector, spray sQuery.rules across them
            for( std::shared_ptr< Node > match : matches ) {
              match->pushMatchingQuery( &sQuery.rules );
            }
          }
        }

        std::set< std::shared_ptr< Node > > WidgetEngine::selectItems( const Selector& selector, const std::set< std::shared_ptr< Node > > from ) {
          std::set< std::shared_ptr< Node > > result;

          bool tagPresent = !selector.tag.empty();
          bool idPresent = !selector.id.empty();
          bool classPresent = !selector.classes.empty();

          // Use list of tokens to get list of nodes that match selector
          for( std::shared_ptr< Node > node : from ) {
            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( node ) ) {

              if( idPresent ) {
                // Find the ID and see if it has the associated tag and class
                std::shared_ptr< Node > idNode = container->getByID( selector.id );
                if( idNode ) {
                  // - idNode must have all classes associated with this selector
                  bool hasAllClasses = true;
                  for( const std::string& classID : selector.classes ) {
                    if( idNode->hasClass( classID ) ) {
                      hasAllClasses = false;
                      break;
                    }
                  }

                  // - idNode must have tag name
                  if( hasAllClasses && ( !tagPresent || idNode->getName() == selector.tag ) ) {
                    // Ain't nothing else gonna match this selector, since IDs are specced to be unique. There's not supposed to be another element with this same ID, we found it.
                    // There's no point in continuing to search. Bail out.
                    return { idNode };
                  }
                }
              } else if ( tagPresent && classPresent ) {
                std::vector< std::shared_ptr< Node > > tagMatches = container->getByName( selector.tag );
                std::vector< std::shared_ptr< Node > > classMatches = container->getByClass( selector.classes );

                std::sort( tagMatches.begin(), tagMatches.end() );
                std::sort( classMatches.begin(), classMatches.end() );

                std::set_intersection(
                  tagMatches.begin(), tagMatches.end(),
                  classMatches.begin(), classMatches.end(),
                  std::inserter( result, result.begin() )
                );
              } else {

                // id is not specified and only tag or class (or neither) is specified
                if( tagPresent ) {
                  std::vector< std::shared_ptr< Node > > tagMatches = container->getByName( selector.tag );
                  result.insert( tagMatches.begin(), tagMatches.end() );
                }

                if( classPresent ) {
                  std::vector< std::shared_ptr< Node > > classMatches = container->getByClass( selector.classes );
                  result.insert( classMatches.begin(), classMatches.end() );
                }

              }

            }
          }

          return result;
        }

        /**
         * Some regexes courtesy of Sizzle
         */
        WidgetEngine::Selector WidgetEngine::toSelector( const std::string& token ) {
          static const std::string IDENTIFIER = "(?:\\\\.|[\\w-])+";
          static const std::regex TAG( "^(" + IDENTIFIER + "|[*])", std::regex::ECMAScript );
          static const std::regex ID( "#(" + IDENTIFIER + ")", std::regex::ECMAScript );
          static const std::regex CLASS( "\\.(" + IDENTIFIER + ")", std::regex::ECMAScript );

          std::string tag;
          std::string id;
          std::vector< std::string > classes;

          std::smatch matches;

          if( std::regex_search( token, matches, TAG ) ) {
            tag = matches[ 1 ].str();
          }

          if( std::regex_search( token, matches, ID ) ) {
            id = matches[ 1 ].str();
          }

          std::sregex_iterator it( token.begin(), token.end(), CLASS );
          std::sregex_iterator end;
          while ( it != end ) {
            for( unsigned i = 1; i < it->size(); i += 2 ) {
              classes.emplace_back( ( *it )[ i ] );
            }

            ++it;
          }

          return WidgetEngine::Selector{ tag, id, classes };
        }

        /**
         * Remanage the order of all the drawables
         */
        void WidgetEngine::update() {

          checkInputDevice();

          int maximum = -1;

          std::vector< std::shared_ptr< Drawable > > drawables;
          zTraverse( root, maximum, drawables );

          // Add drawables to overlay
          overlay.setDrawables( drawables );
        }

        /**
         * Process events here
         */
        void WidgetEngine::checkInputDevice() {
          if( input.frameMouseDown ) {
            bool eat = root->fireSignal( "mousedown", *input.frameMouseDown );

            if( eat ) {
              std::cout << "Eating the event" << std::endl;
              input.frameMouseDown = nullptr;
            }
          }
        }

        void WidgetEngine::zTraverse( std::shared_ptr< Node > node, int& globalMaximum, std::vector< std::shared_ptr< Drawable > >& drawables ) {
          int lastChildZ = 0;

          if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( node ) ) {
            std::deque< std::shared_ptr< Node > >& children = container->getChildren();

            if( !children.empty() ) {
              int localMaximum = ++globalMaximum;

              for( std::shared_ptr< Node > child : children ) {
                int childZ = child->getStyleValue< int >( "z-order" );

                if( childZ > lastChildZ ) {
                  lastChildZ = childZ;
                  localMaximum = ++globalMaximum;
                }

                if( std::shared_ptr< Drawable > drawable = child->getOrCreateDrawable() ) {
                  drawables.emplace_back( drawable );
                  drawable->zOrder = localMaximum;
                }

                zTraverse( child, globalMaximum, drawables );
              }
            }

          }
        }

        void WidgetEngine::append( std::shared_ptr< Node > node ) {
          auto& children = root->getChildren();

          if( !children.empty() ) {
            node->setStyleValue( "z-order", ( int ) children.back()->getStyleValue< int >( "z-order" ) + 1 );
          }

          root->append( node );
        }

        void WidgetEngine::prepend( std::shared_ptr< Node > node ) {
          root->prepend( node );
        }

        void WidgetEngine::remove( std::shared_ptr< Node > node ) {
          root->detach( node );
        }

      }
    }
  }
}
