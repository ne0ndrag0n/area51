#ifndef WIDGET_NODE
#define WIDGET_NODE

#include "graphics/gui/types.hpp"
#include "graphics/gui/widget/style/types.hpp"
#include "graphics/gui/widget/style/nodestyle.hpp"
// dangerous
#include "graphics/gui/widget/signal/signalbank.hpp"
#include <memory>
#include <string>
#include <map>
#include <any>
#include <unordered_set>

// XXX: Remove. debug stuff
//#include <typeinfo>
//#include <cxxabi.h>
// use abi::__cxa_demangle to demangle names

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      class Drawable;

      namespace Widget {
        namespace Signal {
          class MouseSignal;
        }

        /**
         * Base class for all GUI widgets
         */
        class Node : public std::enable_shared_from_this< Node > {
        protected:
          std::string id;
          std::unordered_set< std::string > classes;

          std::shared_ptr< Drawable > drawable;

          std::weak_ptr< Node > parent;

          Style::NodeStyle style;
          std::map< std::string, stx::any > attributes;

          Node();

        public:

          // Signals live on node but an external manager (such as WidgetEngine) is responsible
          // for triggering them
          Signal::SignalBank signalBank;

          virtual ~Node() = default;
          virtual std::shared_ptr< Drawable > getOrCreateDrawable() = 0;

          template< typename T >
          T getAttributeValue( const std::string& key ) const {
            return stx::any_cast< T >( attributes.at( key ) );
          };
          void setAttributeValue( const std::string& key, stx::any value );

          Style::NodeStyle& getStyle();

          std::shared_ptr< Node > getParent() const;
          void setParent( std::shared_ptr< Node > parent );

          std::string getID() const;
          void setID( const std::string& newID );

          virtual void draw( DrawableContext* context );

          bool hasClass( const std::string& classID ) const;
          void addClass( const std::string& classID );
          void removeClass( const std::string& classID );
          void clearClasses();

          virtual std::string getName() const = 0;
        };

      }
    }
  }
}




#endif
