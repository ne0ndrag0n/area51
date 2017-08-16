#ifndef SIGNAL_BANK
#define SIGNAL_BANK

#include "graphics/gui/widget/signal/callbackvectorsignal.hpp"
#include "device/eventtype/mouse.hpp"
#include <map>
#include <string>
#include <memory>
#include <any>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;

        namespace Signal {

          struct SignalBank {
            std::map< std::string, CallbackVectorSignal< Device::EventType::Mouse > > mouse = {
              { "mousedown", CallbackVectorSignal< Device::EventType::Mouse >() },
              { "mouseup", CallbackVectorSignal< Device::EventType::Mouse >() },
              { "mousemove", CallbackVectorSignal< Device::EventType::Mouse >() }
            };
          };

        }
      }
    }
  }
}


#endif
