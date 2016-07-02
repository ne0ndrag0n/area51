#ifndef MODELINSTANCE
#define MODELINSTANCE

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
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
    std::map< std::string, GFXTransform > transforms;
    GLuint shaderProgram;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : model( model ), shaderProgram( shaderProgram ) {
      transforms.emplace( "_base", GFXTransform{} );
    }

    GFXTransform& getBaseTransform() {
      return transforms[ "_base" ];
    }

    void setBaseTransform( GFXTransform transform ) {
      transforms[ "_base" ] = transform;
    }

    void drawEntity() {
      for( auto& pair : model.meshes ) {
        // Prior to drawing individual meshes within a model, we need to reset the shader to our base transform
        auto baseTransform = getBaseTransform();
        baseTransform.apply( shaderProgram );

        auto& mesh = *( pair.second );
        // If there are additional transforms that need to be applied, apply them by adding them to the previous transform,
        // and then using the new transform
        mesh.draw( shaderProgram );
      }
    }
};

#endif
