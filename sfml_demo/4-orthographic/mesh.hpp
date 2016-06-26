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
    // Set origin at the time the mesh is added to the model
    // This is a pointer to the model's position in the world
    const glm::vec3* origin;
    glm::vec3 localPosition;
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
      GLuint id;
      std::string type;
      aiString path;
    };

    using Index = GLuint;

    std::vector< Vertex > vertices;
    std::vector< Index > indices;
    std::vector< Texture > textures;

    Mesh( std::vector< Vertex > vertices, std::vector< Index > indices, std::vector< Texture > textures, const glm::vec3* origin ) :
      vertices( vertices ), indices( indices ), textures( textures ), origin( origin ) {
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

    void setRelativePosition( glm::vec3 localPosition ) {
      localPosition = *origin + localPosition;
    }

    void setShaderUniform( Shader shader ) {
      glm::mat4 model;
      model = glm::translate( model, localPosition );
      glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    }

    void draw( Shader shader ) {
      auto numTextures = textures.size();

      if( numTextures > 0 ) {
        for( int i = 0; i != numTextures; i++ ) {
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
    }
};

#endif
