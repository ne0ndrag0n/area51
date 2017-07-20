#include "graphics/gui/widget/node.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        Node::Node() {}

        std::shared_ptr< Drawable > Node::getDrawable() const {
          return drawable;
        }

        std::shared_ptr< Node > Node::getParent() const {
          return parent.lock();
        }

        void Node::setParent( std::shared_ptr< Node > parent ) {
          this->parent = parent;
        }

      }
    }
  }
}
