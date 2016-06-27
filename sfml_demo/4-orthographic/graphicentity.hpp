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
 * A GraphicEntity is a specific instance of a graphic model placed on a lot. It contains
 * a position vector and other associated states (depending on the type of model).
 */
class GraphicEntity {

  private:
    // The referred-to model
    const Model* model;

  public:
    glm::vec3 position;
    glm::vec3 scale = glm::vec3( 1.0, 1.0, 1.0 );

    GraphicEntity( const Model* model ) : model( model ) {}
};

#endif
