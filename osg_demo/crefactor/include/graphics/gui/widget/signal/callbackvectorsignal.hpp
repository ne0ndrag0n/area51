#ifndef CALLBACK_VECTOR_SIGNAL
#define CALLBACK_VECTOR_SIGNAL

#include <vector>
#include <functional>
#include <memory>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;

        namespace Signal {

          template< typename CallbackArg >
          class CallbackVectorSignal {
            std::vector< std::function< void( CallbackArg& ) > > callbacks;
          protected:
            Node* parent;

          public:
            CallbackVectorSignal( Node* parent ) : parent( parent ) {}

            unsigned int connect( std::function< void( CallbackArg& ) > callback ) {
              for( unsigned int i = 0; i != callbacks.size(); i++ ) {
                if( !callbacks[ i ] ) {
                  callbacks[ i ] = callback;
                  return i;
                }
              }

              callbacks.emplace_back( callback );
              return callbacks.size() - 1;
            }

            void disconnect( unsigned int index ) {
              callbacks[ index ] = std::function< void( CallbackArg& ) >();
            }

            virtual bool fire( CallbackArg param ) {
              for( std::function< void( CallbackArg& ) > callback : callbacks ) {
                callback( param );
              }

              return true;
            }

          };

        }
      }
    }
  }
}


#endif
