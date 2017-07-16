#include "device/input.hpp"
#include <iostream>

namespace BlueBear {
  namespace Device {

    Input::InternalAdapter::InternalAdapter( Input& input ) : input( input ) {}

    bool Input::InternalAdapter::handle(
      const osgGA::GUIEventAdapter& eventAdapter,
      osgGA::GUIActionAdapter& actionAdapter
    ) {
      switch( eventAdapter.getEventType() ) {
        case osgGA::GUIEventAdapter::KEYDOWN: {
          Key key = ( Key ) eventAdapter.getKey();

          auto it = input.keyEventTypes.keyDown.find( Key::ANY );
          if( it != input.keyEventTypes.keyDown.end() ) {
            input.triggerKeyboardEvents( key, it->second );
          }

          it = input.keyEventTypes.keyDown.find( key );
          if( it != input.keyEventTypes.keyDown.end() ) {
            input.triggerKeyboardEvents( key, it->second );
          }
          break;
        }
        case osgGA::GUIEventAdapter::KEYUP: {
          Key key = ( Key ) eventAdapter.getKey();

          auto it = input.keyEventTypes.keyUp.find( Key::ANY );
          if( it != input.keyEventTypes.keyUp.end() ) {
            input.triggerKeyboardEvents( key, it->second );
          }

          it = input.keyEventTypes.keyUp.find( key );
          if( it != input.keyEventTypes.keyUp.end() ) {
            input.triggerKeyboardEvents( key, it->second );
          }
          break;
        }
        case osgGA::GUIEventAdapter::PUSH: {
          break;
        }
        case osgGA::GUIEventAdapter::RELEASE: {
          break;
        }
        default:
          break;
      }
    }

    Input::Input() {
      inputAdapter = new InternalAdapter( *this );
    }

    void Input::triggerKeyboardEvents( Key key, std::vector< std::function< void( Key ) > >& list ) {
      for( std::function< void( Key ) > callback : list ) {
        if( callback ) {
          callback( key );
        }
      }
    }

    Input::Adapter Input::getAdapter() const {
      return inputAdapter;
    }

    unsigned int Input::registerKeyEvent(
      Key key,
      std::function< void( Key ) > callback,
      std::map< Key, std::vector< std::function< void( Key ) > > >& eventType
    ) {
      return addItem< std::function< void( Key ) > >( callback, eventType[ key ] );
    }

    void Input::unregisterKeyEvent(
      Key key,
      unsigned int handle,
      std::map< Key, std::vector< std::function< void( Key ) > > >& eventType
    ) {
      auto it = eventType.find( key );

      if( it != eventType.end() ) {
        it->second[ handle ] = std::function< void( Key ) >();
      }
    }

    unsigned int Input::registerKeyDownEvent( Key key, std::function< void( Key ) > callback ) {
      return registerKeyEvent( key, callback, keyEventTypes.keyDown );
    }

    unsigned int Input::registerKeyUpEvent( Key key, std::function< void( Key ) > callback ) {
      return registerKeyEvent( key, callback, keyEventTypes.keyUp );
    }

    void Input::unregisterKeyDownEvent( Key key, unsigned int handle ) {
      unregisterKeyEvent( key, handle, keyEventTypes.keyDown );
    }

    void Input::unregisterKeyUpEvent( Key key, unsigned int handle ) {
      unregisterKeyEvent( key, handle, keyEventTypes.keyUp );
    }

  }
}
