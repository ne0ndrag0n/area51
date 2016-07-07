#ifndef GFXTEXTURE
#define GFXTEXTURE

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

class GFXTexture {
  private:
    GFXTexture( const GFXTexture& );
    GFXTexture& operator=( const GFXTexture& );

  public:
    GFXTexture( GLuint id, aiString path ) :
      id( id ), path( path ) {}
    GFXTexture( std::string texFromFile ) {
      sf::Image texture;
      if( !texture.loadFromFile( texFromFile ) ) {
        std::cout << "Couldn't load texture " << texFromFile << std::endl;
        return;
      }

      glGenTextures( 1, &id );
      glBindTexture( GL_TEXTURE_2D, id );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        auto size = texture.getSize();
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr() );
        glGenerateMipmap( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, 0 );
    }
    ~GFXTexture() {
      glDeleteTextures( 1, &id );
    }
    GLuint id = -1;
    aiString path;
};

#endif
