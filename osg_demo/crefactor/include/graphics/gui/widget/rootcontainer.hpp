#ifndef ROOT_WIDGET
#define ROOT_WIDGET

#include "graphics/gui/widget/container.hpp"
#include "graphics/gui/types.hpp"
#include <string>
#include <memory>
#include <any>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class RootContainer : public Container {

        public:
          std::string getName() const override;

          virtual void draw( DrawableContext* context );

          static std::shared_ptr< RootContainer > create();

        };

      }
    }
  }
}

#endif
