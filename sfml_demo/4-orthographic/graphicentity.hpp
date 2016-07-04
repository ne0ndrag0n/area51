#ifndef MODELINSTANCE
#define MODELINSTANCE

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "gfxtransform.hpp"
#include "mesh.hpp"

/**
 * A GFXInstance is a specific instance of a graphic model placed on a lot. It contains
 * a position vector and other associated states (depending on the type of model).
 */
class GFXInstance {

  private:
    GLuint shaderProgram;
    GFXTransform transform;

    void prepareInstanceRecursive( const GFXModel& model ) {
      // At this level, copy the list of meshes
      // shared_ptrs will be copied, incrementing the count
      meshes = model.meshes;

      for( auto& pair : model.children ) {
        auto& child = *( pair.second );

        // Hand down the same transform as the parent to this model
        children.emplace( pair.first, std::make_shared< GFXInstance >( child, shaderProgram ) );
      }
    }

  public:
    std::map< std::string, std::shared_ptr< Mesh > > meshes;
    std::map< std::string, std::shared_ptr< GFXInstance > > children;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : shaderProgram( shaderProgram ) {
      prepareInstanceRecursive( model );
    }

    std::shared_ptr< GFXInstance > findChildByName( std::string& name ) {
      auto iterator = children.find( name );

      if( iterator != children.end() ) {
        return iterator->second;
      }
    }

    void move( glm::vec3 position ) {
      // Set this node's position by adding to it - Everything is done relative to its current position (which starts out at 0.0f, 0.0f, 0.0f)
      transform.position += position;

      // Trickle this change down to its children
      for( auto& pair : children ) {
        pair.second->move( position );
      }
    }

    void scale( glm::vec3 by ) {
      transform.scale += by;

      for( auto& pair : children ) {
        pair.second->scale( by );
      }
    }

    void rotate( GLfloat angle, glm::vec3 axes ) {
      transform.rotationAngle += angle;
      transform.rotationAxes = axes;

      for( auto& pair : children ) {
        pair.second->rotate( angle, axes );
      }
    }

    void resetRotation() {
      transform.rotationAngle = 0.0f;
      transform.rotationAxes = glm::vec3( 0.0f, 0.0f, 0.0f );

      for( auto& pair : children ) {
        pair.second->resetRotation();
      }
    }

    void setApplyRotation( bool apply ) {
      transform.setRotate = apply;
    }

    void setApplyScale( bool apply ) {
      transform.setScale = apply;
    }

    glm::vec3 getPosition() {
      return transform.position;
    }

    glm::vec3 getScale() {
      return transform.scale;
    }

    std::pair< GLfloat, glm::vec3 > getRotation() {
      return std::make_pair( transform.rotationAngle, transform.rotationAxes );
    }

    // Draw this entity's meshes as well as the meshes of all its children
    void drawEntity() {
      transform.sendToShader( shaderProgram );

      for( auto& pair : meshes ) {
        auto& mesh = *( pair.second );

        mesh.draw( shaderProgram );
      }

      for( auto& pair : children ) {
        pair.second->drawEntity();
      }
    }
};

#endif
