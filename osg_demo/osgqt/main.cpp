/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 9 Recipe 3
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QString>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QCoreApplication>

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/QWidgetImage>
#include <iostream>

QWidget* createDemoWidget( const QString& movieFile )
{
    QLabel* label = new QLabel;
    QPushButton* playBtn = new QPushButton("Play");
    QPushButton* stopBtn = new QPushButton("Stop");

    QWidget* demo = new QWidget;
    demo->setGeometry( 1, 1, 450, 450 );
    demo->setLayout( new QVBoxLayout );
    demo->layout()->addWidget( label );
    demo->layout()->addWidget( playBtn );
    demo->layout()->addWidget( stopBtn );

    QMovie* movie = new QMovie(movieFile);
    label->setMovie( movie );
	label->setFixedHeight( 400 );
    QObject::connect( playBtn, SIGNAL(clicked()), movie, SLOT(start()) );
    QObject::connect( stopBtn, SIGNAL(clicked()), movie, SLOT(stop()) );
    return demo;
}

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = 1920; traits->height = 1080;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextVersion = "3.3";
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    if( !gc.valid() )
    {
        std::cout << "Unable to create OpenGL context." << std::endl;
        return( 1 );
    }

    osg::ref_ptr<osgQt::QWidgetImage> widgetImage =
        new osgQt::QWidgetImage( createDemoWidget("./animation.gif") );

    osg::ref_ptr<osgViewer::InteractiveImageHandler> handler =
        new osgViewer::InteractiveImageHandler( widgetImage.get() );

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( widgetImage.get() );

    osg::ref_ptr<osg::Geometry> quad =
        osg::createTexturedQuadGeometry(osg::Vec3(), osg::X_AXIS, osg::Z_AXIS);
    quad->setEventCallback( handler.get() );
    quad->setCullCallback( handler.get() );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setCullingActive( false );
    geode->addDrawable( quad.get() );
    geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    geode->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( geode.get() );

    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
    viewer.setCameraManipulator( new osgGA::TrackballManipulator );
    viewer.addEventHandler( new osgViewer::StatsHandler );

    osg::ref_ptr< osg::Camera > camera = viewer.getCamera();
    camera->setGraphicsContext( gc );
    camera->setViewport( 0, 0, 1920, 1080 );

    while ( !viewer.done() )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        viewer.frame();
    }
    return 0;
}
