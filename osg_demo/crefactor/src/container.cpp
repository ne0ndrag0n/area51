#include "graphics/gui/widget/container.hpp"
#include "graphics/gui/drawable.hpp"
#include "device/eventtype/mouse.hpp"
#include "eventmanager.hpp"
#include <any>
#include <algorithm>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        /**
         * A container alone does not have any drawable (don't add this drawable)
         */
        std::shared_ptr< Drawable > Container::getOrCreateDrawable() {
          drawable = nullptr;

          return drawable;
        }

        std::string Container::getName() const {
          return "Container";
        }

        std::shared_ptr< Node > Container::getByIndex( unsigned int index ) const {
          return children.at( index );
        }

        std::shared_ptr< Node > Container::getByID( const std::string& id ) const {

          for( std::shared_ptr< Node > child : children ) {
            if( child->getID() == id ) {
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

        std::vector< std::shared_ptr< Node > > Container::getByClass( const std::vector< std::string >& classes ) const {
          std::vector< std::shared_ptr< Node > > items;

          for( std::shared_ptr< Node > child : children ) {

            for( const std::string& classId : classes ) {
              if( child->hasClass( classId ) ) {
                items.emplace_back( child );
                break;
              }
            }


            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              std::vector< std::shared_ptr< Node > > childItems = container->getByClass( classes );
              items.insert( items.end(), childItems.begin(), childItems.end() );
            }
          }

          return items;
        }

        std::vector< std::shared_ptr< Node > > Container::getByName( const std::string& name ) const {
          std::vector< std::shared_ptr< Node > > items;

          for( std::shared_ptr< Node > child : children ) {
            if( name == "*" || child->getName() == name ) {
              items.emplace_back( child );
            }

            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              std::vector< std::shared_ptr< Node > > childItems = container->getByName( name );
              items.insert( items.end(), childItems.begin(), childItems.end() );
            }
          }

          return items;
        }

        std::vector< std::shared_ptr< Node > > Container::getByPredicate( std::function< bool( std::shared_ptr< Node > ) > predicate ) const {
          std::vector< std::shared_ptr< Node > > items;

          for( std::shared_ptr< Node > child : children ) {

            if( predicate( child ) ) {
              items.emplace_back( child );
            }

            if( std::shared_ptr< Container > container = std::dynamic_pointer_cast< Container >( child ) ) {
              std::vector< std::shared_ptr< Node > > childItems = container->getByPredicate( predicate );
              items.insert( items.end(), childItems.begin(), childItems.end() );
            }

          }

          return items;
        }

        std::deque< std::shared_ptr< Node > >& Container::getChildren() {
          return children;
        }

        /**
         * Positioning rules so far
         * - RootContainer does not respect overlay vs flow and treats every element as overlay.
         * - This container and most others calculates flow and overlay separately. Overlay elements have higher z-order on their drawables.
         * - ltr, rtl flow: height of children is 100% of the width, minus padding
         * - ttb, btt flow: width of children is 100% of the height, minus padding
         * - if elements cannot be properly calculated, they will not be drawn!
         */
        void Container::positionChildren() {

          // Do absolutely nothing if there's no children
          if( children.empty() ) {
            return;
          }

          std::vector< std::shared_ptr< Node > > flowNodes;
          std::vector< std::shared_ptr< Node > > overlayNodes;

          std::copy_if( children.begin(), children.end(), std::back_inserter( flowNodes ), []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "flow";
          } );

          std::copy_if( children.begin(), children.end(), std::back_inserter( overlayNodes ), []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "overlay";
          } );

          // Position flow nodes
          std::string flowDirection( getStyle().getValue< const char* >( "flow" ) );
          Containers::Rect< int > flowRegion = getFlowPositionRegion();
          int padding = getStyle().getValue< int >( "padding" );

          // Adjust flowRegion width and height by padding
          flowRegion.x += padding;
          flowRegion.y += padding;
          flowRegion.width -= padding;
          flowRegion.height -= padding;

          if( flowDirection == "ltr" || flowDirection == "rtl" ) {

            // TODO

          } else {
            // btt or ttb
          }
        }

        Containers::Rect< int > Container::getFlowPositionRegion() {
          return Containers::Rect< int >{
            getStyle().getValue< int >( "left" ),
            getStyle().getValue< int >( "top" ),
            ( int ) getStyle().getValue< double >( "width" ),
            ( int ) getStyle().getValue< double >( "height" )
          };
        }

        Containers::Rect< int > Container::getOverlayPositionRegion() {
          return Containers::Rect< int >{
            getStyle().getValue< int >( "left" ),
            getStyle().getValue< int >( "top" ),
            ( int ) getStyle().getValue< double >( "width" ),
            ( int ) getStyle().getValue< double >( "height" )
          };
        }

        void Container::prepend( std::shared_ptr< Node > node ) {
          children.insert( children.begin(), node );

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Container::append( std::shared_ptr< Node > node ) {
          children.emplace_back( node );

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Container::detach( std::shared_ptr< Node > node ) {
          children.erase(
            std::remove( children.begin(), children.end(), node ),
            children.end()
          );

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Container::clear() {
          children.clear();

          eventManager.REFLOW_REQUIRED.trigger();
        }

        void Container::draw( DrawableContext* context ) {
          Node::draw( context );

          std::vector< std::shared_ptr< Node > > flowNodes = getByPredicate( []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "flow";
          } );
          std::vector< std::shared_ptr< Node > > overlayNodes = getByPredicate( []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "overlay";
          } );

          for( std::shared_ptr< Node > node : flowNodes ) {
            node->draw( context );
          }

          for( std::shared_ptr< Node > node : overlayNodes ) {
            node->draw( context );
          }
        }

        std::shared_ptr< Container > Container::create() {
          return std::shared_ptr< Container >( new Container() );
        }

      }
    }
  }
}
