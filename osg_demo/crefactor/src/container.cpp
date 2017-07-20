#include "graphics/gui/widget/container.hpp"

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        /**
         * A container alone does not have any drawable (don't add this drawable)
         */
        void Container::createDrawable() {
          drawable = nullptr;
        }

      }
    }
  }
}
