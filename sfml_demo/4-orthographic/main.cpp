#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "model.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
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

  Model m( "./floor.obj" );

  GLfloat vertices[] = {
    -50.0f, 50.0f, 0.0f,     0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    50.0f, 50.0f, 0.0f,      0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -50.0f, -50.0f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -50.0f, -50.0f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    50.0f, 50.0f, 0.0f,      0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    50.0f, -50.0f, 0.0f,     0.0f, 0.0f, 0.0f,  1.0f, 0.0f
  };

  GLuint VBO, VAO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );

  glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
      glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

      glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );
      glEnableVertexAttribArray( 0 );

      glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)) );
      glEnableVertexAttribArray( 1 );

      glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat) ) );
      glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray( 0 );

  GLuint alienTexture2;
  {
    sf::Image alien;
    if( !alien.loadFromFile( "floor.png" ) ) {
      std::cout << "Couldn't load required texture." << std::endl;
      exit(1);
    }

    glGenTextures( 1, &alienTexture2 );
    glBindTexture( GL_TEXTURE_2D, alienTexture2 );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      auto size = alien.getSize();
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, alien.getPixelsPtr() );
      glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, 0 );
  }

  sf::Clock clock;
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

  // BlueBear is normally played through an isometric projection. Which isn't really true isometric, it's just the "90s isometric" look using an
  // orthographic projection and strategically placed camera. Our camera up vector is +Z, so objects on top of objects have increasing Z values.
  // THE IMPORTANT BIT: ** The camera goes -X and -Y for as much as the camera goes +Z from the Z of the point it looks at. **
  // Read that again carefully. This forms a 45 degree camera angle (because of 45-45-90 triangles). Explanation:
  // The camera looks at a point 100 units from the ground and 300 units up from that point (arbitrary values). The height of the camera mostly
  // affects the appearance of the perspective view. Therefore, the camera is as high up from the floor (at Z-1000) as high as the point it looks at,
  // plus the Z distance from that point it is looking at. In the standard case, that makes a Z distance of 400 pixels from the floor (-600).

  // In BlueBear, you may enter the house itself using a perspective projection! Depending on what is planned, these constraints will not
  // apply in that mode. Each mode begins with the same constaints as isometric mode, but some may or may not be kept:
  // Explorer View: All bets are off; you can go anywhere and do what you want. Compute the direction vector based on the pitch and yaw of the mouse.
  // Doll's-Eye View: This is explorer view, locked to the Z-height of the doll. The camera never goes higher or lower than the doll's eye height.

  sf::Vector2i center( ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT / 2 ) );

  LotCamera lotCamera( shader.Program, SCREEN_WIDTH, SCREEN_HEIGHT );
  mainWindow.setMouseCursorVisible( lotCamera.ortho );

  while( mainWindow.isOpen() ) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.use();
    lotCamera.position();

    glActiveTexture( GL_TEXTURE0 );
      glBindTexture( GL_TEXTURE_2D, alienTexture2 );
      glUniform1i( glGetUniformLocation( shader.Program, "texture0" ), 0 );

    GLuint uModel = glGetUniformLocation( shader.Program, "model" );

    glBindVertexArray( VAO );
      // Floor tiles
      //for( int x = -16; x != 16; x++ ) {
        //for( int y = -16; y != 16; y++ ) {
          glm::mat4 model;
          model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -1000.0f ) );
          glUniformMatrix4fv( uModel, 1, GL_FALSE, glm::value_ptr( model ) );

          glDrawArrays( GL_TRIANGLES, 0, 6 );
        //}
      //}
    glBindVertexArray( 0 );

    mainWindow.pushGLStates();
      text.setString( lotCamera.ortho ? "Isometric" : "First-person" );
      coords.setString( lotCamera.positionToString().c_str() );
      cameraCoords.setString( lotCamera.directionToString().c_str() );
      mainWindow.draw( text );
      mainWindow.draw( cameraCoords );
      mainWindow.draw( coords );
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
        glDeleteVertexArrays( 1, &VAO );
        glDeleteBuffers( 1, &VBO );
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
          lotCamera.move( 0.0f, 10.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Down ) {
          lotCamera.move( 0.0f, -10.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Right ) {
          lotCamera.move( 10.0f, 0.0f, 0.0f );
        }

        if( event.key.code == sf::Keyboard::Left ) {
          lotCamera.move( -10.0f, 0.0f, 0.0f );
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
