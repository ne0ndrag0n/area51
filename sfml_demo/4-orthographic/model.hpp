#ifndef MODEL3D
#define MODEL3D

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
#include "mesh.hpp"

class Model {

  public:
    Model( std::string path ) {
      loadModel( path );
    }
    void draw( Shader shader ) {
      for( auto& meshPtr : meshes ) {
        meshPtr->draw( shader );
      }
    }

  private:
    std::vector< std::unique_ptr< Mesh > > meshes;
    std::string directory;

    void loadModel( std::string path ) {
      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals );

      if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cout << "Warning: Could not import file " << path << std::endl;
        return;
      }

      directory = path.substr( 0, path.find_last_of( '/' ) );

      processNode( scene->mRootNode, scene );
    }

    void processNode( aiNode* node, const aiScene* scene ) {
      for( int i = 0; i < node->mNumMeshes; i++ ) {
        aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
        this->processMesh( mesh, scene );
      }

      for( int i = 0; i < node->mNumChildren; i++ ) {
        processNode( node->mChildren[ i ], scene );
      }
    }

    void processMesh( aiMesh* mesh, const aiScene* scene ) {
      std::vector< Mesh::Vertex > vertices;
      std::vector< Mesh::Index > indices;
      std::vector< Mesh::Texture > textures;

      for( int i = 0; i < mesh->mNumVertices; i++ ) {
        Mesh::Vertex vertex;
        vertex.position = glm::vec3( mesh->mVertices[ i ].x, mesh->mVertices[ i ].y, mesh->mVertices[ i ].z );
        vertex.normal = glm::vec3( mesh->mNormals[ i ].x, mesh->mNormals[ i ].y, mesh->mNormals[ i ].z );
        if( mesh->mTextureCoords[ 0 ] ) {
          vertex.textureCoordinates = glm::vec2( mesh->mTextureCoords[ 0 ][ i ].x, mesh->mTextureCoords[ 0 ][ i ].y );
        } else {
          vertex.textureCoordinates = glm::vec2( 0.0f, 0.0f );
        }
        vertices.push_back( vertex );
      }

      for( int i = 0; i < mesh->mNumFaces; i++ ) {
        aiFace face = mesh->mFaces[ i ];
        for( int j = 0; j < face.mNumIndices; j++ ) {
          indices.push_back( face.mIndices[ j ] );
        }
      }

      if( mesh->mMaterialIndex >= 0 ) {
        aiMaterial* material = scene->mMaterials[ mesh->mMaterialIndex ];
        // Just do diffuse maps for now
        textures = loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
      }

      meshes.emplace_back( std::make_unique< Mesh >( vertices, indices, textures ) );
    }

    std::vector< Mesh::Texture > loadMaterialTextures( aiMaterial* material, aiTextureType type, std::string typeName ) {
      std::vector< Mesh::Texture > textures;

      auto texCount = material->GetTextureCount( type );
      for( int i = 0; i < texCount; i++ ) {
        aiString str;
        material->GetTexture( type, i, &str );
        Mesh::Texture texture{ textureFromFile( str.C_Str(), directory ), typeName, str };
        textures.push_back( texture );
      }

      return textures;
    }

    GLuint textureFromFile( std::string path, std::string directory ) {
      sf::Image texture;
      if( !texture.loadFromFile( directory + "/" + path ) ) {
        std::cout << "Couldn't load texture " << path << std::endl;
        return -1;
      }

      GLuint textureID;
      glGenTextures( 1, &textureID );
      glBindTexture( GL_TEXTURE_2D, textureID );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        auto size = texture.getSize();
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr() );
        glGenerateMipmap( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, 0 );

      return textureID;
    }
};

#endif
