#ifndef WIDGET_NODE
#define WIDGET_NODE

#include "graphics/gui/widget/style/types.hpp"
#include "graphics/gui/widget/style/nodestyle.hpp"
#include <memory>
#include <string>
#include <unordered_set>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      class Drawable;

      namespace Widget {

        /**
         * Base class for all GUI widgets
         */
        class Node {
        protected:
          std::shared_ptr< Drawable > drawable;
          std::weak_ptr< Node > parent;

          Node();

          virtual void createDrawable() = 0;

        public:
          std::string id;
          std::unordered_set< std::string > classes;
          Style::NodeStyle style;

          virtual ~Node() = default;

          std::shared_ptr< Drawable > getDrawable() const;

          std::shared_ptr< Node > getParent() const;
          void setParent( std::shared_ptr< Node > parent );

          virtual std::string getName() const = 0;
        };

      }
    }
  }
}




#endif
