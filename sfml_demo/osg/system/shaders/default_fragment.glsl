#version 330 core

// gl3_FragmentShader
// From OpenSceneGraph/StateSet.cpp

uniform sampler2D baseTexture;

in vec2 texCoord;
in vec4 vertexColor;

out vec4 color;

void main( void ) {
  // Gets us to where we were before with the default Concordia shader
  color = /* vertexColor * */ texture( baseTexture, texCoord );
}
