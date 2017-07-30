#include <memory>
#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osg/Node>
#include "device/display.hpp"
#include "device/input.hpp"
#include "graphics/rendering/model.hpp"
#include "graphics/rendering/sceneview.hpp"
#include "graphics/rendering/sceneview_camera.hpp"
#include "graphics/rendering/group.hpp"
#include "graphics/rendering/types.hpp"
#include "graphics/gui/overlay.hpp"
#include "graphics/gui/drawables/window.hpp"
#include "graphics/gui/widget/widgetengine.hpp"
#include "graphics/gui/widget/window.hpp"

using namespace BlueBear;
using namespace BlueBear::Graphics::Rendering;
using namespace BlueBear::Device;

std::shared_ptr< Model > cyl;
std::shared_ptr< Group > myGroup = Group::create();

int main( int argc, char** argv ) {
	Display displayDevice( 1200, 700 );
	Input inputDevice;
	Graphics::GUI::Overlay overlay( displayDevice );
	SceneView sceneView( displayDevice, inputDevice, &overlay );
	Graphics::GUI::Widget::WidgetEngine widgetEngine( displayDevice, inputDevice, overlay );

	std::shared_ptr< Graphics::GUI::Widget::Window > windowWidget = Graphics::GUI::Widget::Window::create( "Debug Options" );
	windowWidget->setStyleValue( "left", 10 );
	windowWidget->setStyleValue( "top", 10 );
	windowWidget->setStyleValue( "width", 400.0 );
	windowWidget->setStyleValue( "height", 300.0 );
	widgetEngine.append( windowWidget );

	std::shared_ptr< Graphics::GUI::Widget::Window > windowWidget2 = Graphics::GUI::Widget::Window::create( "Test Window 2" );
	windowWidget2->setStyleValue( "left", 100 );
	windowWidget2->setStyleValue( "top", 100 );
	windowWidget2->setStyleValue( "width", 400.0 );
	windowWidget2->setStyleValue( "height", 300.0 );
	widgetEngine.append( windowWidget2 );

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
	while( sceneView.update() ) {
		widgetEngine.update();
	}

	return EXIT_SUCCESS;
}
