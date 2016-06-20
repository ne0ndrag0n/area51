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

  // This is the aforementioned "height from the lookat point". The less this is, the bigger the scene will appear.
  GLfloat cameraHeight = 300.0f;
  // Z will never be used here, as worldPosition refers to a relative value added to the origin of the lookingAt and camera positions.
  glm::vec3 lookingAt( 0.0f, 0.0f, -900.0f );
  glm::vec3 camera( -cameraHeight, -cameraHeight, lookingAt.z + cameraHeight );
  // lookingAt = camera + direction
  glm::vec3 direction = glm::normalize( lookingAt - camera );
  glm::vec3 up( 0.0f, 0.0f, 1.0f );
  sf::Vector2i center( 320, 240 );
  GLfloat yaw = 0.0f;
  GLfloat pitch = 0.0f;

  glm::vec3 cubes[] = {
    glm::vec3( 0.0f, 0.0f, -950.0f ),
    glm::vec3( 0.0f, 0.0f, -850.0f ),

    glm::vec3( -100.0f, -100.0f, -950.0f ),
    glm::vec3( -500.0f, -500.0f, -950.0f ),
    glm::vec3( -700.0f, -700.0f, -950.0f ),
    glm::vec3( -1000.0f, -1000.0f, -950.0f )
  };

  bool ortho = false;
  mainWindow.setMouseCursorVisible( ortho );

  while( mainWindow.isOpen() ) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader.use();

    // Adjust camera
    glm::mat4 view;
    // If you are in perspective mode:
    // - Your camera position will not change without moving around.
    view = glm::lookAt( camera, camera + direction, up );

    // Apply correct projection (to have real-world perspective)
    glm::mat4 projection;
    projection = ortho ? glm::ortho( -320.0f, 320.0f, -240.0f, 240.0f, 0.0f, 2000.0f ) : glm::perspective( 45.0f, (float)640/(float)480, 0.1f, 2000.0f );

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
      for( int i = 0; i != 6; i++ ) {
        glm::mat4 model;
        model = glm::translate( model, cubes[ i ] );
        glUniformMatrix4fv( uModel, 1, GL_FALSE, glm::value_ptr( model ) );

        glDrawArrays( GL_TRIANGLES, 0, 36 );
      }
    glBindVertexArray( 0 );

    mainWindow.pushGLStates();
      text.setString( ortho ? "Isometric" : "First-person" );
      std::stringstream s;
      s << camera.x << ", " << camera.y << ", " << camera.z;
      coords.setString( s.str().c_str() );
      mainWindow.draw( text );
      mainWindow.draw( coords );
    mainWindow.popGLStates();

    mainWindow.display();

    if( !ortho ) {
      sf::Vector2i mouseDelta = sf::Mouse::getPosition( mainWindow ) - center;

      if( !( mouseDelta.x == 0 && mouseDelta.y == 0 ) ) {
        // the yaw and pitch have to get done
        GLfloat xOffset = -mouseDelta.x * 0.07f;
        GLfloat yOffset = -mouseDelta.y * 0.07f;

        // Flip because our world is ass-backwards
        yaw += xOffset;
        pitch += yOffset;

        if( pitch > 89.0f ) {
          pitch = 89.0f;
        }
        if( pitch < -89.0f ) {
          pitch = -89.0f;
        }

        std::cout << "Yaw: " << yaw << " Pitch: " << pitch << std::endl;
        glm::vec3 newDirection(
          cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ),
          sin( glm::radians( pitch ) ),
          sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) )
        );
        direction = glm::normalize( newDirection );
      }

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
          if( ortho ) {
            ortho = false;
            mainWindow.setMouseCursorVisible( false );
          } else {
            ortho = true;
            mainWindow.setMouseCursorVisible( true );
          }
        }

        if( event.key.code == sf::Keyboard::Up ) {
          //worldPosition.y = worldPosition.y + 10.0f;
          camera.y = camera.y + 10.0f;
        }

        if( event.key.code == sf::Keyboard::Down ) {
          //worldPosition.y = worldPosition.y - 10.0f;
          camera.y = camera.y - 10.0f;
        }

        if( event.key.code == sf::Keyboard::Right ) {
          //worldPosition.x = worldPosition.x + 10.0f;
          camera.x = camera.x + 10.0f;
        }

        if( event.key.code == sf::Keyboard::Left ) {
          //worldPosition.x = worldPosition.x - 10.0f;
          camera.x = camera.x - 10.0f;
        }
      }
    }
  }
  return 0;
}
