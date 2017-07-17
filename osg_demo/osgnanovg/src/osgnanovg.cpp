#include "nanovg/nanovg.h"
#include <osg/GLDefines>
#include "nanovg/nanovg_gl.h"

#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/io_utils>

#include <iostream>

class NanoVGDrawable : public osg::Drawable {
public:
    NanoVGDrawable()
    :   _vg(NULL), _activeContextID(0), _width(800), _height(600), _initialized(false)
    {
        setSupportsDisplayList( false );
    }

    virtual void drawImplementation( osg::RenderInfo& renderInfo ) const
    {
        unsigned int contextID = renderInfo.getContextID();
        if ( !_initialized )
        {
            NanoVGDrawable* constMe = const_cast<NanoVGDrawable*>(this);
            constMe->_vg = nvgCreateGL3( NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG );
            if ( !constMe->_vg )
            {
                OSG_NOTICE << "[NanoVGDrawable] Failed to create VG context" << std::endl;
                return;
            }

            constMe->initializeGL( renderInfo.getState() );
            constMe->_activeContextID = contextID;
            constMe->_initialized = true;
        }
        else if ( _vg && contextID==_activeContextID )
        {
            osg::State* state = renderInfo.getState();
            state->disableAllVertexArrays();
            state->disableTexCoordPointer( 0 );

            nvgBeginFrame( _vg, _width, _height, 1.0f );
            updateGL( state );
            nvgEndFrame( _vg );
        }
    }

    virtual void releaseGLObjects( osg::State* state=0 ) const
    {
        if ( state && state->getGraphicsContext() )
        {
            osg::GraphicsContext* gc = state->getGraphicsContext();
            if ( gc->makeCurrent() )
            {
                NanoVGDrawable* constMe = const_cast<NanoVGDrawable*>(this);
                if ( constMe->_vg )
                {
                    constMe->deinitializeGL( state );
                    nvgDeleteGL3( constMe->_vg );
                }
                gc->releaseContext();
            }
        }
    }

    virtual void initializeGL( osg::State* state )
    {
        std::string file = osgDB::findDataFile( "Images/osg256.png" );
        int img = nvgCreateImage( _vg, file.c_str(), 0 );
        if ( img!=0 ) _loadedImages.push_back( img );

        exampleFont = nvgCreateFont( _vg, "sans", "roboto.ttf" );
        if( exampleFont == -1 ) {
          std::cout << "Failed to load font" << std::endl;
        }
    }

    virtual void deinitializeGL( osg::State* state )
    {
        for ( unsigned int i=0; i<_loadedImages.size(); ++i )
            nvgDeleteImage( _vg, _loadedImages[i] );
    }

    virtual void updateGL( osg::State* state ) const
    {
        // Some test drawings...
        nvgBeginPath( _vg );
        nvgRect( _vg, 300, 300, 300, 30 );
        nvgFillColor( _vg, nvgRGBA(128, 96, 0, 255) );
        nvgFill( _vg );
        nvgClosePath( _vg );

        nvgBeginPath( _vg );
        nvgCircle( _vg, 400, 500, 50 );
        nvgFillColor( _vg, nvgRGBA(0, 192, 255, 100) );
        nvgFill( _vg );
        nvgClosePath( _vg );

        if ( _loadedImages.size()>0 )
        {
            NVGpaint imgPaint = nvgImagePattern( _vg, 600, 150, 256, 256, 0.0f,
                                                 _loadedImages[0], 1.0f );
            nvgBeginPath( _vg );
            nvgRoundedRect( _vg, 600, 150, 256, 256, 5 );
            nvgFillPaint( _vg, imgPaint );
            nvgFill( _vg );
            nvgClosePath( _vg );
        }

        if( exampleFont != -1 ) {
          nvgFontSize( _vg, 18.0f );
          nvgFontFace( _vg, "sans" );
          nvgFillColor( _vg, nvgRGBA( 255, 255, 255, 255 ) );

          nvgTextAlign( _vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE );
          nvgText( _vg, 305, 310, "Booga Booga Booga", NULL );
        }
    }

    void setWindowSize( int w, int h )
    { _width = w; _height = h; }

protected:
    NVGcontext* _vg;
    std::vector<int> _loadedImages;
    int exampleFont;
    unsigned int _activeContextID;
    int _width, _height;
    bool _initialized;
};

class NanoVGHandler : public osgGA::GUIEventHandler {
public:
    NanoVGHandler( osg::Camera* c, NanoVGDrawable* vg ) : _camera(c), _vg(vg) {}
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        int width = ea.getWindowWidth(), height = ea.getWindowHeight();
        switch ( ea.getEventType() )
        {
        case osgGA::GUIEventAdapter::RESIZE:
            if ( _camera.valid() )
            {
                std::cout << "Resize" << std::endl;
                _camera->setViewport( 0.0, 0.0, width, height );
                _vg->setWindowSize( width, height );
            }
            break;
        default:
            break;
        }
        return false;
    }

protected:
    osg::observer_ptr<osg::Camera> _camera;
    NanoVGDrawable* _vg;
};

class NanoVGHandler2 : public osgGA::GUIEventHandler {
public:
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        int width = ea.getWindowWidth(), height = ea.getWindowHeight();
        switch ( ea.getEventType() )
        {
        case osgGA::GUIEventAdapter::PUSH:
          std::cout << "Click" << std::endl;
          break;
        case osgGA::GUIEventAdapter::RELEASE:
          std::cout << "Release" << std::endl;
          break;
        case osgGA::GUIEventAdapter::KEYDOWN:
          std::cout << ea.getKey() << std::endl;
          break;
        case osgGA::GUIEventAdapter::KEYUP:
          std::cout << ea.getKey() << std::endl;
          break;
        default:
            break;
        }
        return false;
    }
};

int main( int argc, char** argv )
{
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

    osg::ref_ptr<NanoVGDrawable> vgDrawable = new NanoVGDrawable;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setCullingActive( false );
    geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    geode->addDrawable( vgDrawable.get() );

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setAllowEventFocus( false );
    camera->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
    camera->addChild( geode.get() );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( osgDB::readNodeFile("cylinder.fbx") );
    root->addChild( camera.get() );

    osgViewer::Viewer viewer;
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    viewer.addEventHandler( new osgViewer::StatsHandler );
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    viewer.addEventHandler( new NanoVGHandler(camera.get(), vgDrawable.get()) );
    viewer.addEventHandler( new NanoVGHandler2 );
    viewer.setSceneData( root.get() );

    osg::ref_ptr< osg::Camera > mainCamera = viewer.getCamera();
    mainCamera->setGraphicsContext( gc );
    mainCamera->setViewport( 0, 0, 1200, 700 );

    vgDrawable->setWindowSize( 1200, 700 );

    osg::setNotifyLevel( osg::NotifySeverity::ALWAYS );
    return viewer.run();
}
