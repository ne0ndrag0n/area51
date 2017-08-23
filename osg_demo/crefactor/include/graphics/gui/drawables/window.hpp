#ifndef WINDOW_DRAWABLE
#define WINDOW_DRAWABLE

#include "graphics/gui/drawable.hpp"
#include "containers/color.hpp"
#include <nanovg/nanovg.h>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;
      }

      namespace Drawables {

        struct Window : public Drawable {
        protected:
          std::weak_ptr< Widget::Node > parent;

          NVGcolor toColor( const Containers::Color< unsigned char >& color );

        public:
          Window( std::weak_ptr< Widget::Node > parent );

          virtual void draw( DrawableContext* context ) override;

          void drawBeforeChildren( DrawableContext* context );
          void drawAfterChildren( DrawableContext* context );
        };

      }
    }
  }
}


#endif
