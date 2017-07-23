#ifndef DEVICE_TYPES
#define DEVICE_TYPES

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

namespace BlueBear {
  namespace Device {

    using ContextHelper = std::shared_ptr< sf::RenderWindow >;

  }
}

#endif
