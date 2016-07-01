#ifndef MODELINSTANCE
#define MODELINSTANCE

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

/**
 * A GFXInstance is a specific instance of a graphic model placed on a lot. It contains
 * a position vector and other associated states (depending on the type of model).
 */
class GFXInstance {

  private:
    // The referred-to model
    const GFXModel& model;

  public:
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
    glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );
    glm::vec3 rotationAxes = glm::vec3( 0.0f, 0.0f, 0.0f );
    GLfloat rotationAngle = 0.0f;
    glm::mat4 transform;
    GLuint shaderProgram;
    // Performance-related switches
    bool applyScale = false;
    bool applyRotation = false;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : model( model ), shaderProgram( shaderProgram ) {}

    /**
     * Apply all transformation matricies to the object given translation, scale, and rotation
     * glm::translate, glm::rotate, then glm::scale (the actual multiplication is done backwards).
     *
     * The call to position() sets the vertex shader uniform for the vertex position. Call it directly
     * before a draw.
     */
    void setTransformUniform() {
      transform = glm::mat4();

      // There's always going to be a translation
      transform = glm::translate( transform, position );

      // Then rotate
      if( applyRotation ) {
        transform = glm::rotate( transform, rotationAngle, rotationAxes );
      }

      // Then scale
      if( applyScale ) {
        transform = glm::scale( transform, scale );
      }

      // Set the uniform for the shader
      glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "model" ), 1, GL_FALSE, glm::value_ptr( transform ) );
    }

    void drawEntity() {
      // Something will need to be made to set up a transformation mesh for each submesh
      setTransformUniform();
      for( auto& pair : model.meshes ) {
        auto& mesh = *( pair.second );
        mesh.draw( shaderProgram );
      }
    }
};

#endif
