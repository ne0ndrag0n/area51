#include "graphics/gui/widget/container.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        /**
         * A container alone does not have any drawable (don't add this drawable)
         */
        void Container::createDrawable() {
          drawable = nullptr;
        }

        std::string Container::getName() const {
          return "Container";
        }

        std::shared_ptr< Node > Container::getByIndex( unsigned int index ) const {
          return children.at( index );
        }

        std::shared_ptr< Node > Container::getByID( const std::string& id ) const {

          for( std::shared_ptr< Node > child : children ) {
            if( child->id == id ) {
              return child;
            }

            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              if( std::shared_ptr< Node > result = container->getByID( id ) ) {
                return result;
              }
            }
          }

          return nullptr;
        }

        std::vector< std::shared_ptr< Node > > Container::getByClass( const std::string& classId ) const {
          std::vector< std::shared_ptr< Node > > items;

          for( std::shared_ptr< Node > child : children ) {
            if( child->classes.find( classId ) != child->classes.end() ) {
              items.emplace_back( child );
            }

            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              std::vector< std::shared_ptr< Node > > childItems = container->getByClass( classId );
              items.insert( items.end(), childItems.begin(), childItems.end() );
            }
          }

          return items;
        }

        std::vector< std::shared_ptr< Node > > Container::getByName( const std::string& name ) const {
          std::vector< std::shared_ptr< Node > > items;

          for( std::shared_ptr< Node > child : children ) {
            if( child->getName() == name ) {
              items.emplace_back( child );
            }

            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              std::vector< std::shared_ptr< Node > > childItems = container->getByName( name );
              items.insert( items.end(), childItems.begin(), childItems.end() );
            }
          }

          return items;
        }

      }
    }
  }
}
