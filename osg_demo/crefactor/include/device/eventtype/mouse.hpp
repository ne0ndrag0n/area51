#ifndef EVENT_TYPE_MOUSE
#define EVENT_TYPE_MOUSE

#include "device/eventtype/basicevent.hpp"

namespace BlueBear {
  namespace Device {
    namespace EventType {

      struct Mouse : public BasicEvent {
        float x;
        float y;
        int buttonMask;

        Mouse( float x, float y, int buttonMask );
      };

    }
  }
}


#endif
