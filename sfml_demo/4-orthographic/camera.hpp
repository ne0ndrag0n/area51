#ifndef CAMERA
#define CAMERA

#include <string>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

class LotCamera {

  private:
    glm::mat4 view;
    glm::mat4 projection;
    GLuint program;

    GLfloat cameraHeight = 3.0f;
    glm::vec3 lookingAt = glm::vec3( 0.0f, 0.0f, -9.0f );
    glm::vec3 camera = glm::vec3( -cameraHeight, -cameraHeight, lookingAt.z + cameraHeight );
    const glm::vec3 originalDirection = glm::vec3( glm::normalize( lookingAt - camera ) );
    glm::vec3 up = glm::vec3( 0.0f, 0.0f, 1.0f );
    GLfloat yaw = 0.0f;
    GLfloat pitch = 0.0f;

    float perspectiveAspectRatio;
    float widthHalf;
    float heightHalf;
    float zoom = 1.0f;
    const float zoomIncrement = 0.25f;

  public:
    glm::vec3 direction = originalDirection;
    bool ortho = true;

    LotCamera( GLuint program, int screenWidth, int screenHeight ) : program( program ) {
      widthHalf = ( (float)screenWidth / 2 );
      heightHalf = ( (float)screenHeight / 2 );
      perspectiveAspectRatio = (float)screenWidth / (float)screenHeight;
    }

    void move( GLfloat x, GLfloat y, GLfloat z ) {
      camera += glm::vec3( x, y, z );
    }

    bool setOrthographic( bool flag ) {
      ortho = flag;

      if( ortho ) {
        direction = originalDirection;
      }

      return ortho;
    }

    float zoomIn() {
      if( zoom != 1.0f ) {
        zoom -= zoomIncrement;
      }

      return zoom;
    }

    float zoomOut() {
      if( zoom != 3.0f ) {
        zoom += zoomIncrement;
      }

      return zoom;
    }

    float setZoom( float zoomSetting ) {
      return zoom = zoomSetting;
    }

    void position() {
      GLfloat scaledWidthHalf = ( widthHalf * zoom ) / 100.0f;
      GLfloat scaledHeightHalf = ( heightHalf * zoom ) / 100.0f;

      view = glm::lookAt( camera, camera + direction, up );
      projection = ortho ?
         glm::ortho( -scaledWidthHalf, scaledWidthHalf, -scaledHeightHalf, scaledHeightHalf, -20.0f, 50.0f ) :
         glm::perspective( 45.0f, perspectiveAspectRatio, 0.1f, 50.0f );
      // Set uniforms
      glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
      glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    }

    void updateFirstPersonView( int deltaX, int deltaY ) {
      if( !( deltaX == 0 && deltaY == 0 ) ) {
        // These are the correct settings for an up normal of Y, not Z
        // Adjust them to work for an up normal of Z...
        GLfloat xOffset = -deltaX * 0.07f;
        GLfloat yOffset = -deltaY * 0.07f;

        // Flip because our world is ass-backwards
        yaw += xOffset;
        pitch += yOffset;

        if( pitch > 89.0f ) {
          pitch = 89.0f;
        }
        if( pitch < -89.0f ) {
          pitch = -89.0f;
        }

        glm::vec3 newDirection(
          cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ),
          sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) ),
          sin( glm::radians( pitch ) )
        );
        direction = glm::normalize( newDirection );
      }
    }

    std::string positionToString() {
      std::stringstream s;
      s << "Camera: " << camera.x << ", " << camera.y << ", " << camera.z;
      return s.str();
    }

    std::string directionToString() {
      std::stringstream s;
      s << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
      return s.str();
    }
};

#endif
