#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nanovg.h"
#include "nanovg_gl.h"

#include <osg/Drawable>
#include <osg/MatrixTransform>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/io_utils>

class NanoVGDrawable : public osg::Drawable {
public:
    NanoVGDrawable()
    :   _vg(NULL), _activeContextID(0), _width(800), _height(600), _initialized(false)
    {
        setSupportsDisplayList( false );
    }

    NanoVGDrawable( const NanoVGDrawable& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY )
    :   osg::Drawable(copy, copyop), _vg(copy._vg),
        _loadedImages(copy._loadedImages), _activeContextID(copy._activeContextID),
        _width(copy._width), _height(copy._height), _initialized(copy._initialized)
    {
    }

    META_Object( osg, NanoVGDrawable );

    virtual void drawImplementation( osg::RenderInfo& renderInfo ) const
    {
        unsigned int contextID = renderInfo.getContextID();
        if ( !_initialized )
        {
            NanoVGDrawable* constMe = const_cast<NanoVGDrawable*>(this);
            glewInit();
            std::cout << "starting..." << std::endl;
            constMe->_vg = nvgCreateGL3( NVG_ANTIALIAS|NVG_STENCIL_STROKES|NVG_DEBUG );
            std::cout << "done" << std::endl;
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
        nvgFillColor( _vg, nvgRGBA(255, 192, 0, 255) );
        nvgFill( _vg );
        nvgClosePath( _vg );

        nvgBeginPath( _vg );
        nvgCircle( _vg, 400, 500, 50 );
        nvgFillColor( _vg, nvgRGBA(0, 192, 255, 100) );
        nvgFill( _vg );
        nvgClosePath( _vg );

        if ( _loadedImages.size()>0 )
        {
            NVGpaint imgPaint = nvgImagePattern( _vg, 600, 150, 300, 400, 0.0f,
                                                 _loadedImages[0], 1.0f );
            nvgBeginPath( _vg );
            nvgRoundedRect( _vg, 600, 150, 300, 400, 5 );
            nvgFillPaint( _vg, imgPaint );
            nvgFill( _vg );
            nvgClosePath( _vg );
        }
    }

    void setWindowSize( int w, int h )
    { _width = w; _height = h; }

protected:
    NVGcontext* _vg;
    std::vector<int> _loadedImages;
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
        /*
            if ( _camera.valid() )
            {
                _camera->setViewport( 0.0, 0.0, width, height );
                _vg->setWindowSize( width, height );
            }
            */
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

int main( int argc, char** argv ) {
    GLFWwindow* window;
    glfwSetErrorCallback( []( int error, const char* description ) {
      std::cout << description << std::endl;
    } );

    if( !glfwInit() ) {
      std::cout << "Init failed for GLFW" << std::endl;
      exit( 1 );
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    glfwWindowHint( GLFW_SAMPLES, 0 );
    glfwWindowHint( GLFW_RED_BITS, 8 );
    glfwWindowHint( GLFW_GREEN_BITS, 8 );
    glfwWindowHint( GLFW_BLUE_BITS, 8 );
    glfwWindowHint( GLFW_ALPHA_BITS, 8 );
    glfwWindowHint( GLFW_STENCIL_BITS, 8 );
    glfwWindowHint( GLFW_DEPTH_BITS, 24 );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );

    if( !( window = glfwCreateWindow( 1200, 700, "ne0ndrag0n area51 GLFW-OSG evaluation", NULL, NULL ) ) ) {
      std::cout << "Window failed for GLFW" << std::endl;
      glfwTerminate();
      exit( 1 );
    }

    glfwMakeContextCurrent( window );
    glewInit();
    glfwSwapInterval( 1 );

    osg::ref_ptr<NanoVGDrawable> vgDrawable = new NanoVGDrawable;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setCullingActive( false );
    geode->addDrawable( vgDrawable.get() );
    geode->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    geode->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

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
    //viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    //viewer.addEventHandler( new osgViewer::StatsHandler );
    //viewer.addEventHandler( new osgViewer::WindowSizeHandler );
    //viewer.addEventHandler( new NanoVGHandler(camera.get(), vgDrawable.get()) );
    viewer.setSceneData( root.get() );
    //viewer.setUpViewOnSingleScreen( 0 );

    osgViewer::GraphicsWindow* gw = viewer.setUpViewerAsEmbeddedInWindow( 0, 0, 1200, 700 );

    viewer.realize();

    vgDrawable->setWindowSize( 1200, 700 );
    /*
    if ( gw )
    {
        // Send window size event for NanoVG to handle
        int x, y, w, h;
        gw->getWindowRectangle( x, y, w, h );
        vgDrawable->setWindowSize( w, h );
    }
    return viewer.run();
    */

    while( !glfwWindowShouldClose( window ) && !viewer.done() ) {
      glfwPollEvents();
      glfwSwapBuffers( window );

      viewer.frame();
    }

    glfwDestroyWindow( window );
    glfwTerminate();
    return 0;
}
