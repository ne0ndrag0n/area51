#ifndef TOMBSTONE_COUNTER
#define TOMBSTONE_COUNTER

#include <set>
#include <vector>

namespace BlueBear {
  namespace Tools {

    class TombstoneCounter {
      std::set< unsigned int > usedValues;

    public:
      TombstoneCounter() = default;
      TombstoneCounter( std::vector< unsigned int > preUse );

      void use( unsigned int value );
      void free( unsigned int value );

      unsigned int getNextValue();
    };

  }
}

#endif
