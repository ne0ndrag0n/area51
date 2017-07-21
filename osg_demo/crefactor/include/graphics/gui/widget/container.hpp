#ifndef WIDGET_CONTAINER
#define WIDGET_CONTAINER

#include "graphics/gui/widget/node.hpp"
#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <functional>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class Container : public Node {
        protected:
          std::deque< std::shared_ptr< Node > > children;

          virtual void createDrawable() override;

        public:
          void forEach( std::function< void( std::shared_ptr< Node > ) > predicate ) const;

          unsigned int getNumChildren() const;

          std::shared_ptr< Node > getByIndex( unsigned int index ) const;
          std::shared_ptr< Node > getByID( const std::string& id ) const;
          std::vector< std::shared_ptr< Node > > getByClass( const std::string& classId ) const;
          std::vector< std::shared_ptr< Node > > getByName( const std::string& name ) const;

          void prepend( std::shared_ptr< Node > node );
          void append( std::shared_ptr< Node > node );
          void remove( std::shared_ptr< Node > node );
          void clear();

          std::string getName() const;
        };

      }
    }
  }
}



#endif
