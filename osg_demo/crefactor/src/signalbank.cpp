#include "graphics/gui/widget/signal/signalbank.hpp"
#include "graphics/gui/widget/signal/mousesignal.hpp"
#include "device/eventtype/mouse.hpp"
#include <iostream>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        namespace Signal {

          SignalBank::SignalBank( Node* parent ) : parent( parent ) {
            mouseSignals = {
              { "mousedown", MouseSignal( parent ) },
              { "mouseup", MouseSignal( parent ) }
            };
          }

          MouseSignal& SignalBank::getMouseSignal( const std::string& signalId ) {
            return mouseSignals.at( signalId );
          }

          bool SignalBank::fireSignal( const std::string& signalId, const stx::any& data ) {
            {
              auto it = mouseSignals.find( signalId );

              if( it != mouseSignals.end() ) {
                return it->second.fire( stx::any_cast< Device::EventType::Mouse >( data ) );
              }
            }

            // TODO: Warning, do nothing. signalId was an invalid signal across all categories
            return false;
          }

        }
      }
    }
  }
}
