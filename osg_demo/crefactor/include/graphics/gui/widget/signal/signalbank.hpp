#ifndef SIGNAL_BANK
#define SIGNAL_BANK

#include "graphics/gui/widget/signal/mousesignal.hpp"
#include <map>
#include <string>
#include <any>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {
        class Node;

        namespace Signal {

          class SignalBank {
            std::map< std::string, MouseSignal > mouseSignals;
            // std::map< std::string, KeySignal > keySignals;
            // std::map< std::string, MiscSignal > miscSignals;

            // Neither a Signal nor a SignalBank should ever exist independently of a Node
            // This is fine
            Node* parent;

          public:
            SignalBank( Node* parent );

            MouseSignal& getMouseSignal( const std::string& signalId );
            bool fireSignal( const std::string& signalId, const stx::any& data );
          };

        }
      }
    }
  }
}


#endif
