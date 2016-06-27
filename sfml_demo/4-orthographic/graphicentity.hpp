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
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
    glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );
    glm::vec3 rotationAxes = glm::vec3( 0.0f, 0.0f, 0.0f );
    GLfloat rotationAngle = 0.0f;
    // Performance-related switches
    bool applyScale = false;
    bool applyRotation = false;

    GraphicEntity( const Model* model ) : model( model ) {}
};

#endif
