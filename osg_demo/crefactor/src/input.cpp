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
          break;
        }
        case osgGA::GUIEventAdapter::KEYUP: {
          break;
        }
        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG: {
          input.frameMouseMove = std::make_unique< EventType::Mouse >( eventAdapter.getX(), ( eventAdapter.getWindowHeight() - eventAdapter.getY() ), eventAdapter.getButtonMask() );
          break;
        }
        case osgGA::GUIEventAdapter::PUSH: {
          input.frameMouseDown = std::make_unique< EventType::Mouse >( eventAdapter.getX(), ( eventAdapter.getWindowHeight() - eventAdapter.getY() ), eventAdapter.getButtonMask() );
          break;
        }
        case osgGA::GUIEventAdapter::RELEASE: {
          input.frameMouseUp = std::make_unique< EventType::Mouse >( eventAdapter.getX(), ( eventAdapter.getWindowHeight() - eventAdapter.getY() ), eventAdapter.getButtonMask() );
          break;
        }
        default:
          break;
      }
    }

    Input::Input() {
      inputAdapter = new InternalAdapter( *this );
    }

    Input::Adapter Input::getAdapter() const {
      return inputAdapter;
    }

  }
}
