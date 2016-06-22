#ifndef WORLDOBJECT
#define WORLDOBJECT

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
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

  public:
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 textureCoordinates;
    };

    struct Texture {
      GLuint id;
      std::string type;
      aiString path;
    };

    using Index = GLuint;

    std::vector< Vertex > vertices;
    std::vector< Index > indices;
    std::vector< Texture > textures;

    Mesh( std::vector< Vertex > vertices, std::vector< Index > indices, std::vector< Texture > textures ) :
      vertices( vertices ), indices( indices ), textures( textures ) {
      setupMesh();
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
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) 0 );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) offsetof( Vertex, normal ) );
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( GLvoid* ) offsetof( Vertex, textureCoordinates ) );

      glBindVertexArray( 0 );
    }

    void draw( Shader shader ) {
        for( int i = 0; i != textures.size(); i++ ) {
          glActiveTexture( GL_TEXTURE0 + i );
            std::stringstream stream;
            stream << "texture" << i;
            std::string uniformName = stream.str();
            glBindTexture( GL_TEXTURE_2D, textures[ i ].id );
            glUniform1i(
              glGetUniformLocation( shader.Program, uniformName.c_str() ), i
            );
        }

        glBindVertexArray( VAO );
          glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
    }
};

#endif
