// demo6 fragment shader

#version 330


// Incoming attributes from the vertex shader are automatically
// *interpolated* between vertices by OpenGL.
//
// Vertex positions are interpolated to get the fragment position.
// Vertex colours are interpolated to get the colour.

in mediump vec3 colour;

out mediump vec4 fragColour;


void main()

{
  fragColour = vec4( colour, 1.0 );
}
