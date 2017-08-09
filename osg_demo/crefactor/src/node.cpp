#include "graphics/gui/widget/node.hpp"
#include "graphics/gui/drawable.hpp"
#include "eventmanager.hpp"
#include <algorithm>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        Node::Node() {

        }

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

        void Node::setAttributeValue( const std::string& key, stx::any value ) {
          attributes[ key ] = value;
        }

        Style::NodeStyle& Node::getStyle() {
          return style;
        }

        void Node::draw( DrawableContext* context ) {
          if( std::shared_ptr< Drawable > drawable = getOrCreateDrawable() ) {
            drawable->draw( context );
          }
        }

      }
    }
  }
}
