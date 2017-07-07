#ifndef RENDERING_MODEL
#define RENDERING_MODEL

#include "graphics/rendering/types.hpp"
#include "graphics/rendering/group.hpp"
#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osgAnimation/BasicAnimationManager>
#include <string>
#include <exception>

namespace BlueBear {
  namespace Graphics {
    namespace Rendering {

      class Model {
        osg::ref_ptr< osg::Node > model;
        osg::ref_ptr< osg::PositionAttitudeTransform > root;
        osgAnimation::AnimationMap animationMap;

        struct AnimFinder : public osg::NodeVisitor {
          osg::ref_ptr< osgAnimation::BasicAnimationManager > animationManager;

          AnimFinder();
          void apply( osg::Node& node );
        };

        void buildAnimationMap();

      public:
        Model( const std::string& path );

        void setPosition( const Vec3& position );
        Vec3 getPosition();

        void setAttitude( const Quat& attitude );
        Quat getAttitude();

        void setScale( const Vec3& scale );
        Vec3 getScale();

        friend void Group::addChild( const Model& model );
        friend void Group::removeChild( const Model& model );

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
