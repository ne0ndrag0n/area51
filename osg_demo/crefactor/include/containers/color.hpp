#ifndef CONTAINERS_COLOUR
#define CONTAINERS_COLOUR

#include <string>
#include <sstream>
#include <iomanip>

namespace BlueBear {
  namespace Containers {

    template < typename T >
    struct Color {
      T r;
      T g;
      T b;
      T a;

      Color( T r, T g, T b, T a ) : r( r ), g( g ), b( b ), a( a ) {}
      Color( const std::string& colString ) {
        setFromString( colString );
      }

      void setFromString( const std::string& colString ) {
        r = std::stoul( colString.substr( 1, 2 ), nullptr, 16 );
        g = std::stoul( colString.substr( 3, 2 ), nullptr, 16 );
        b = std::stoul( colString.substr( 5, 2 ), nullptr, 16 );
        a = std::stoul( colString.substr( 7, 2 ), nullptr, 16 );
      }

      std::string toString() const {
        std::stringstream stream;

        stream
          << "#"
          << std::setw( sizeof( T ) * 2 )
          << std::hex
          << r
          << g
          << b
          << a;

        return stream.str();
      }
    };

  }
}

#endif
