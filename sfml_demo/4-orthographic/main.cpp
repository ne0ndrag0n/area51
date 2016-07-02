#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "graphicentity.hpp"
#include "gfxtransform.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 576;
float zoom = 2.0f;

int main() {
  sf::RenderWindow mainWindow(
    sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ),
    "OpenGL",
    sf::Style::Close,
    sf::ContextSettings( 24, 8, 0, 3, 3 )
  );

  mainWindow.setVerticalSyncEnabled( true );

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  Shader shader( "vertex.glsl", "fragment.glsl" );

  glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
  glEnable( GL_DEPTH_TEST );

  sf::Vector2i center( ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT / 2 ) );

  LotCamera lotCamera( shader.Program, SCREEN_WIDTH, SCREEN_HEIGHT );
  mainWindow.setMouseCursorVisible( lotCamera.ortho );

  // GFXModels may contain multiple Mesh objects
  GFXModel m( "floor/floor.obj" );
  GFXModel box( "box/box.obj" );
  GFXModel smallBox( "twobox/twobox.obj" );

  sf::Font dosvga;
  if( !dosvga.loadFromFile( "font.ttf" ) ) {
    std::cout << "Couldn't load font.ttf" << std::endl;
    return 1;
  }

  sf::Text text;
  text.setFont( dosvga );
  text.setCharacterSize( 16 );
  text.setColor( sf::Color::White );

  sf::Text coords;
  coords.setFont( dosvga );
  coords.setCharacterSize( 16 );
  coords.setColor( sf::Color::Red );
  coords.setPosition( 0, 16 );

  sf::Text cameraCoords;
  cameraCoords.setFont( dosvga );
  cameraCoords.setCharacterSize( 16 );
  cameraCoords.setColor( sf::Color::Cyan );
  cameraCoords.setPosition( 0, 32 );

  GFXInstance b1( box, shader.Program );
  b1.getBaseTransform().position = glm::vec3( 0.0f, 0.0f, -9.5f );

  GFXInstance b2( box, shader.Program );
  b2.getBaseTransform().position = glm::vec3( 0.0f, 0.0f, -8.5f );

  GFXInstance b3( box, shader.Program );
  b3.getBaseTransform().position = glm::vec3( 0.0f, 0.0f, -7.5f );

  GFXInstance smallPair( smallBox, shader.Program );
  smallPair.getBaseTransform().position = glm::vec3( 0.0f, 1.0f, -9.85f );

  GFXTransform topCubeTransform;
  topCubeTransform.rotationAngle = 45.0f;
  topCubeTransform.rotationAxes.z = 1.0f;
  topCubeTransform.applyRotation = true;
  smallPair.transforms[ "TopCube" ] = topCubeTransform;

  std::vector< GFXInstance > floorTiles;
  for( int x = -16; x != 16; x++ ) {
    for( int y = -16; y != 16; y++ ) {
      GFXInstance floorTile( m, shader.Program );
      floorTile.getBaseTransform().position = glm::vec3( ( GLfloat ) x, ( GLfloat ) y, -10.0f );
      floorTiles.push_back( floorTile );
    }
  }

  sf::Clock clock;

  while( mainWindow.isOpen() ) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.use();
    lotCamera.position();

    // Draw the world
    for( auto& floorTile : floorTiles ) {
      floorTile.drawEntity();
    }
    b1.drawEntity();
    b2.drawEntity();
    b3.drawEntity();
    smallPair.drawEntity();
    // Adjust the rotation on the topCubeTransform
    smallPair.transforms[ "TopCube" ].rotationAngle = glm::radians( ( GLfloat ) clock.getElapsedTime().asSeconds() * 2.0f * 90.0f );

    mainWindow.pushGLStates();
      text.setString( lotCamera.ortho ? "Isometric" : "First-person" );
      coords.setString( lotCamera.positionToString().c_str() );
      cameraCoords.setString( lotCamera.directionToString().c_str() );

      mainWindow.draw( text );
      mainWindow.draw( coords );
      mainWindow.draw( cameraCoords );
    mainWindow.popGLStates();

    mainWindow.display();

    if( !lotCamera.ortho ) {
      sf::Vector2i mouseDelta = sf::Mouse::getPosition( mainWindow ) - center;
      lotCamera.updateFirstPersonView( mouseDelta.x, mouseDelta.y );
      sf::Mouse::setPosition( center, mainWindow );
    }

    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
      }

      if( event.type == sf::Event::KeyPressed ) {
        if( event.key.code == sf::Keyboard::P ) {
          if( lotCamera.ortho ) {
            lotCamera.setOrthographic( false );
            mainWindow.setMouseCursorVisible( false );
            sf::Mouse::setPosition( center, mainWindow );
          } else {
            lotCamera.setOrthographic( true );
            mainWindow.setMouseCursorVisible( true );
          }
        }

        if( event.key.code == sf::Keyboard::Up ) {
          lotCamera.move( 0.0f, 0.1f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Down ) {
          lotCamera.move( 0.0f, -0.1f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Right ) {
          lotCamera.move( 0.1f, 0.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Left ) {
          lotCamera.move( -0.1f, 0.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Add && zoom != 1.0f ) {
          lotCamera.zoomIn();
        }

        if( event.key.code == sf::Keyboard::Subtract && zoom != 3.0f ) {
          lotCamera.zoomOut();
        }
      }
    }
  }
  return 0;
}
