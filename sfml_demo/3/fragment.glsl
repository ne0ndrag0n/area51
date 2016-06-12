#version 330 core
in vec3 fragColor;
in vec2 fragTexture;

out vec4 color;

uniform sampler2D alien;

void main() {
  color = texture( alien, fragTexture );
}
