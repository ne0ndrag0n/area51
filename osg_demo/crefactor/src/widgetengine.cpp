#include "graphics/gui/widget/widgetengine.hpp"
#include "graphics/gui/widget/widgetengine_adapter.hpp"
#include "graphics/gui/widget/signal/callbackvectorsignal.hpp"
#include "graphics/gui/widget/node.hpp"
#include "graphics/gui/widget/container.hpp"
#include "graphics/gui/widget/rootcontainer.hpp"
#include "graphics/gui/widget/window.hpp"
#include "graphics/gui/drawable.hpp"
#include "device/display.hpp"
#include "device/input.hpp"
#include "device/eventtype/mouse.hpp"
#include "containers/color.hpp"
#include "containers/rect.hpp"
#include "eventmanager.hpp"
#include <osg/Geode>
#include <osg/Camera>
#include <osg/StateAttribute>
#include <osg/StateSet>
#include <osg/Matrix>
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

        WidgetEngine::WidgetEngine( const Device::Display& display, Device::Input& input ) : display( display ), input( input ) {
          prepareOverlay();

          root = RootContainer::create();

          buildDefaultStylesheet();

          eventManager.REFLOW_REQUIRED.listen( this, std::bind( &WidgetEngine::refreshStylesheet, this ) );
        }

        WidgetEngine::~WidgetEngine () {
          eventManager.REFLOW_REQUIRED.stopListening( this );
        }

        void WidgetEngine::prepareOverlay() {
          osg::ref_ptr< Adapter > adapter = new Adapter( *this );

          osg::ref_ptr< osg::Geode > geode = new osg::Geode();
          geode->setCullingActive( false );
          geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
          geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
          geode->addDrawable( adapter );

          overlay = new osg::Camera;
          overlay->setClearMask( GL_DEPTH_BUFFER_BIT );
          overlay->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
          overlay->setRenderOrder( osg::Camera::POST_RENDER );
          overlay->setAllowEventFocus( false );
          overlay->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
          overlay->addChild( geode );
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
                { "position", "flow" },
                { "left", 0 },
                { "top", 0 },
                { "width", 1.0 },
                { "height", 1.0 },
                { "padding", 5 },
                { "vertical-align", "middle" },
                { "horizontal-align", "middle" }
              }
            }
          );

          // Root element should still have all the same style shit
          root->getStyle().pushMatchingQuery( &styleSheet[ 0 ].rules );

          // root element has a flow of "none", allowing window elements to float around (and for GUI elements to be placed arbitrarily)
          // root element is intended to be inaccessible from Lua, etc
          root->getStyle().setValue( "flow", "none" );
          root->getStyle().setValue( "width", ( double ) display.getWidth() );
          root->getStyle().setValue( "height", ( double ) display.getHeight() );
        }

        /**
         * Re-do the style on all widgets.
         */
        void WidgetEngine::refreshStylesheet() {
          // Expected behaviour here is get every node that's in the tree
          std::vector< std::shared_ptr< Node > > allNodes = root->getByName( "*" );
          // Clear ALL matching queries
          for( std::shared_ptr< Node > node : allNodes ) {
            node->getStyle().clearMatchingQueries();
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
              match->getStyle().pushMatchingQuery( &sQuery.rules );
            }
          }
        }

        void WidgetEngine::drawUnits( NVGcontext* context ) {
          for( std::shared_ptr< Node > immediateChild : root->getChildren() ) {
            // Draw element top-down so that each child is drawn after its parent
            immediateChild->draw( context );
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
        }

        osg::Vec2i WidgetEngine::getAbsolutePosition( std::shared_ptr< Node > node ) {
          osg::Vec2i result;

          int left = node->getStyle().getValue< int >( "left" );
          int top = node->getStyle().getValue< int >( "top" );

          result.set( left, top );

          std::shared_ptr< Node > current = node->getParent();
          while( current ) {
            result.set(
              result.x() + current->getStyle().getValue< int >( "left" ),
              result.y() + current->getStyle().getValue< int >( "top" )
            );

            current = current->getParent();
          }

          return result;
        }

        void WidgetEngine::checkMouseEvent( const std::string& eventId, std::unique_ptr< Device::EventType::Mouse >& data ) {
          if( data ) {

            // Step backwards through rootElement children, and stop at the FIRST mouse event that matches a top-level element
            auto& children = root->getChildren();
            std::shared_ptr< Node > foundElement = nullptr;

            for ( auto it = children.rbegin(); it != children.rend(); ++it ) {
              std::shared_ptr< Node > node = *it;

              // Skip this child if it can't be drawn (don't really know why there'd be a GUI element here without a drawable)
              if( !node->getOrCreateDrawable() ) {
                continue;
              }

              Containers::Rect< int > dimensions{
                node->getStyle().getValue< int >( "left" ),
                node->getStyle().getValue< int >( "top" ),
                ( int ) node->getStyle().getValue< double >( "width" ),
                ( int ) node->getStyle().getValue< double >( "height" )
              };

              if( dimensions.pointWithin( data->x, data->y ) ) {
                // We've found the top-level element!
                foundElement = node;
                break;
              }
            }

            if( foundElement ) {
              std::vector< std::shared_ptr< Node > > eventChain = { foundElement };

              // Go through this element and add any matching children
              if( std::shared_ptr< Container > asContainer = std::dynamic_pointer_cast< Container >( foundElement ) ) {
                std::vector< std::shared_ptr< Node > > children = asContainer->getByPredicate( [ & ]( std::shared_ptr< Node > node ) {
                  osg::Vec2i absolutePosition = getAbsolutePosition( node );

                  Containers::Rect< int > dimensions{
                    absolutePosition.x(),
                    absolutePosition.y(),
                    ( int ) node->getStyle().getValue< double >( "width" ),
                    ( int ) node->getStyle().getValue< double >( "height" )
                  };

                  return dimensions.pointWithin( data->x, data->y );
                } );

                eventChain.insert( eventChain.end(), children.begin(), children.end() );
              }

              // Rip those events!
              for( std::shared_ptr< Node > eventChild : eventChain ) {
                eventChild->signalBank.mouse.at( eventId ).fire( *data );
              }

              // Eat the event
              data.reset();

            }

          }

        }

        /**
         * Process events here
         */
        void WidgetEngine::checkInputDevice() {
          checkMouseEvent( "mousedown", input.frameMouseDown );
          checkMouseEvent( "mouseup", input.frameMouseUp );
        }

        void WidgetEngine::windowDragBegin( std::shared_ptr< Window > target, Device::EventType::Mouse event ) {
          std::cout << "Mousedown in target " << std::hex << target.get() << std::endl;
        }

        void WidgetEngine::windowDragEnd() {

        }

        void WidgetEngine::attachWindowManagerEvents( std::shared_ptr< Window > window ) {
          if( window ) {
            window->signalBank.mouse.at( "mousedown" ).connect( std::bind( &WidgetEngine::windowDragBegin, this, window, std::placeholders::_1 ) );
          }
        }

        void WidgetEngine::append( std::shared_ptr< Node > node ) {
          root->append( node );

          attachWindowManagerEvents( std::dynamic_pointer_cast< Window >( node ) );
        }

        void WidgetEngine::prepend( std::shared_ptr< Node > node ) {
          root->prepend( node );

          attachWindowManagerEvents( std::dynamic_pointer_cast< Window >( node ) );
        }

        void WidgetEngine::remove( std::shared_ptr< Node > node ) {
          root->detach( node );
        }

        OverlayHelper WidgetEngine::getOverlayHelper() const {
          return overlay;
        }

      }
    }
  }
}
