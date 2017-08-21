#include "graphics/gui/widget/container_positioner.hpp"
#include <algorithm>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        Container::Positioner::Positioner( Container& parent ) : parent( parent ) {
          std::copy_if( parent.children.begin(), parent.children.end(), std::back_inserter( flowNodes ), []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "flow";
          } );

          std::copy_if( parent.children.begin(), parent.children.end(), std::back_inserter( overlayNodes ), []( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( "position" ) ) == "overlay";
          } );

          flowRegion = parent.getFlowPositionRegion();

          int padding = parent.getStyle().getValue< int >( "padding" );
          flowRegion.x += padding;
          flowRegion.y += padding;
          flowRegion.width -= padding;
          flowRegion.height -= padding;
        }

        void Container::Positioner::buildAutoNumericPairs( const std::string& dimension ) {
          autoNodes.clear();
          numericNodes.clear();

          std::copy_if( flowNodes.begin(), flowNodes.end(), std::back_inserter( autoNodes ), [ & ]( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( dimension ) ) == "auto";
          } );

          std::copy_if( flowNodes.begin(), flowNodes.end(), std::back_inserter( numericNodes ), [ & ]( std::shared_ptr< Node > child ) {
            return std::string( child->getStyle().getValue< const char* >( dimension ) ) != "auto";
          } );
        }

        void Container::Positioner::setFull( const std::string& dimension, double amount ) {
          for( std::shared_ptr< Node > node : flowNodes ) {
            node->getStyle().setComputedValue( dimension, amount );
          }
        }

        void Container::Positioner::positionLTR() {
          buildAutoNumericPairs( "width" );
          setFull( "height", flowRegion.height );

          int availableSpace = flowRegion.width;

          for( std::shared_ptr< Node > node : numericNodes ) {
            double width = node->getStyle().getValue< double >( "width" );

            if( width <= 1.0 ) {
              // Interpret as percentage
              double actual = availableSpace * width;
              availableSpace -= actual;

              node->getStyle().setComputedValue( "width", actual );
            } else {
              // Interpret as px
              availableSpace -= width;
            }
          }

          // Split remaining space between auto nodes
          if( autoNodes.size() ) {
            double segmentedAvailableSpace = availableSpace / autoNodes.size();
            for( std::shared_ptr< Node > node : autoNodes ) {
              node->getStyle().setComputedValue( "width", segmentedAvailableSpace );
            }
          }

        }
      }
    }
  }
}
