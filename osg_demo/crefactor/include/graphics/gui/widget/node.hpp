#ifndef WIDGET_NODE
#define WIDGET_NODE

#include "graphics/gui/widget/style/types.hpp"
#include "graphics/gui/widget/style/nodestyle.hpp"
#include <memory>
#include <string>
#include <map>
#include <any>
#include <unordered_set>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      class Drawable;

      namespace Widget {

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

          virtual ~Node() = default;
          virtual std::shared_ptr< Drawable > getOrCreateDrawable() = 0;

          template< typename T >
          T getStyleValue( const std::string& key ) const {
            return stx::any_cast< T >( style.getValue( key ) );
          };
          virtual void setStyleValue( const std::string& key, stx::any value );

          template< typename T >
          T getAttributeValue( const std::string& key ) const {
            return stx::any_cast< T >( attributes.at( key ) );
          };
          void setAttributeValue( const std::string& key, stx::any value );

          void clearStyleQueries();
          void pushMatchingQuery( const Style::RuleMap* ruleMap );

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
