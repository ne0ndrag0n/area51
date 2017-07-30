#include "device/eventtype/mouse.hpp"

namespace BlueBear {
  namespace Device {
    namespace EventType {

      Mouse::Mouse( float x, float y, int buttonMask ) : x( x ), y( y ), buttonMask( buttonMask ) {}

    }
  }
}
