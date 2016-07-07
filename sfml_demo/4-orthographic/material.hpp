#ifndef GFXMATERIAL
#define GFXMATERIAL

#include <map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class GFXMaterial {

  public:
    class Texture {
      private:
        Texture( const Texture& );
        Texture& operator=( const Texture& );

      public:
        Texture( GLuint id, aiString path ) :
          id( id ), path( path ) {}
        GLuint id;
        aiString path;
    };

    using TextureList = std::vector< std::shared_ptr< Texture > >;

    TextureList diffuseTextures;
    TextureList specularTextures;

    GFXMaterial() {}
    GFXMaterial( TextureList diffuseTextures ) : diffuseTextures( diffuseTextures ) {}
    GFXMaterial( TextureList diffuseTextures, TextureList specularTextures ) : diffuseTextures( diffuseTextures ), specularTextures( specularTextures ) {}

    void sendToShader( GLuint shaderProgram ) {
      auto numTextures = diffuseTextures.size();

      for( int i = 0; i != numTextures; i++ ) {
        glActiveTexture( GL_TEXTURE0 + i );
          std::stringstream stream;
          stream << "diffuse" << i;
          std::string uniformName = stream.str();
          glBindTexture( GL_TEXTURE_2D, diffuseTextures[ i ]->id );
          glUniform1i( glGetUniformLocation( shaderProgram, uniformName.c_str() ), i );
      }
    }
};

#endif
