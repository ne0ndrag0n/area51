#include "graphics/gui/widget/window.hpp"
#include "graphics/gui/drawables/window.hpp"
#include "graphics/gui/widget/container.hpp"
#include "containers/rect.hpp"
#include <utility>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        Window::Window( const std::string& title ) : Container::Container() {
          setAttributeValue( "title", title );
        }

        std::shared_ptr< Window > Window::create( const std::string& title ) {
          return std::shared_ptr< Window >( new Window( title ) );
        }

        std::shared_ptr< Drawable > Window::getOrCreateDrawable() {
          if( !drawable ) {
            return drawable = std::make_shared< Drawables::Window >( shared_from_this() );
          } else {
            return drawable;
          }
        }

        std::string Window::getName() const {
          return "Window";
        }

      }
    }
  }
}
