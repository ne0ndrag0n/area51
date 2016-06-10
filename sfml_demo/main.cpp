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
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  sf::RenderWindow mainWindow(
    sf::VideoMode( 640, 480 ),
    "ne0ndrag0n area51 - SFML & OpenGL Training",
    sf::Style::Close
  );

  mainWindow.setVerticalSyncEnabled( true );

  while( mainWindow.isOpen() ) {
    sf::Event event;
    while( mainWindow.pollEvent( event ) ) {
      if( event.type == sf::Event::Closed ) {
        mainWindow.close();
      }
    }

    mainWindow.clear( sf::Color::Black );
    mainWindow.display();
  }
  return 0;
}
