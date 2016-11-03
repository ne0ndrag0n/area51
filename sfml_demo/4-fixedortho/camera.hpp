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
    glm::vec3 lookingAt = glm::vec3( 0.0f, 0.0f, -10.0f );
    glm::vec3 camera = glm::vec3( 0, 0, -10.0f );
    glm::vec3 originalDirection = glm::vec3( glm::normalize( lookingAt - camera ) );
    glm::vec3 up = glm::vec3( 0.0f, 0.0f, 1.0f );
    GLfloat yaw = 0.0f;
    GLfloat pitch = 0.0f;
    const glm::vec2 rotations[4] = {
      glm::vec2( -1.0f, -1.0f ),
      glm::vec2( -1.0f, 1.0f ),
      glm::vec2( 1.0f, 1.0f ),
      glm::vec2( 1.0f, -1.0f )
    };
    GLuint currentRotation = 0;

    float perspectiveAspectRatio;
    float widthHalf;
    float heightHalf;
    float zoom = 1.0f;
    const float zoomIncrement = 0.25f;
    bool dirty = true;

    void doRotate() {
      glm::vec2 quadrant = rotations[ currentRotation ];

      camera.x = cameraHeight * quadrant.x;
      camera.y = cameraHeight * quadrant.y;
      direction = originalDirection = glm::vec3( glm::normalize( lookingAt - camera ) );
      dirty = true;
    }

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
      dirty = true;
    }

    bool setOrthographic( bool flag ) {
      ortho = flag;

      if( ortho ) {
        camera = glm::vec3( 0.0f, 0.0f, -10.0f );
      } else {
        camera = glm::vec3( -cameraHeight, -cameraHeight, lookingAt.z + cameraHeight );
      }

      dirty = true;
      return ortho;
    }

    float zoomIn() {
      if( zoom != 1.0f ) {
        zoom -= zoomIncrement;
        dirty = true;
      }

      return zoom;
    }

    float zoomOut() {
      if( zoom != 3.0f ) {
        zoom += zoomIncrement;
        dirty = true;
      }

      return zoom;
    }

    float setZoom( float zoomSetting ) {
      dirty = true;
      return zoom = zoomSetting;
    }

    void position() {
      if( dirty ) {
        // getOrthoView for now
        view = getOrthoView();
        projection = ortho ?
           getOrthoMatrix() :
           glm::perspective( 45.0f, perspectiveAspectRatio, 0.1f, 50.0f );

        dirty = false;
      }
      // Set uniforms
      glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
      glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    }

    glm::mat4 getOrthoView() {
      glm::mat4 view;

      view = glm::translate( view, camera );

      view = glm::rotate( view, glm::radians( -60.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
      view = glm::rotate( view, glm::radians( 45.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

      return view;
    }

    glm::mat4 getOrthoMatrix() {
      glm::mat4 ortho;
      GLfloat scaledWidthHalf = ( widthHalf * zoom ) / 100.0f;
      GLfloat scaledHeightHalf = ( heightHalf * zoom ) / 100.0f;

      ortho = glm::ortho( -scaledWidthHalf, scaledWidthHalf, -scaledHeightHalf, scaledHeightHalf, -20.0f, 50.0f );

      return ortho;
    }

    void walkForward() {
      camera += glm::vec3( direction.x * 0.1f, direction.y * 0.1f, 0.0f );
      dirty = true;
    }

    void walkBackward() {
      camera -= glm::vec3( direction.x * 0.1f, direction.y * 0.1f, 0.0f );
      dirty = true;
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
        dirty = true;
      }
    }

    void rotateRight() {
      if( ortho ) {
        currentRotation = ( currentRotation - 1 ) % 4;

        doRotate();
      }
    }

    void rotateLeft() {
      if( ortho ) {
        currentRotation = ( currentRotation + 1 ) % 4;

        doRotate();
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
