#ifndef DRAWABLE
#define DRAWABLE

#include <memory>
#include <GL/glew.h>
#include "mesh.hpp"
#include "material.hpp"


class Drawable {

  public:
    std::shared_ptr< Mesh > mesh;
    std::shared_ptr< GFXMaterial > material;

    Drawable( std::shared_ptr< Mesh > mesh, std::shared_ptr< GFXMaterial > material ) :
      mesh( mesh ), material( material ) {}

    void render( GLuint shaderID ) {
      material->sendToShader( shaderID );
      mesh->drawElements();
    }
};

#endif
