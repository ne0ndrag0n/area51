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
#include "gfxtransform.hpp"

/**
 * A GFXInstance is a specific instance of a graphic model placed on a lot. It contains
 * a position vector and other associated states (depending on the type of model).
 */
class GFXInstance {

  private:
    // The referred-to model
    const GFXModel& model;

  public:
    GFXTransform baseTransform;
    GLuint shaderProgram;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : model( model ), shaderProgram( shaderProgram ) {}

    void drawEntity() {
      baseTransform.apply( shaderProgram );
      for( auto& pair : model.meshes ) {
        auto& mesh = *( pair.second );
        mesh.draw( shaderProgram );
      }
    }
};

#endif
