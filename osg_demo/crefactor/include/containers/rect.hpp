#ifndef CONTAINERS_RECT
#define CONTAINERS_RECT

namespace BlueBear {
  namespace Containers {

    template < typename T >
    struct Rect {
      T x;
      T y;
      T width;
      T height;

      bool pointWithin( T pointX, T pointY ) {
        return
          ( pointX >= x ) &&
          ( pointY >= y ) &&
          ( pointX <= x + width ) &&
          ( pointY <= y + height );
      }
    };

  }
}

#endif
