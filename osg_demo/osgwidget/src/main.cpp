/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 9 Recipe 5
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgWidget/WindowManager>
#include <osgWidget/Box>
#include <osgWidget/Canvas>
#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>
#include <osg/Program>
#include <osg/Shader>
#include <iostream>
#include <sstream>

osg::ref_ptr< osg::Program > shaderProgram;

extern bool tabPressed( osgWidget::Event& ev );

osg::ref_ptr< osg::Program > getProgram() {
  osg::ref_ptr< osg::Program > program = new osg::Program();

  osg::ref_ptr< osg::Shader > vertex = new osg::Shader( osg::Shader::VERTEX );
  vertex->loadShaderSourceFromFile( "system/shaders/default_vertex.glsl" );

  osg::ref_ptr< osg::Shader > fragment = new osg::Shader( osg::Shader::FRAGMENT );
  fragment->loadShaderSourceFromFile( "system/shaders/default_fragment.glsl" );

  program->addShader( fragment );
  program->addShader( vertex );

  //node->getOrCreateStateSet()->setAttributeAndModes( program, osg::StateAttribute::ON );
  return program;
}

osgWidget::Label* createLabel( const std::string& name, const std::string& text,
                               float size, const osg::Vec4& color )
{
    osg::ref_ptr<osgWidget::Label> label = new osgWidget::Label(name);
    label->setLabel( text );
    label->setFont( "roboto.ttf" );
    label->setFontSize( size );
    label->setFontColor( 1.0f, 1.0f, 1.0f, 1.0f );
    label->setColor( color );
    label->addSize( 10.0f, 10.0f );
    label->setCanFill( true );

    label->getText()->getOrCreateStateSet()->setAttributeAndModes( shaderProgram, osg::StateAttribute::ON );

    return label.release();
}

osgWidget::Window* createSimpleTabs( float winX, float winY )
{
    //osg::ref_ptr<osgWidget::Canvas> contents = new osgWidget::Canvas("contents");
    osg::ref_ptr<osgWidget::Box> tabs =
        new osgWidget::Box("tabs", osgWidget::Box::HORIZONTAL);

    for ( unsigned int i=0; i<3; ++i )
    {
        osg::Vec4 color(0.0f, (float)i / 3.0f, 0.0f, 1.0f);
        std::string str = std::string( "Tab-" ) + std::to_string( i );

        /*
        osgWidget::Label* content = createLabel(ss.str(), ss2.str(), 10.0f, color);
        content->setLayer( osgWidget::Widget::LAYER_MIDDLE, i );
        contents->addWidget( content, 0.0f, 0.0f );
        */

        osgWidget::Label* tab = createLabel(str, str, 20.0f, color);
        /*
        tab->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
        tab->addCallback( new osgWidget::Callback(
            &tabPressed, osgWidget::EVENT_MOUSE_PUSH ) );
        */
        tabs->addWidget( tab );
    }

    osg::ref_ptr<osgWidget::Box> main = new osgWidget::Box("main", osgWidget::Box::VERTICAL);
    main->setOrigin( winX, winY );
    main->attachMoveCallback();
    //main->addWidget( contents->embed() );
    //main->addWidget( tabs->embed() );
    main->addWidget( createLabel("title", "Tabs Demo", 15.0f, osg::Vec4(0.0f, 0.4f, 1.0f, 1.0f)) );
    return main.release();
}

bool tabPressed( osgWidget::Event& ev )
{
  /*
    osgWidget::Label* content = static_cast<osgWidget::Label*>( ev.getData() );
    if ( !content ) return false;

    osgWidget::Canvas* canvas = dynamic_cast<osgWidget::Canvas*>( content->getParent() );
    if ( canvas )
    {
        osgWidget::Canvas::Vector& objs = canvas->getObjects();
        for( unsigned int i=0; i<objs.size(); ++i )
            objs[i]->setLayer( osgWidget::Widget::LAYER_MIDDLE, i );

        content->setLayer( osgWidget::Widget::LAYER_TOP, 0 );
        canvas->resize();
    }
    return true;
    */
    return false;
}

int main( int argc, char** argv )
{
    shaderProgram = getProgram();

    osgViewer::Viewer viewer;
    osg::ref_ptr<osgWidget::WindowManager> wm =
        new osgWidget::WindowManager(&viewer, 1024.0f, 768.0f, 0xf0000000);
    osg::Camera* camera = wm->createParentOrthoCamera();

    wm->addChild( createSimpleTabs(100.0f, 100.0f) );
    wm->resizeAllWindows();

    osg::ref_ptr<osg::Group> root = new osg::Group;
    //root->addChild( osgDB::readNodeFile("cow.osg") );
    root->addChild( camera );

    viewer.setSceneData( root.get() );
    viewer.setUpViewInWindow( 50, 50, 1024, 768 );
    viewer.addEventHandler( new osgWidget::MouseHandler(wm.get()) );
    viewer.addEventHandler( new osgWidget::KeyboardHandler(wm.get()) );
    viewer.addEventHandler( new osgWidget::ResizeHandler(wm.get(), camera) );
    viewer.addEventHandler( new osgWidget::CameraSwitchHandler(wm.get(), camera) );
    return viewer.run();
}
