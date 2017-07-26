#ifndef WIDGET_WINDOW
#define WIDGET_WINDOW

#include "graphics/gui/widget/container.hpp"
#include "containers/rect.hpp"
#include "containers/color.hpp"
#include <memory>
#include <string>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class Window : public Container {
        protected:

          Window( const std::string& title );

        public:
          static std::shared_ptr< Window > create( const std::string& title );

          virtual std::shared_ptr< Drawable > getOrCreateDrawable();

        };

      }
    }
  }
}

#endif
