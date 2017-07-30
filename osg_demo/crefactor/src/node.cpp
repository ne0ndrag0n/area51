#include "graphics/gui/widget/node.hpp"
#include "device/eventtype/basicevent.hpp"
#include "eventmanager.hpp"
#include <algorithm>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        Node::Node() : signalBank( Signal::SignalBank( this ) ) {}

        std::shared_ptr< Node > Node::getParent() const {
          return parent.lock();
        }

        void Node::setParent( std::shared_ptr< Node > parent ) {
          this->parent = parent;
        }

        std::string Node::getID() const {
          return id;
        }

        void Node::setID( const std::string& newID ) {
          id = newID;

          eventManager.REFLOW_REQUIRED.trigger();
        }

        bool Node::hasClass( const std::string& classID ) const {
          return classes.find( classID ) != classes.end();
        }

        void Node::addClass( const std::string& classID ) {
          classes.insert( classID );

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Node::removeClass( const std::string& classID ) {
          classes.erase( classID );

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Node::clearClasses() {
          classes.clear();

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Node::setStyleValue( const std::string& key, stx::any value ) {
          style.setValue( key, value );
        }

        void Node::clearStyleQueries() {
          style.clearMatchingQueries();
        }

        void Node::pushMatchingQuery( const Style::RuleMap* ruleMap ) {
          style.pushMatchingQuery( ruleMap );
        }

        void Node::setAttributeValue( const std::string& key, stx::any value ) {
          attributes[ key ] = value;
        }

        bool Node::fireSignal( const std::string& signalId, stx::any data ) {
          // Pass-through to SignalBank that can also call node children in override methods
          signalBank.fireSignal( signalId, data );
        }

      }
    }
  }
}
