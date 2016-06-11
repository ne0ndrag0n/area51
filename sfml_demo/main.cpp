#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <GL/glew.h>

const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(0.0f, 0.5f, 0.0f, 1.0f);\n"
    "}\n\0";

GLuint compileShaders() {
  GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
  glCompileShader( vertexShader );
  GLint success;
  GLchar infoLog[ 512 ];
  glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
  if ( !success ) {
      glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      return -1;
  }

  GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
  glCompileShader( fragmentShader );
  glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
  if ( !success ) {
      glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      return -1;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader( shaderProgram, vertexShader );
  glAttachShader( shaderProgram, fragmentShader );
  glLinkProgram( shaderProgram );

  glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
  if ( !success ) {
      glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      return -1;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

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

  GLuint program = compileShaders();

  glViewport( 0, 0, 640, 480 );

  GLfloat vertices[] = {
    0.5f,  0.5f, 0.0f,  // Top Right
    0.5f, -0.5f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
    -0.5f,  0.5f, 0.0f   // Top Left
  };
  GLuint indicies[] = { 0, 1, 3, 1, 2, 3 };

  GLuint VBO, VAO, EBO;
  glGenVertexArrays( 1, &VAO );
  glGenBuffers( 1, &VBO );
  glGenBuffers( 1, &EBO );

  glBindVertexArray( VAO );

  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid* ) 0 );
  glEnableVertexAttribArray( 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indicies ), indicies, GL_STATIC_DRAW );

  glBindVertexArray( 0 );

  while( mainWindow.isOpen() ) {
    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
        glDeleteVertexArrays( 1, &VAO );
        glDeleteBuffers( 1, &VBO );
        return 0;
      }
    }

    mainWindow.clear( sf::Color::Black );

    glUseProgram( program );
    glBindVertexArray( VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );

    mainWindow.display();
  }
  return 0;
}
