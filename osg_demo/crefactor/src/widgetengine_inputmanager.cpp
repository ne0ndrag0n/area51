#include "graphics/gui/widget/widgetengine_inputmanager.hpp"
#include "device/input.hpp"
#include "device/eventtype/mouse.hpp"
#include "graphics/gui/widget/node.hpp"
#include "graphics/gui/widget/container.hpp"
#include "graphics/gui/widget/rootcontainer.hpp"
#include "graphics/gui/widget/window.hpp"
#include "containers/rect.hpp"
#include <vector>
#include <iostream>
#include <functional>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        WidgetEngine::InputManager::InputManager( WidgetEngine& parent ) : parent( parent ) {}

        osg::Vec2i WidgetEngine::InputManager::getAbsolutePosition( std::shared_ptr< Node > node ) {
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

        void WidgetEngine::InputManager::checkMouseEvent( const std::string& eventId, std::unique_ptr< Device::EventType::Mouse >& data ) {
          if( data ) {

            // Step backwards through rootElement children, and stop at the FIRST mouse event that matches a top-level element
            auto& children = parent.root->getChildren();
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

        void WidgetEngine::InputManager::update() {
          checkMouseEvent( "mousedown", parent.input.frameMouseDown );
          checkMouseEvent( "mouseup", parent.input.frameMouseUp );
        }

        void WidgetEngine::InputManager::attachWindowManagerEvents( std::shared_ptr< Window > window ) {
          if( window ) {
            window->signalBank.mouse.at( "mousedown" ).connect( std::bind( &WidgetEngine::InputManager::windowDragBegin, this, window, std::placeholders::_1 ) );
          }
        }

        void WidgetEngine::InputManager::windowDragBegin( std::shared_ptr< Window > target, Device::EventType::Mouse event ) {
          std::cout << "Mousedown in target " << std::hex << target.get() << std::endl;
        }

        void WidgetEngine::InputManager::windowDragEnd() {

        }

      }
    }
  }
}
