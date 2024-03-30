// demo9 vertex shader

#version 330


uniform mat4 MVP;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColour;

out mediump vec3 colour;


void main()

{
  gl_Position = MVP * vec4( vertPosition, 1.0f );
  colour = vertColour;
}
