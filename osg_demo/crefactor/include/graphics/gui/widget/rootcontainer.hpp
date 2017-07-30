#ifndef ROOT_WIDGET
#define ROOT_WIDGET

#include "graphics/gui/widget/container.hpp"
#include <string>
#include <memory>
#include <any>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class RootContainer : public Container {

        public:
          bool fireSignal( const std::string& signalId, const stx::any& data ) override;

          std::string getName() const override;

          static std::shared_ptr< RootContainer > create();

        };

      }
    }
  }
}

#endif
