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
#include "drawable.hpp"

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
    std::map< std::string, Drawable > drawables;
    std::map< std::string, std::unique_ptr< GFXModel > > children;

    GFXModel( std::string path ) {
      loadModel( path );
    }
    // Used internally to generate child nodes
    GFXModel( aiNode* node, const aiScene* scene, std::string& directory, aiMatrix4x4 parentTransform ) : directory( directory ) {
      processNode( node, scene, parentTransform );
    }

  private:
    std::string directory;

    glm::mat4 aiToGLMmat4( aiMatrix4x4& matrix ) {
      glm::mat4 result;

      result[ 0 ] = glm::vec4( matrix[ 0 ][ 0 ], matrix[ 1 ][ 0 ], matrix[ 2 ][ 0 ], matrix[ 3 ][ 0 ] );
      result[ 1 ] = glm::vec4( matrix[ 0 ][ 1 ], matrix[ 1 ][ 1 ], matrix[ 2 ][ 1 ], matrix[ 3 ][ 1 ] );
      result[ 2 ] = glm::vec4( matrix[ 0 ][ 2 ], matrix[ 1 ][ 2 ], matrix[ 2 ][ 2 ], matrix[ 3 ][ 2 ] );
      result[ 3 ] = glm::vec4( matrix[ 0 ][ 3 ], matrix[ 1 ][ 3 ], matrix[ 2 ][ 3 ], matrix[ 3 ][ 3 ] );

      return result;
    }

    glm::vec4 aiToGLMvec4( aiVector3D& vector ) {
      glm::vec4 result;

      result.x = vector.x;
      result.y = vector.y;
      result.z = vector.z;
      result.w = 1.0f;

      return result;
    }

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
        processNode( scene->mRootNode->mChildren[ 0 ], scene, aiMatrix4x4() );
      } else {
        processNode( scene->mRootNode, scene, aiMatrix4x4() );
      }
    }

    void processNode( aiNode* node, const aiScene* scene, aiMatrix4x4 parentTransform ) {
      std::cout << "Processing " << node->mName.C_Str() << std::endl;

      aiMatrix4x4 resultantTransform = parentTransform * node->mTransformation;

      for( int i = 0; i < node->mNumMeshes; i++ ) {
        aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
        std::cout << '\t' << "Loading mesh " << mesh->mName.C_Str() << std::endl;
        this->processMesh( mesh, scene, node->mName.C_Str(), aiToGLMmat4( resultantTransform ) );
      }

      for( int i = 0; i < node->mNumChildren; i++ ) {
        children.emplace( node->mChildren[ i ]->mName.C_Str(), std::make_unique< GFXModel >( node->mChildren[ i ], scene, directory, resultantTransform ) );
      }

      std::cout << "Done with " << node->mName.C_Str() << std::endl;
    }

    void processMesh( aiMesh* mesh, const aiScene* scene, std::string nodeTitle, glm::mat4 transformation ) {
      std::vector< Vertex > vertices;
      std::vector< Index > indices;

      std::shared_ptr< GFXMaterial > defaultMaterial;

      for( int i = 0; i < mesh->mNumVertices; i++ ) {
        Vertex vertex;
        vertex.position = glm::vec3( transformation * aiToGLMvec4( mesh->mVertices[ i ] ) );
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

        defaultMaterial = std::make_shared< GFXMaterial >( loadMaterialTextures( material, aiTextureType_DIFFUSE ) );
      }

      drawables.emplace( nodeTitle, Drawable( std::make_shared< Mesh >( vertices, indices ), defaultMaterial ) );
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
