#ifndef CONTAINERS_COLOUR
#define CONTAINERS_COLOUR

namespace BlueBear {
  namespace Containers {

    template < typename T >
    struct Color {
      T r;
      T g;
      T b;
      T a;
    };

  }
}

#endif
