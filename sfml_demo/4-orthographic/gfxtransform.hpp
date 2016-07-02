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
    glm::vec3 rotationAxes = glm::vec3( 0.0f, 0.0f, 0.0f );
    GLfloat rotationAngle = 0.0f;
    glm::mat4 matrix;
    // Performance-related switches
    bool applyScale = false;
    bool applyRotation = false;

    GFXTransform() {}

    /**
     * Compose a transform by adding together each of its components: position, scale, rotation axis, and rotation angle.
     *
     * The new GFXTransform will inherit the applyScale and applyRotation of the right hand side
     */
    friend GFXTransform operator+( const GFXTransform& left, const GFXTransform& right ) {
      GFXTransform result;
      result.position = left.position + right.position;
      result.scale = left.scale + right.scale;
      result.rotationAxes = right.rotationAxes;
      result.rotationAngle = left.rotationAngle + right.rotationAngle;

      result.applyScale = right.applyScale;
      result.applyRotation = right.applyRotation;

      return result;
    }

    void apply( GLuint shaderProgram ) {
      matrix = glm::mat4();

      // There's always going to be a translation
      matrix = glm::translate( matrix, position );

      // Then rotate
      if( applyRotation ) {
        matrix = glm::rotate( matrix, rotationAngle, rotationAxes );
      }

      // Then scale
      if( applyScale ) {
        matrix = glm::scale( matrix, scale );
      }

      // Set the uniform for the shader
      glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "model" ), 1, GL_FALSE, glm::value_ptr( matrix ) );
    }
};

#endif
