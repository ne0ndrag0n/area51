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
    GFXTransform transform;
    std::map< std::string, std::shared_ptr< Mesh > > meshes;
    std::map< std::string, std::shared_ptr< GFXInstance > > children;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : shaderProgram( shaderProgram ) {
      prepareInstanceRecursive( model );
    }

    std::shared_ptr< GFXInstance > findChildByName( std::string name ) {
      auto iterator = children.find( name );

      if( iterator != children.end() ) {
        return iterator->second;
      }
    }

    void drawEntity() {
      // Pass an identity matrix to mix-in
      glm::mat4 identity;
      drawEntity( identity );
    }

    // Draw this entity's meshes as well as the meshes of all its children
    void drawEntity( glm::mat4& parent ) {
      glm::mat4 nextParent = transform.sendToShader( shaderProgram, parent );

      for( auto& pair : meshes ) {
        auto& mesh = *( pair.second );

        mesh.draw( shaderProgram );
      }

      for( auto& pair : children ) {
        // Apply the same transform of the parent
        pair.second->drawEntity( nextParent );
      }
    }
};

#endif
