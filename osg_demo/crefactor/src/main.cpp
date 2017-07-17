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

using namespace BlueBear;
using namespace BlueBear::Graphics::Rendering;
using namespace BlueBear::Device;

std::shared_ptr< Model > cyl;
std::shared_ptr< Group > myGroup = Group::create();

int main( int argc, char** argv ) {
	Display displayDevice( 1200, 700 );
	Graphics::GUI::Overlay overlay( displayDevice );
	Input inputDevice;

	SceneView sceneView(
		displayDevice,
		inputDevice
	);

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
	while( sceneView.update() );

	return EXIT_SUCCESS;
}
