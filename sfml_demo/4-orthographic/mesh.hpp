#ifndef WORLDOBJECT
#define WORLDOBJECT

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh {

  private:
    GLuint VAO, VBO, EBO;
    // Meshes depend on OpenGL global states - You really shouldn't be copying 'em.
    Mesh( const Mesh& );
    Mesh& operator=( const Mesh& );

  public:
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 textureCoordinates;
    };

    struct Texture {
      Texture( GLuint id, aiString path ) :
        id( id ), path( path ) {}
      GLuint id;
      aiString path;
    };

    using Index = GLuint;
    using TextureList = std::vector< std::shared_ptr< Texture > >;
    using TextureBundle = std::map< std::string, TextureList >;

    std::vector< Vertex > vertices;
    std::vector< Index > indices;
    TextureBundle materialTextures;

    Mesh( std::vector< Vertex > vertices, std::vector< Index > indices, TextureBundle materialTextures ) :
      vertices( vertices ), indices( indices ), materialTextures( materialTextures ) {
      setupMesh();
    }

    ~Mesh() {
      glDeleteVertexArrays( 1, &VAO );
      glDeleteBuffers( 1, &VBO );
      glDeleteBuffers( 1, &EBO );
    }

    void setupMesh() {
      glGenVertexArrays( 1, &VAO );
      glGenBuffers( 1, &VBO );
      glGenBuffers( 1, &EBO );

      glBindVertexArray( VAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
          glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vertex ), &vertices[ 0 ], GL_STATIC_DRAW );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
          glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( Index ), &indices[ 0 ], GL_STATIC_DRAW );

        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) 0 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) offsetof( Vertex, normal ) );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) offsetof( Vertex, textureCoordinates ) );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

      glBindVertexArray( 0 );
    }

    void draw( GLuint shaderProgram ) {
      for( auto& pair : materialTextures ) {
        auto& type = pair.first;
        auto& textures = pair.second;
        auto numTextures = textures.size();

        for( int i = 0; i != numTextures; i++ ) {
          glActiveTexture( GL_TEXTURE0 + i );
            std::stringstream stream;
            stream << type << i;
            std::string uniformName = stream.str();
            glBindTexture( GL_TEXTURE_2D, textures[ i ]->id );
            glUniform1i( glGetUniformLocation( shaderProgram, uniformName.c_str() ), i );
        }
      }

      glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
      glBindVertexArray( 0 );
    }
};

#endif
