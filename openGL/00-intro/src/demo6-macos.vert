// demo6 vertex shader
//
// Attribute 0 is position, which gets transformed by the 4x4 matrix M
// Attribute 1 is colour, which gets passed to the fragment shader

#version 330

uniform mat4 M;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColour;

out mediump vec3 colour;


void main()

{
  gl_Position = M * vec4( vertPosition, 1 );

  colour = vertColour;
}
