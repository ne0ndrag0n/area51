#ifndef WIDGET_ENGINE
#define WIDGET_ENGINE

#include <stack>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;

        /**
         * The WidgetEngine sits next to the Overlay and submits drawables to it, ordering each drawable's z-order prior to
         * the re-draw of Overlay.
         */
        class WidgetEngine {
        public:

        private:

        };

      }
    }
  }
}


#endif
