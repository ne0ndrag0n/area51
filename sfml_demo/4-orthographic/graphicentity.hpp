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

    void prepareInstanceRecursive( const GFXModel& node ) {

    }


  public:
    GFXTransform transform;
    std::map< std::string, std::shared_ptr< GFXModel > > children;
    GLuint shaderProgram;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : model( model ), shaderProgram( shaderProgram ) {
      prepareInstanceRecursive( model );
    }
    GFXInstance( const GFXModel& model, GLuint shaderProgram, GFXTransform& parentTransform ) : model( model ), shaderProgram( shaderProgram ), transform( parentTransform ) {
      prepareInstanceRecursive( model );
    }

    /**
     * Traverse this tree and draw all the models within
     */
    void drawEntity() {

    }
};

#endif
