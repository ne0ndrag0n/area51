#ifndef WIDGET_ENGINE
#define WIDGET_ENGINE

#include "graphics/gui/widget/style/types.hpp"
#include <osg/Vec2i>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <tuple>
#include <any>

namespace BlueBear {
  namespace Device {
    namespace EventType {
      class Mouse;
    }

    class Display;
    class Input;
  }

  namespace Graphics {
    namespace GUI {
      class Drawable;
      class Overlay;

      namespace Widget {
        class Node;
        class Container;
        class RootContainer;
        class Window;

        /**
         * The WidgetEngine sits next to the Overlay and submits drawables to it, ordering each drawable's z-order prior to
         * the re-draw of Overlay.
         */
        class WidgetEngine {
          struct Selector {
            std::string tag;
            std::string id;
            std::vector< std::string > classes;
          };

          struct StylesheetQuery {
            std::string query;
            Style::RuleMap rules;
          };

          const Device::Display& display;
          Device::Input& input;
          Overlay& overlay;
          std::vector< StylesheetQuery > styleSheet;
          std::shared_ptr< RootContainer > root;

          void buildDefaultStylesheet();
          void refreshStylesheet();
          Selector toSelector( const std::string& token );
          std::set< std::shared_ptr< Node > > selectItems(
             const Selector& selector,
             const std::set< std::shared_ptr< Node > > from
          );

          void zTraverse(
            std::shared_ptr< Node > node,
            int& globalMaximum,
            std::vector< std::shared_ptr< Drawable > >& drawables
          );
          void checkInputDevice();

          osg::Vec2i getAbsolutePosition( std::shared_ptr< Node > node, int left, int top );

          void windowDragBegin( std::shared_ptr< Window > target, Device::EventType::Mouse event );
          void windowDragEnd();

          void attachWindowManagerEvents( std::shared_ptr< Window > window );
          void checkMouseEvent( const std::string& eventId, std::unique_ptr< Device::EventType::Mouse >& data );

        public:
          WidgetEngine( const Device::Display& display, Device::Input& input, Overlay& overlay );
          ~WidgetEngine();

          void update();

          void append( std::shared_ptr< Node > node );
          void prepend( std::shared_ptr< Node > node );
          void remove( std::shared_ptr< Node > node );
        };

      }
    }
  }
}


#endif
