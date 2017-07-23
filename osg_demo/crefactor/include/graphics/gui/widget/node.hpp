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
          std::string id;
          std::unordered_set< std::string > classes;
          std::shared_ptr< Drawable > drawable;
          std::weak_ptr< Node > parent;

          Node();

          virtual std::shared_ptr< Drawable > getOrCreateDrawable() = 0;

        public:
          Style::NodeStyle style;

          virtual ~Node() = default;

          std::shared_ptr< Drawable > getDrawable() const;

          std::shared_ptr< Node > getParent() const;
          void setParent( std::shared_ptr< Node > parent );

          std::string getID() const;
          void setID( const std::string& newID );

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
