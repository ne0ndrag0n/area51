#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/io_utils>

#include <iostream>

int main( int argc, char** argv ) {
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = 1200; traits->height = 700;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextVersion = "3.3";
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    if( !gc.valid() )
    {
        osg::notify( osg::FATAL ) << "Unable to create OpenGL context." << std::endl;
        return( 1 );
    }

    // Plain quad
    osg::ref_ptr< osg::Geometry > quad = osg::createTexturedQuadGeometry( osg::Vec3( 100.0, 100.0, 0.0 ), osg::Vec3( 20.0, 0.0, 0.0 ), osg::Vec3( 0.0, 20.0, 0.0 ) );

    // Geode to hold the quad (a drawable)
    osg::ref_ptr< osg::Geode > geode = new osg::Geode;
    geode->setCullingActive( false );
    geode->addDrawable( quad.get() );
    geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setAllowEventFocus( false );
    camera->setProjectionMatrix( osg::Matrix::ortho2D( 0.0, 1200.0, 700.0, 0.0 ) );
    camera->addChild( geode.get() );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( osgDB::readNodeFile("cylinder.fbx") );
    root->addChild( camera.get() );

    osgViewer::Viewer viewer;
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    viewer.addEventHandler( new osgViewer::StatsHandler );
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    viewer.setSceneData( root.get() );

    osg::ref_ptr< osg::Camera > mainCamera = viewer.getCamera();
    mainCamera->setGraphicsContext( gc );
    mainCamera->setViewport( 0, 0, 1200, 700 );

    osg::setNotifyLevel( osg::NotifySeverity::ALWAYS );
    return viewer.run();
}
