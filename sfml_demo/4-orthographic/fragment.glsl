#version 330 core
in vec2 fragTexture;

out vec4 color;

uniform sampler2D alien;
uniform sampler2D alien2;

uniform bool isFloorTile;

void main() {
  color = texture( alien2, fragTexture );
}
