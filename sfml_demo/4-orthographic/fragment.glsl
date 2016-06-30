#version 330 core
in vec2 fragTexture;

out vec4 color;

uniform sampler2D texture0;

void main() {
  color = texture( texture0, fragTexture );
}
