#include "graphics/gui/widget/signal/mousesignal.hpp"
#include "graphics/gui/widget/node.hpp"
#include "containers/rect.hpp"
#include <memory>
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        namespace Signal {

          bool MouseSignal::fire( Device::EventType::Mouse mouse ) {
            // Check if the mouse is within the boundaries of the Node
            Containers::Rect< int > dimensions{
              parent->getStyleValue< int >( "left" ),
              parent->getStyleValue< int >( "top" ),
              ( int ) parent->getStyleValue< double >( "width" ),
              ( int ) parent->getStyleValue< double >( "height" )
            };

            if( dimensions.pointWithin( mouse.x, mouse.y ) ) {
              CallbackVectorSignal< Device::EventType::Mouse >::fire( mouse );
              // Allow event to be eaten so it doesn't bleed through object picking
              return true;
            }

            return false;
          }

        }
      }
    }
  }
}
