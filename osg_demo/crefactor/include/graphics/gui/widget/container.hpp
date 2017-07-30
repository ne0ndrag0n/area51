#ifndef WIDGET_CONTAINER
#define WIDGET_CONTAINER

#include "graphics/gui/widget/node.hpp"
#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <any>
#include <functional>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class Container : public Node {
        protected:
          std::deque< std::shared_ptr< Node > > children;

          virtual std::shared_ptr< Drawable > getOrCreateDrawable() override;
          void sort();

          using Node::Node;

        public:
          static std::shared_ptr< Container > create();

          void forEach( std::function< void( std::shared_ptr< Node > ) > predicate ) const;

          unsigned int getNumChildren() const;

          void setStyleValue( const std::string& key, stx::any value ) override;

          std::shared_ptr< Node > getByIndex( unsigned int index ) const;
          std::shared_ptr< Node > getByID( const std::string& id ) const;
          std::vector< std::shared_ptr< Node > > getByClass( const std::vector< std::string >& classes ) const;
          std::vector< std::shared_ptr< Node > > getByName( const std::string& name ) const;

          std::deque< std::shared_ptr< Node > >& getChildren();

          void prepend( std::shared_ptr< Node > node );
          void append( std::shared_ptr< Node > node );
          void detach( std::shared_ptr< Node > node );
          void clear();

          bool fireSignal( const std::string& signalId, const stx::any& data ) override;

          std::string getName() const;
        };

      }
    }
  }
}



#endif
