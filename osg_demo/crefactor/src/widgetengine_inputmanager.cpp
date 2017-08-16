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
#include <algorithm>

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

            std::vector< std::shared_ptr< Node > > eventChain;

            // If any items are attached to the root itself, fire those off too
            // Don't bother checking root for boundaries; it's the entire screen!
            if( parent.root->signalBank.mouse.find( eventId ) != parent.root->signalBank.mouse.end() ) {
              eventChain.emplace_back( parent.root );
            }

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
              eventChain.emplace_back( foundElement );

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
            }

            // Rip those events!
            for( std::shared_ptr< Node > eventChild : eventChain ) {
              eventChild->signalBank.mouse.at( eventId ).fire( *data );
            }

            // Eat the event
            if( eventChain.size() ) {
              data.reset();
            }

          }
        }

        void WidgetEngine::InputManager::onWindowDrag( std::shared_ptr< Window > target, int diffX, int diffY, Device::EventType::Mouse event ) {
          int newLeft = event.x - diffX;
          int newTop = event.y - diffY;

          target->getStyle().setValue( "left", newLeft );
          target->getStyle().setValue( "top", newTop );
        }

        void WidgetEngine::InputManager::update() {
          checkMouseEvent( "mousedown", parent.input.frameMouseDown );
          checkMouseEvent( "mouseup", parent.input.frameMouseUp );
          checkMouseEvent( "mousemove", parent.input.frameMouseMove );
        }

        void WidgetEngine::InputManager::attachWindowManagerEvents( std::shared_ptr< Window > window ) {
          if( window ) {
            window->signalBank.mouse.at( "mousedown" ).connect( std::bind( &WidgetEngine::InputManager::windowDragBegin, this, window, std::placeholders::_1 ) );
          }
        }

        void WidgetEngine::InputManager::windowDragBegin( std::shared_ptr< Window > target, Device::EventType::Mouse event ) {
          int left = target->getStyle().getValue< int >( "left" );
          int top = target->getStyle().getValue< int >( "top" );

          Containers::Rect< int > titlebar{ left, top, ( int ) target->getStyle().getValue< double >( "width" ), 20 };

          if( titlebar.pointWithin( event.x, event.y ) ) {
            // Mousedown in titlebar
            // Attach the mousemove event and the mouseup event
            DragEventPair eventPair;

            eventPair.moveEvent = parent.root->signalBank.mouse.at( "mousemove" ).connect(
              std::bind( &WidgetEngine::InputManager::onWindowDrag, this, target, event.x - left, event.y - top, std::placeholders::_1 )
            );

            eventPair.endEvent = parent.root->signalBank.mouse.at( "mouseup" ).connect(
              std::bind( &WidgetEngine::InputManager::windowDragEnd, this, std::placeholders::_1 )
            );

            lockedEventPair = eventPair;
          }

          auto& children = parent.root->getChildren();
          if( children.size() && *children.rbegin() != target ) {
            children.erase(
              std::remove( children.begin(), children.end(), target ),
              children.end()
            );

            children.emplace_back( target );
          }
        }

        void WidgetEngine::InputManager::windowDragEnd( Device::EventType::Mouse event ) {
          DragEventPair eventPair = stx::any_cast< DragEventPair >( lockedEventPair );

          parent.root->signalBank.mouse.at( "mousemove" ).disconnect( eventPair.moveEvent );
          parent.root->signalBank.mouse.at( "mouseup" ).disconnect( eventPair.endEvent );

          lockedEventPair = stx::any();
        }

      }
    }
  }
}
