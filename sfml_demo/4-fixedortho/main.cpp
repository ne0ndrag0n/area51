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
#include "material.hpp"
#include "gfxtransform.hpp"
#include "atlasbuilder.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;
float zoom = 2.0f;

int main() {
  sf::RenderWindow mainWindow(
    sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ),
    "OpenGL",
    sf::Style::Close,
    sf::ContextSettings( 24, 8, 0, 3, 3 )
  );

  mainWindow.setFramerateLimit( 30 );

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  Shader shader( "vertex.glsl", "fragment.glsl" );

  glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );

  sf::Vector2i center( ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT / 2 ) );

  LotCamera lotCamera( shader.Program, SCREEN_WIDTH, SCREEN_HEIGHT );
  mainWindow.setMouseCursorVisible( lotCamera.ortho );

  // GFXModels may contain multiple Mesh objects
  GFXModel m( "floor/floor.dae" );
  GFXModel box( "box/box.dae" );

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

  std::vector< GFXInstance > floorTiles;

  for( int i = 0; i != 10; i++ ) {
    GFXInstance floorTile( m, shader.Program );
    floorTile.setPosition( glm::vec3( 0, 0, ( i * 2 ) ) );
    floorTiles.push_back( floorTile );

    GFXInstance floorTile2( m, shader.Program );
    floorTile2.setPosition( glm::vec3( 1.0f, 0.0f, ( i * 2 ) ) );
    floorTiles.push_back( floorTile2 );

    GFXInstance boxTile1( box, shader.Program );
    boxTile1.setPosition( glm::vec3( 2.0f, 0.0f, ( i * 2 ) ) );
    floorTiles.push_back( boxTile1 );
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

    mainWindow.pushGLStates();
      text.setString( "Concordia Engine Test" );
      coords.setString( lotCamera.positionToString() );

      mainWindow.draw( text );
      mainWindow.draw( coords );
    mainWindow.popGLStates();

    mainWindow.display();

    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
      }

      if( event.type == sf::Event::KeyPressed ) {

        /*
        if( event.key.code == sf::Keyboard::Q ) {
          lotCamera.rotateLeft();
        }

        if( event.key.code == sf::Keyboard::E ) {
          lotCamera.rotateRight();
        }
        */

        if( event.key.code == sf::Keyboard::Up ) {
          lotCamera.move( 0.0f, -0.1f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Down ) {
          lotCamera.move( 0.0f, 0.1f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Right ) {
          lotCamera.move( -0.1f, 0.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Left ) {
          lotCamera.move( 0.1f, 0.0f, 0.0f );
        }

        /*
        if( event.key.code == sf::Keyboard::Add && zoom != 1.0f ) {
          lotCamera.zoomIn();
        }

        if( event.key.code == sf::Keyboard::Subtract && zoom != 3.0f ) {
          lotCamera.zoomOut();
        }
        */
      }
    }
  }
  return 0;
}
