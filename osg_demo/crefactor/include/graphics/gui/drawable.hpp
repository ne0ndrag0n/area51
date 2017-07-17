#ifndef BLUEBEAR_DRAWABLE
#define BLUEBEAR_DRAWABLE

namespace BlueBear {
  namespace Graphics {
    namespace GUI {

      /**
       * The Drawable interface is used by the Overlay to represent any type
       * that can be drawn using the "draw()" method.
       */
      struct Drawable {
        virtual ~Drawable() = default;

        unsigned int zOrder = 0;

        bool operator<( const Drawable& other );
        virtual void draw() = 0;
      };

    }
  }
}

#endif
