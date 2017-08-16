#ifndef WIDGET_ENGINE_INPUT_MANAGER
#define WIDGET_ENGINE_INPUT_MANAGER

#include "graphics/gui/widget/widgetengine.hpp"
#include <string>
#include <memory>
#include <any>
#include <osg/Vec2i>

namespace BlueBear {
  namespace Device {
    namespace EventType {
      class Mouse;
    }
  }

  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Window;

        class WidgetEngine::InputManager {

          WidgetEngine& parent;

          struct DragEventPair {
            unsigned int moveEvent;
            unsigned int endEvent;
          };

          stx::any lockedEventPair;

          void checkMouseEvent( const std::string& eventId, std::unique_ptr< Device::EventType::Mouse >& data );
          osg::Vec2i getAbsolutePosition( std::shared_ptr< Node > node );

          void windowDragBegin( std::shared_ptr< Window > target, Device::EventType::Mouse event );
          void windowDragEnd( Device::EventType::Mouse event );

          void onWindowDrag( std::shared_ptr< Window > target, int diffX, int diffY, Device::EventType::Mouse event );

        public:
          InputManager( WidgetEngine& parent );

          void update();
          void attachWindowManagerEvents( std::shared_ptr< Window > window );
        };

      }
    }
  }
}


#endif
