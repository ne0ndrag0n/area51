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
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

osgViewer::Viewer viewer;

void printCameraTransform() {
	osg::Vec3d eye;
	osg::Vec3d center;
	osg::Vec3d up;

	osg::ref_ptr< osgGA::TrackballManipulator > m = ( osgGA::TrackballManipulator* ) viewer.getCameraManipulator();
	m->getTransformation( eye, center, up );

	std::cout << "Eye: " << eye.x() << " " << eye.y() << " " << eye.z() << std::endl;
	std::cout << "Center: " << center.x() << " " << center.y() << " " << center.z() << std::endl;
	std::cout << "Up: " << up.x() << " " << up.y() << " " << up.z() << std::endl;

}

template<typename G,typename W> bool convertEvent( sf::Event& event, G gw, W window ) {
	auto eventQueue = gw->getEventQueue();

	switch ( event.type ) {
		case sf::Event::MouseMoved:
			eventQueue->mouseMotion( event.mouseMove.x, event.mouseMove.y );
			return true;

		case sf::Event::MouseButtonPressed:
			if( event.mouseButton.button == sf::Mouse::Left ) {
				event.mouseButton.button = static_cast< decltype( event.mouseButton.button ) >( 1 );
			} else if( event.mouseButton.button == sf::Mouse::Right ) {
				event.mouseButton.button = static_cast< decltype( event.mouseButton.button ) >( 3 );
			}
			eventQueue->mouseButtonPress( event.mouseButton.x, event.mouseButton.y, event.mouseButton.button );
			return true;

		case sf::Event::MouseButtonReleased:
			if( event.mouseButton.button == sf::Mouse::Left ) {
				event.mouseButton.button = static_cast< decltype( event.mouseButton.button ) >( 1 );
			} else if( event.mouseButton.button == sf::Mouse::Right ) {
				event.mouseButton.button = static_cast< decltype( event.mouseButton.button ) >( 3 );
			}
			eventQueue->mouseButtonRelease( event.mouseButton.x, event.mouseButton.y, event.mouseButton.button );
			printCameraTransform();
			return true;

		case sf::Event::KeyReleased:
			eventQueue->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) event.key.code);
			return true;

		case sf::Event::KeyPressed:
			eventQueue->keyPress( (osgGA::GUIEventAdapter::KeySymbol) event.key.code);
			if ( event.key.code == sf::Keyboard::Escape ) {
				window->close();
			}
			return true;

		case sf::Event::Resized:
			eventQueue->windowResize(0, 0, event.size.width, event.size.height );
			gw->resized( 0, 0, event.size.width, event.size.height );
			return true;

		case sf::Event::Closed:
			window->close();
			return true;

		default:
			break;
	}

	return false;
}

osg::ref_ptr< osg::Group > getRootNode() {
	osg::ref_ptr< osg::Group > root = new osg::Group();

	osg::ref_ptr< osg::Node > cylinder = osgDB::readRefNodeFile( "mydata/cylinder.fbx" );
	osg::ref_ptr< osg::Node > floor = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );
	osg::ref_ptr< osg::Node > floor2 = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );
	osg::ref_ptr< osg::Node > floor3 = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );

	osg::ref_ptr< osg::PositionAttitudeTransform > pipeTransform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floorTransform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floor2Transform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floor3Transform = new osg::PositionAttitudeTransform();

	root->addChild( floorTransform.get() );
	floorTransform->addChild( floor.get() );
	floorTransform->setPosition( osg::Vec3( 0.0, 0.0, 0.0 ) );

	return root;
}

int main( int argc, char** argv ) {

	int width = 800; int height = 600;

	std::shared_ptr< sf::RenderWindow > window = std::make_shared< sf::RenderWindow >(
		sf::VideoMode( width, height ),
		"ne0ndrag0n area51 SFML-OSG evaluation",
		sf::Style::Default,
		sf::ContextSettings( 24 )
	);
	window->setFramerateLimit( 60 );

	osg::ref_ptr< osgViewer::GraphicsWindowEmbedded > gw = viewer.setUpViewerAsEmbeddedInWindow( 0, 0, width, height );

	osg::ref_ptr< osg::Group > root = getRootNode();
	viewer.setSceneData( root.get() );

	osg::ref_ptr< osg::Camera > camera = viewer.getCamera();
	camera->setClearColor( osg::Vec4( 0.0f, 0.15f, 0.0f, 1.0f ) );

	osg::ref_ptr< osgGA::TrackballManipulator > manipulator = new osgGA::TrackballManipulator;
	viewer.setCameraManipulator( manipulator.get() );
	viewer.addEventHandler( new osgViewer::StatsHandler );
	viewer.realize();

	printCameraTransform();

	while( window->isOpen() ) {
		sf::Event event;

		while ( window->pollEvent( event ) ) {
			// pass the SFML event into the viewers event queue
			convertEvent( event, gw, window );
		}

		viewer.frame();
		window->display();
	}

	return EXIT_SUCCESS;
}
