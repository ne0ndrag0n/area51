#include "tools/tombstonecounter.hpp"

namespace BlueBear {
  namespace Tools {

    TombstoneCounter::TombstoneCounter( std::vector< unsigned int > preUse ) {
      for( unsigned int pre : preUse ) {
        use( pre );
      }
    }

    void TombstoneCounter::use( unsigned int value ) {
      usedValues.emplace( value );
    }

    unsigned int TombstoneCounter::getNextValue() {
      unsigned int expected = 0;

      for( unsigned int value : usedValues ) {
        if( value == expected ) {
          expected++;
        } else {
          break;
        }
      }

      usedValues.emplace( expected );
      return expected;
    }

    void TombstoneCounter::free( unsigned int value ) {
      usedValues.erase( value );
    }

  }
}
