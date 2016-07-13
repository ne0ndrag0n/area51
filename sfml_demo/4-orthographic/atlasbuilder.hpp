#ifndef ATLASBUILDER
#define ATLASBUILDER

#include <istream>
#include <string>
#include <jsoncpp/json/json.h>
#include <SFML/Graphics.hpp>
#include <exception>
#include <utility>
#include <memory>
#include "gfxtexture.hpp"

/**
 * Builds a texture atlas from a given JSON schema and a base image.
 */
class AtlasBuilder {

  private:
    struct AtlasMapping {
      unsigned int x;
      unsigned int y;
      unsigned int width;
      unsigned int height;
      std::string imagePath;
    };

    sf::Image base;

  public:
    struct CannotLoadImageException : public std::exception { const char* what () const throw () { return "C++ Exception"; } };

    std::map< std::string, AtlasMapping > mappings;

    AtlasBuilder( std::string& path ) {
      std::ifstream schemaFile( path );

      Json::Value schema;
      Json::Reader reader;

      reader.parse( schemaFile, schema );

      std::string baseImagePath = schema[ "base" ].asString();
      Json::Value components = schema[ "mappings" ];

      // Load base atlas into an sf::Image
      // This atlas will contain an image where we can overlay additional images
      // to create a new texture atlas in the shape of the old one
      if( !base.loadFromFile( baseImagePath ) ) {
        throw CannotLoadImageException();
      }

      // Load all components into mappings
      for( Json::Value::iterator jsonIterator = components.begin(); jsonIterator != components.end(); ++jsonIterator ) {
        std::string key = jsonIterator.key().asString();
        Json::Value value = *jsonIterator;

        mappings[ key ] = AtlasMapping{
          ( unsigned int ) value[ "x" ].asInt(),
          ( unsigned int ) value[ "y" ].asInt(),
          ( unsigned int ) value[ "width" ].asInt(),
          ( unsigned int ) value[ "height" ].asInt(),
          ""
        };
      }
    }

    std::shared_ptr< GFXTexture > getTextureAtlas() {
      sf::Image atlasBase = base;

      // Apply each overlay
      for( auto& pair : mappings ) {
        sf::Image overlay;
        AtlasMapping mapping = pair.second;

        if( !overlay.loadFromFile( mapping.imagePath ) ) {
          throw CannotLoadImageException();
        }

        atlasBase.copy( overlay, mapping.x, mapping.y );
      }

      // Overlay this sf::Image into an OpenGL texture
      return std::make_shared< GFXTexture >( atlasBase );
    }
};

#endif
