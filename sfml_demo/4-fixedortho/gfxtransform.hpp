#ifndef GFXTRANSFORM
#define GFXTRANSFORM

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

class GFXTransform {

  public:
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
    glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );
    glm::vec3 rotationAxes = glm::vec3( 0.0f, 0.0f, 1.0f );
    GLfloat rotationAngle = 0.0f;
    glm::mat4 matrix;

    GFXTransform() {}

    glm::mat4 sendToShader( GLuint shaderProgram, glm::mat4& mixin, bool& recalculate ) {

      if( recalculate ) {
        matrix = glm::mat4();

        // Mixin the parent matrix
        matrix = matrix * mixin;

        // There's always going to be a translation
        matrix = glm::translate( matrix, position );

        // Then rotate
        matrix = glm::rotate( matrix, rotationAngle, rotationAxes );

        // Then scale
        matrix = glm::scale( matrix, scale );

        recalculate = false;
      }

      // Set the uniform for the shader
      glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "model" ), 1, GL_FALSE, glm::value_ptr( matrix ) );

      return matrix;
    }
};

#endif
