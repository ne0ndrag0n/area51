#ifndef EVENT_OBJECT
#define EVENT_OBJECT

#include "containers/rect.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        struct Event {
          Containers::Rect< unsigned int > mouseStatus;
        };

      }
    }
  }
}

#endif
