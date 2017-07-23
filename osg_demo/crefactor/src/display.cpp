#include "device/display.hpp"
#include "graphics/gui/overlay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace BlueBear {
  namespace Device {

    Display::Display( unsigned int width, unsigned int height ) :
      width( width ), height( height ) {

        /*
        osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20;
        traits->y = 30;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->windowName = "ne0ndrag0n area51 OSG evaluation";
        traits->glContextVersion = "3.3";

        contextHelper = osg::GraphicsContext::createGraphicsContext( traits.get() );
        if( !contextHelper.valid() ) {
          throw FailedContextException();
        }
        */

        contextHelper = std::make_shared< sf::RenderWindow >(
      		sf::VideoMode( width, height ),
      		"ne0ndrag0n area51 SFML-OSG evaluation",
      		sf::Style::Default,
      		sf::ContextSettings( 24, 8, 0, 3, 3, sf::ContextSettings::Core )
      	);

        // needed? for os x?
        // osg::DisplaySettings::instance()->setVertexBufferHint(osg::DisplaySettings::VertexBufferHint::VERTEX_ARRAY_OBJECT);
    }

    Display::~Display() {}

    ContextHelper Display::getContextHelper() const {
      return contextHelper;
    }

    unsigned int Display::getWidth() const {
      return width;
    }

    unsigned int Display::getHeight() const {
      return height;
    }
  }
}
