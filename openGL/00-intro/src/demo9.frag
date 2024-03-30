// demo9 fragment shader

#version 300 es


in mediump vec3 colour;
out mediump vec4 fragColour;


void main()

{
  fragColour = vec4( colour, 1.0 );
}
