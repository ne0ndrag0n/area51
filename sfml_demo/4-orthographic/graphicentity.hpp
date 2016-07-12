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
#include "drawable.hpp"

/**
 * A GFXInstance is a specific instance of a graphic model placed on a lot. It contains
 * a position vector and other associated states (depending on the type of model).
 */
class GFXInstance {

  private:
    GLuint shaderProgram;
    GFXTransform transform;
    bool dirty = true;

    void prepareInstanceRecursive( const GFXModel& model ) {
      // Copy the list of drawables
      drawables = model.drawables;

      for( auto& pair : model.children ) {
        auto& child = *( pair.second );

        // Hand down the same transform as the parent to this model
        children.emplace( pair.first, std::make_shared< GFXInstance >( child, shaderProgram ) );
      }
    }

  public:
    std::map< std::string, Drawable > drawables;
    std::map< std::string, std::shared_ptr< GFXInstance > > children;

    GFXInstance( const GFXModel& model, GLuint shaderProgram ) : shaderProgram( shaderProgram ) {
      prepareInstanceRecursive( model );
    }

    std::shared_ptr< GFXInstance > findChildByName( std::string name ) {
      return children[ name ];
    }

    void drawEntity() {
      // Pass an identity matrix to mix-in
      glm::mat4 identity;
      drawEntity( identity );
    }

    // Draw this entity's meshes as well as the meshes of all its children
    void drawEntity( glm::mat4& parent ) {
      glm::mat4 nextParent = transform.sendToShader( shaderProgram, parent, dirty );

      for( auto& pair : drawables ) {
        auto& drawable = pair.second;

        drawable.render( shaderProgram );
      }

      for( auto& pair : children ) {
        // Apply the same transform of the parent
        pair.second->drawEntity( nextParent );
      }
    }

    void markDirty() {
      dirty = true;

      for( auto& pair : children ) {
        pair.second->markDirty();
      }
    }

    glm::vec3 getPosition() {
      return transform.position;
    }

    void setPosition( const glm::vec3& position ) {
      transform.position = position;
      markDirty();
    }

    glm::vec3 getScale() {
      return transform.scale;
    }

    void setScale( const glm::vec3& scale ) {
      transform.scale = scale;
      markDirty();
    }

    glm::vec3 getRotationAxes() {
      return transform.rotationAxes;
    }

    void setRotationAxes( const glm::vec3& rotationAxes ) {
      transform.rotationAxes = rotationAxes;
      markDirty();
    }

    GLfloat getRotationAngle() {
      return transform.rotationAngle;
    }

    void setRotationAngle( GLfloat rotationAngle ) {
      transform.rotationAngle = rotationAngle;
      markDirty();
    }
};

#endif
