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
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "graphics/rendering/model.hpp"
#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/group.hpp"
#include "graphics/rendering/light.hpp"

using namespace BlueBear::Graphics::Rendering;

std::vector< Model > models;

template<typename W> bool convertEvent( sf::Event& event, W window ) {
	switch ( event.type ) {
		case sf::Event::KeyPressed:
			if ( event.key.code == sf::Keyboard::Escape ) {
				window->close();
			}

			if( event.key.code == sf::Keyboard::P ) {
				models[ 3 ].playAnimation( "Armature|ArmatureAction.002" );
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
		sf::VideoMode( 800, 600 ),
		"ne0ndrag0n area51 SFML-OSG evaluation",
		sf::Style::Default,
		sf::ContextSettings( 24 )
	);
	window->setVerticalSyncEnabled( true );

	SceneView sceneView( 800, 600 );

	Group myGroup;

	for( int i = 0; i != 3; i++ ) {
		models.emplace_back( "mydata/floorpanel.fbx" );
	}

	models[ 1 ].setPosition( osg::Vec3( 1.0, 0.0, 0.0 ) );
	models[ 2 ].setPosition( osg::Vec3( 0.0, 1.0, 0.0 ) );

	models.emplace_back( "mydata/cylinder.fbx" );

	for( Model& model : models ) {
		myGroup.add( model );
	}

	sceneView.addGroup( myGroup );

	while( window->isOpen() ) {
		sf::Event event;

		while ( window->pollEvent( event ) ) {
			// pass the SFML event into the viewers event queue
			convertEvent( event, window );
		}

		sceneView.update();
		window->display();
	}

	return EXIT_SUCCESS;
}
