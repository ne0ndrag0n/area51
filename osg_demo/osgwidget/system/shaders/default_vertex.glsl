#version 330 core

// gl3_VertexShader
// From OpenSceneGraph/StateSet.cpp

in vec4 osg_Vertex;
in vec4 osg_Color;
in vec4 osg_MultiTexCoord0;

uniform mat4 osg_ModelViewProjectionMatrix;

out vec2 texCoord;
out vec4 vertexColor;

void main( void ) {
  gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
  texCoord = osg_MultiTexCoord0.xy;
  vertexColor = osg_Color;
}
