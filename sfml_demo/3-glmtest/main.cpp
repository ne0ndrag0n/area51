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
  sf::RenderWindow mainWindow(
    sf::VideoMode( 640, 480 ),
    "ne0ndrag0n area51 - SFML & OpenGL Training",
    sf::Style::Close
  );

  mainWindow.setVerticalSyncEnabled( true );

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  Shader shader( "vertex.glsl", "fragment.glsl" );

  glViewport( 0, 0, 640, 480 );

  // Position, Colour, Texture Mapping
  GLfloat vertices[] = {
    0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f
  };
  GLuint indicies[] = { 0, 1, 3, 1, 2, 3 };

  GLuint VBO, VAO, EBO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenBuffers( 1, &EBO );

  glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
      glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

      glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );
      glEnableVertexAttribArray( 0 );

      glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)) );
      glEnableVertexAttribArray( 1 );

      glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) ( 6 * sizeof(GLfloat)));
      glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indicies ), indicies, GL_STATIC_DRAW );

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

  while( mainWindow.isOpen() ) {
    mainWindow.clear( sf::Color::Black );

    shader.use();

    glm::mat4 trans;
    trans = glm::translate( trans, glm::vec3( 0.5f, -0.5f, 0.0f ) );
    trans = glm::rotate( trans, glm::radians( ( GLfloat ) clock.getElapsedTime().asSeconds() * 90.0f ), glm::vec3( 0.0, 0.0, 1.0 ) );
    trans = glm::scale( trans, glm::vec3( 0.5, 0.5, 0.5 ) );
    GLuint transform = glGetUniformLocation( shader.Program, "rotateAndScale" );
    glUniformMatrix4fv( transform, 1, GL_FALSE, glm::value_ptr( trans ) );

    glActiveTexture( GL_TEXTURE0 );
      glBindTexture( GL_TEXTURE_2D, alienTexture );
      glUniform1i( glGetUniformLocation( shader.Program, "alien" ), 0 );
    glActiveTexture( GL_TEXTURE1 );
      glBindTexture( GL_TEXTURE_2D, alienTexture2 );
      glUniform1i( glGetUniformLocation( shader.Program, "alien2" ), 1 );

    glBindVertexArray( VAO );
      glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );

    mainWindow.display();

    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
        glDeleteVertexArrays( 1, &VAO );
        glDeleteBuffers( 1, &VBO );
      }
    }
  }
  return 0;
}
