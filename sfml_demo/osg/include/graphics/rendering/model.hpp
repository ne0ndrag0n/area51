#ifndef RENDERING_MODEL
#define RENDERING_MODEL

#include "graphics/rendering/types.hpp"
#include "graphics/rendering/object.hpp"
#include <osg/Node>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Texture2D>
#include <osgAnimation/BasicAnimationManager>
#include <string>
#include <exception>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      class Model : public Object {
        osgAnimation::AnimationMap animationMap;
        osg::ref_ptr< osgAnimation::BasicAnimationManager > animationManager;

        struct AnimFinder : public osg::NodeVisitor {
          osg::ref_ptr< osgAnimation::BasicAnimationManager > animationManager;

          AnimFinder();
          void apply( osg::Node& node );
        };

        struct RigTransformImplSetter : public osg::NodeVisitor {
          RigTransformImplSetter();

          void apply( osg::Geode& geode );
          void apply( osg::Drawable* drawable );
        };

        void buildAnimationMap();

      public:
        class Texture {
          osg::ref_ptr< osg::Image > image;

        public:
          Texture( const std::string& filePath );
          Texture( osg::ref_ptr< osg::Image > image );

          void applyTo( Model& model, const std::string& nodeID, unsigned int unit = 0 ) const;

          struct InvalidTextureException : public std::exception {
            const char* what() const throw() {
              return "Could not load file for texture!";
            }
          };
        };

        Model( const std::string& path );

        void playAnimation( const std::string& animationID );

        struct TextureNotFoundException : public std::exception {
          const char* what() const throw() {
            return "Could not find single node or texture!";
          }
        };

        struct InvalidModelException : public std::exception {
          const char* what() const throw() {
            return "Could not load file for model!";
          }
        };
      };

    }
  }
}

#endif
