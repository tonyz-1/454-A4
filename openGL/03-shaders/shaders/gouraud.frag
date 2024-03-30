// Gouraud shading
//
// The fragment shader just passes along the colour that is
// interpolated from the vertices.

#version 300 es

smooth in mediump vec3 colour;	    // smooth = linear interpolation of colour
out mediump vec4 outputColour;

void main()

{
  outputColour = vec4( colour, 1.0f );
}
