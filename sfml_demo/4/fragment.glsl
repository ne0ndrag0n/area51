#version 330 core
in vec2 fragTexture;

out vec4 color;

uniform sampler2D alien;
uniform sampler2D alien2;

void main() {
  color = mix( texture( alien, fragTexture ), texture( alien2, fragTexture ), 0.2 );
}
