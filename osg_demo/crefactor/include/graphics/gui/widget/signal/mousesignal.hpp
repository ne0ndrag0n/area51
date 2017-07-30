#ifndef MOUSE_SIGNAL
#define MOUSE_SIGNAL

#include "graphics/gui/widget/signal/callbackvectorsignal.hpp"
#include "device/eventtype/mouse.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        namespace Signal {

          class MouseSignal : public CallbackVectorSignal< Device::EventType::Mouse > {

          public:
            using CallbackVectorSignal< Device::EventType::Mouse >::CallbackVectorSignal;

            bool fire( Device::EventType::Mouse mouse ) override;

          };

        }
      }
    }
  }
}

#endif
