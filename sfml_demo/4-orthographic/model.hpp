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
#include "material.hpp"
#include "gfxtexture.hpp"

/**
 * split c/p'd from bluebear
 */
std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

class GFXModel {

  public:
    std::map< std::string, std::shared_ptr< Mesh > > meshes;
    std::map< std::string, std::unique_ptr< GFXModel > > children;

    GFXModel( std::string path ) {
      loadModel( path );
    }
    // Used internally to generate child nodes
    GFXModel( aiNode* node, const aiScene* scene, std::string& directory ) : directory( directory ) {
      processNode( node, scene );
    }

  private:
    std::string directory;

    void loadModel( std::string path ) {
      Assimp::Importer importer;
      const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals );

      if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cout << "Warning: Could not import file " << path << std::endl;
        return;
      }

      directory = path.substr( 0, path.find_last_of( '/' ) );

      // If the root node has no meshes and only one child, just skip to that child
      if( scene->mRootNode->mNumChildren == 1 && scene->mRootNode->mNumMeshes == 0 ) {
        processNode( scene->mRootNode->mChildren[ 0 ], scene );
      } else {
        processNode( scene->mRootNode, scene );
      }
    }

    void processNode( aiNode* node, const aiScene* scene ) {
      std::cout << "Processing " << node->mName.C_Str() << std::endl;

      for( int i = 0; i < node->mNumMeshes; i++ ) {
        aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
        std::cout << '\t' << "Loading mesh " << mesh->mName.C_Str() << std::endl;
        this->processMesh( mesh, scene, node->mName.C_Str() );
      }

      for( int i = 0; i < node->mNumChildren; i++ ) {
        children.emplace( node->mChildren[ i ]->mName.C_Str(), std::make_unique< GFXModel >( node->mChildren[ i ], scene, directory ) );
      }

      std::cout << "Done with " << node->mName.C_Str() << std::endl;
    }

    void processMesh( aiMesh* mesh, const aiScene* scene, std::string nodeTitle ) {
      std::vector< Vertex > vertices;
      std::vector< Index > indices;
      GFXMaterial meshMaterial;

      for( int i = 0; i < mesh->mNumVertices; i++ ) {
        Vertex vertex;
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
        meshMaterial.diffuseTextures = loadMaterialTextures( material, aiTextureType_DIFFUSE );
      }

      meshes.emplace( nodeTitle, std::make_shared< Mesh >( vertices, indices, meshMaterial ) );
    }

    GFXMaterial::TextureList loadMaterialTextures( aiMaterial* material, aiTextureType type ) {
      GFXMaterial::TextureList textures;

      auto texCount = material->GetTextureCount( type );
      for( int i = 0; i < texCount; i++ ) {
        aiString str;
        material->GetTexture( type, i, &str );
        textures.push_back( std::make_shared< GFXTexture >( directory + "/" + str.C_Str() ) );
      }

      return textures;
    }
};

#endif
