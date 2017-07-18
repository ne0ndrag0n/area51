#ifndef DISPLAY_DEVICE
#define DISPLAY_DEVICE

#include "device/types.hpp"
#include <osg/ref_ptr>
#include <osg/GraphicsContext>
#include <string>
#include <exception>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      class Overlay;
    }
  }

  namespace Device {

    class Display {
    private:
      ContextHelper contextHelper;
      unsigned int width;
      unsigned int height;
      std::string windowTitle;

    public:
      Display( unsigned int width, unsigned int height );
      ~Display();

    public:
      ContextHelper getContextHelper() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;

      struct FailedContextException : public std::exception {

        const char* what() const throw() {
          return "Failed to create graphics context!";
        }

      };
    };

  }
}


#endif
