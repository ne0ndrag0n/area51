#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 color;	// The color variable has attribute position 1
layout (location = 2) in vec2 texture;

out vec3 fragColor; // Output a color to the fragment shader
out vec2 fragTexture;

void pass() {
  fragColor = color;
  fragTexture = texture;
}

void main()
{
    gl_Position = vec4(position, 1.0);
    pass();
}
