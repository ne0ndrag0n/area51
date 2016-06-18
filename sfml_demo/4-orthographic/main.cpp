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

int main() {
  sf::Window mainWindow(
    sf::VideoMode( 640, 480 ),
    "ne0ndrag0n area51 - SFML & OpenGL Training",
    sf::Style::Close,
    sf::ContextSettings( 24, 8, 0, 3, 3 )
  );

  mainWindow.setVerticalSyncEnabled( true );

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  Shader shader( "vertex.glsl", "fragment.glsl" );

  glViewport( 0, 0, 640, 480 );

  glEnable( GL_DEPTH_TEST );

  GLfloat vertices[] = {
    -50.0f, -50.0f, -50.0f,  0.0f, 0.0f,
     50.0f, -50.0f, -50.0f,  1.0f, 0.0f,
     50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
     50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
    -50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
    -50.0f, -50.0f, -50.0f,  0.0f, 0.0f,

    -50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
     50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
     50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
     50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
    -50.0f,  50.0f,  50.0f,  0.0f, 1.0f,
    -50.0f, -50.0f,  50.0f,  0.0f, 0.0f,

    -50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
    -50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
    -50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
    -50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
    -50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
    -50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

     50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
     50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
     50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
     50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
     50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
     50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

    -50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
     50.0f, -50.0f, -50.0f,  1.0f, 1.0f,
     50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
     50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
    -50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
    -50.0f, -50.0f, -50.0f,  0.0f, 1.0f,

    -50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
     50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
     50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
     50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
    -50.0f,  50.0f,  50.0f,  0.0f, 0.0f,
    -50.0f,  50.0f, -50.0f,  0.0f, 1.0f
  };

  GLuint VBO, VAO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );

  glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
      glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

      glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0 );
      glEnableVertexAttribArray( 0 );

      glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)) );
      glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray( 0 );


  GLuint alienTexture;
  {
    sf::Image alien;
    if( !alien.loadFromFile( "alien.png" ) ) {
      std::cout << "Couldn't load required texture." << std::endl;
      exit(1);
    }

    alien.flipVertically();

    glGenTextures( 1, &alienTexture );
    glBindTexture( GL_TEXTURE_2D, alienTexture );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      auto size = alien.getSize();
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, alien.getPixelsPtr() );
      glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, 0 );
  }

  GLuint alienTexture2;
  {
    sf::Image alien;
    if( !alien.loadFromFile( "alien2.png" ) ) {
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

  glm::vec3 camera( 50.0f, 600.0f, 0.0f );
  glm::vec3 lookingAt( 0.0f, 0.0f, -800.0f );
  // Assuming our world is flat up against a wall, with positive Z being the top of the camera
  glm::vec3 up( 0.0f, 0.0f, 1.0f );

  GLfloat rotAngleX = 45.0f;
  GLfloat rotAngleY = 0.0f;
  GLfloat rotAngleZ = 45.0f;

  glm::vec3 cubes[] = {
    glm::vec3( 0.0f, 0.0f, -900.0f ),
    glm::vec3( 100.0f, 0.0f, -900.0f )
  };

  bool ortho = true;

  while( mainWindow.isOpen() ) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.use();

    // Adjust camera
    glm::mat4 view;
    if( ortho ) {
      view = glm::translate( view, camera );
      view = glm::rotate( view, glm::radians( rotAngleX ), glm::vec3( -1.0f, 0.0f, 0.0f ) );
      view = glm::rotate( view, glm::radians( rotAngleY ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
      view = glm::rotate( view, glm::radians( rotAngleZ ), glm::vec3( 0.0f, 0.0f, -1.0f ) );
    } else {
      // Testing lookat
      view = glm::lookAt( camera, lookingAt, up );
    }
    // Apply correct projection (to have real-world perspective)
    glm::mat4 projection;
    projection = ortho ? glm::ortho( -320.0f, 320.0f, -240.0f, 240.0f, 0.0f, 1000.0f ) : glm::perspective( 45.0f, (float)640/(float)480, 0.1f, 2000.0f );

    GLuint uModel = glGetUniformLocation( shader.Program, "model" );
    GLuint uView = glGetUniformLocation( shader.Program, "view" );
    glUniformMatrix4fv( uView, 1, GL_FALSE, glm::value_ptr( view ) );
    GLuint uProjection = glGetUniformLocation( shader.Program, "projection" );
    glUniformMatrix4fv( uProjection, 1, GL_FALSE, glm::value_ptr( projection ) );

    glActiveTexture( GL_TEXTURE0 );
      glBindTexture( GL_TEXTURE_2D, alienTexture );
      glUniform1i( glGetUniformLocation( shader.Program, "alien" ), 0 );
    glActiveTexture( GL_TEXTURE1 );
      glBindTexture( GL_TEXTURE_2D, alienTexture2 );
      glUniform1i( glGetUniformLocation( shader.Program, "alien2" ), 1 );

    glBindVertexArray( VAO );
      for( int i = 0; i != 2; i++ ) {
        glm::mat4 model;
        model = glm::translate( model, cubes[ i ] );
        glUniformMatrix4fv( uModel, 1, GL_FALSE, glm::value_ptr( model ) );

        glDrawArrays( GL_TRIANGLES, 0, 36 );
      }
    glBindVertexArray( 0 );

    mainWindow.display();

    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
        glDeleteVertexArrays( 1, &VAO );
        glDeleteBuffers( 1, &VBO );
      }

      if( event.type == sf::Event::KeyPressed ) {
        if( event.key.code == sf::Keyboard::P ) {
          if( !ortho ) {
            ortho = true;
            camera.x = 50.0f;
            camera.y = 600.0f;
            camera.z = 0.0f;

            rotAngleX = rotAngleZ = 45.0f;
            rotAngleY = 0.0f;
          } else {
            ortho = false;
            camera = glm::vec3( 750.0f, 750.0f, -50.0f );
            lookingAt = glm::vec3( 0.0f, 0.0f, -800.0f );
            up = glm::vec3( 0.0f, 1.0f, 0.0f );
          }
        }

        std::cout << camera.x << ", " << camera.y << ", " << camera.z << std::endl;
      }
    }
  }
  return 0;
}
