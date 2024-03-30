// demo9 fragment shader

#version 330


in mediump vec3 colour;
out mediump vec4 fragColour;


void main()

{
  fragColour = vec4( colour, 1.0 );
}
