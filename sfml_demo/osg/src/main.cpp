/* OpenSceneGraph example, osgviewerSFML.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

// (C) 2005 Mike Weiblen http://mew.cx/ released under the OSGPL.
// Simple example using GLUT to create an OpenGL window and OSG for rendering.
// Derived from osgGLUTsimple.cpp and osgkeyboardmouse.cpp

#include <memory>
#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osg/Node>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "graphics/rendering/model.hpp"
#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/sceneview_camera.hpp"
#include "graphics/rendering/group.hpp"
#include "graphics/rendering/types.hpp"

using namespace BlueBear::Graphics::Rendering;

std::shared_ptr< Model > cyl;
std::shared_ptr< Group > myGroup = Group::create();

template<typename W, typename S> bool convertEvent( sf::Event& event, W window, S& sceneView ) {
	switch ( event.type ) {
		case sf::Event::KeyPressed:
			if ( event.key.code == sf::Keyboard::Escape ) {
				window->close();
			}

			if( event.key.code == sf::Keyboard::P ) {
				cyl->playAnimation( "Armature|ArmatureAction.002" );
			}

			if( event.key.code == sf::Keyboard::Num1 ) {
				sceneView.camera->setRotation( 0 );
			}

			if( event.key.code == sf::Keyboard::Num2 ) {
				sceneView.camera->setRotation( 1 );
			}

			if( event.key.code == sf::Keyboard::Num3 ) {
				sceneView.camera->setRotation( 2 );
			}

			if( event.key.code == sf::Keyboard::Num4 ) {
				sceneView.camera->setRotation( 3 );
			}

			if( event.key.code == sf::Keyboard::Left ) {
				sceneView.camera->moveLeft();
			}

			if( event.key.code == sf::Keyboard::Right ) {
				sceneView.camera->moveRight();
			}

			if( event.key.code == sf::Keyboard::Up ) {
				sceneView.camera->moveUp();
			}

			if( event.key.code == sf::Keyboard::Down ) {
				sceneView.camera->moveDown();
			}

			if( event.key.code == sf::Keyboard::Add ) {
				sceneView.camera->zoomIn();
			}

			if( event.key.code == sf::Keyboard::Dash ) {
				sceneView.camera->zoomOut();
			}
			return true;

		case sf::Event::Closed:
			window->close();
			return true;

		default:
			break;
	}

	return false;
}

int main( int argc, char** argv ) {
	std::shared_ptr< sf::RenderWindow > window = std::make_shared< sf::RenderWindow >(
		sf::VideoMode( 1200, 700 ),
		"ne0ndrag0n area51 SFML-OSG evaluation",
		sf::Style::Default,
		sf::ContextSettings( 24, 8, 0, 3, 3 )
	);
	window->setFramerateLimit( 60 );

	// needed?
	// osg::DisplaySettings::instance()->setVertexBufferHint(osg::DisplaySettings::VertexBufferHint::VERTEX_ARRAY_OBJECT);

	SceneView sceneView( 1200, 700 );
	std::shared_ptr< Model > cylinder = Model::create( "mydata/cylinder.fbx" );
	std::shared_ptr< Model > floorPanel = Model::create( "mydata/floorpanel.fbx" );

	Model::Texture polishedHardwood( "mydata/hardwood1.png" );
	polishedHardwood.applyTo( floorPanel, "Plane" );

	{
		std::vector< std::shared_ptr< Model > > models;
		for( double y = -3.5; y != 4.5; y++ ) {
			for( double x = -3.5; x != 4.5; x++ ) {
				std::shared_ptr< Model > m = Model::clone( floorPanel );

				models.emplace_back( m );
				m->setPosition( Vec3( x, y, 0.0 ) );
			}
		}

		cyl = Model::clone( cylinder );
		models.emplace_back( cyl );
		cyl->setPosition( Vec3( 0.5, 0.5, 0.0 ) );

		for( std::shared_ptr< Model > model : models ) {
			myGroup->add( model );
		}
	}

	sceneView.addGroup( myGroup );

	// Suppress all OSG log messages, since at this point it's just spam
	osg::setNotifyLevel( osg::NotifySeverity::ALWAYS );
	while( window->isOpen() ) {
		sf::Event event;

		while ( window->pollEvent( event ) ) {
			// pass the SFML event into the viewers event queue
			convertEvent( event, window, sceneView );
		}

		sceneView.update();
		window->display();
	}

	return EXIT_SUCCESS;
}
