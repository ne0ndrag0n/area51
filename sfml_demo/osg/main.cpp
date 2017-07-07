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
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Observer>
#include <osg/NodeVisitor>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgAnimation/AnimationManagerBase>
#include <osgAnimation/BasicAnimationManager>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "graphics/rendering/model.hpp"

using namespace BlueBear::Graphics::Rendering;

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
	osg::ref_ptr< osg::StateSet > stateSet = root->getOrCreateStateSet();

	osg::ref_ptr< osg::Light > overheadLight = new osg::Light();
	overheadLight->setLightNum( 1 );
	overheadLight->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	overheadLight->setDiffuse( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	overheadLight->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	overheadLight->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

	osg::ref_ptr< osg::Light > backLight = new osg::Light();
	backLight->setLightNum( 2 );
	backLight->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	backLight->setDiffuse( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	backLight->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	backLight->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

	osg::ref_ptr< osg::Light > leftLight = new osg::Light();
	leftLight->setLightNum( 3 );
	leftLight->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	leftLight->setDiffuse( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	leftLight->setAmbient( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	leftLight->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

	osg::ref_ptr< osg::LightSource > overhead = new osg::LightSource();
	overhead->setLight( overheadLight.get() );
	overhead->setLocalStateSetModes( osg::StateAttribute::ON );
	overhead->setStateSetModes( *stateSet, osg::StateAttribute::ON );

	osg::ref_ptr< osg::LightSource > back = new osg::LightSource();
	back->setLight( backLight.get() );
	back->setLocalStateSetModes( osg::StateAttribute::ON );
	back->setStateSetModes( *stateSet, osg::StateAttribute::ON );

	osg::ref_ptr< osg::LightSource > left = new osg::LightSource();
	left->setLight( leftLight.get() );
	left->setLocalStateSetModes( osg::StateAttribute::ON );
	left->setStateSetModes( *stateSet, osg::StateAttribute::ON );

	osg::ref_ptr< osg::Node > cylinder = osgDB::readRefNodeFile( "mydata/cylinder.fbx" );
	osg::ref_ptr< osg::Node > floor = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );
	osg::ref_ptr< osg::Node > floor2 = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );
	osg::ref_ptr< osg::Node > floor3 = osgDB::readRefNodeFile( "mydata/floorpanel.fbx" );

	osg::ref_ptr< osg::PositionAttitudeTransform > pipeTransform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floorTransform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floor2Transform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > floor3Transform = new osg::PositionAttitudeTransform();

	osg::ref_ptr< osg::PositionAttitudeTransform > lightTransform = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > lightTransformBack = new osg::PositionAttitudeTransform();
	osg::ref_ptr< osg::PositionAttitudeTransform > lightTransformLeft = new osg::PositionAttitudeTransform();

	root->addChild( floorTransform.get() );
	floorTransform->addChild( floor.get() );
	floorTransform->setPosition( osg::Vec3( 0.0, 0.0, 0.0 ) );

	root->addChild( floor2Transform.get() );
	floor2Transform->addChild( floor2.get() );
	floor2Transform->setPosition( osg::Vec3( 1.0, 0.0, 0.0 ) );

	root->addChild( floor3Transform.get() );
	floor3Transform->addChild( floor3.get() );
	floor3Transform->setPosition( osg::Vec3( 0.0, 1.0, 0.0 ) );

	root->addChild( lightTransform );
	lightTransform->addChild( overhead.get() );
	lightTransform->setPosition( osg::Vec3( 0.0, 0.0, 100.0 ) );

	root->addChild( lightTransformBack );
	lightTransformBack->addChild( back.get() );
	lightTransformBack->setPosition( osg::Vec3( 0.0, -100.0, 0.0 ) );

	root->addChild( lightTransformLeft );
	lightTransformLeft->addChild( left.get() );
	lightTransformLeft->setPosition( osg::Vec3( -100.0, 0.0, 0.0 ) );

	root->addChild( pipeTransform );
	pipeTransform->addChild( cylinder.get() );
	pipeTransform->setPosition( osg::Vec3( 0.0, 0.0, 0.0 ) );

	return root;
}

/*
{
	osg::Vec3 t, s, dq;
	osg::Quat r, unused;
	double angle, x, y, z;
	viewMatrix.decompose( t, r, s, unused );
	r.getRotate( angle, x, y, z );
	std::cout << "Decomposed camera:" << std::endl;
	std::cout << "Translation: " << t.x() << " " << t.y() << " " << t.z() << std::endl;
	std::cout << "Rotation: " << angle << " " << x << " " << y << " " << z << std::endl;
	std::cout << "Scale: " << s.x() << " " << s.y() << " " << s.z() << std::endl;
}
*/

int main( int argc, char** argv ) {
	osgViewer::Viewer viewer;

	int width = 800; int height = 600;
	double widthHalf = ( ( double ) width / 2 ); double heightHalf = ( ( double ) height / 2 );
	double zoom = 1.0;
	double scaledWidthHalf = ( widthHalf * zoom ) / 100.0; double scaledHeightHalf = ( heightHalf * zoom ) / 100.0;

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
	camera->setClearColor( osg::Vec4( 0.0f, 0.2f, 0.0f, 1.0f ) );
	camera->setProjectionMatrixAsOrtho( -scaledWidthHalf, scaledWidthHalf, -scaledHeightHalf, scaledHeightHalf, -20.0, 50.0 );

	osg::Matrixd viewMatrix = camera->getViewMatrix();
	viewMatrix.postMultTranslate( osg::Vec3( 10.0, 10.0, -10.0 ) );
	osg::Quat q1( osg::DegreesToRadians( 45.0 ), osg::Vec3( 0.0, 0.0, 1.0 ) );
	osg::Quat q2( osg::DegreesToRadians( -60.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) );
	osg::Quat q3 = q1 * q2;
	viewMatrix.postMultRotate( q3 );
	camera->setViewMatrix( viewMatrix );

	viewer.addEventHandler( new osgViewer::StatsHandler );
	viewer.realize();

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
