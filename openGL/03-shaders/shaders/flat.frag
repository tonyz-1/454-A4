// flat shading
//
// Just use the colour provided from one vertex

#version 300 es

flat in mediump vec3 colour;		// flat!
out mediump vec4 outputColour;

void main()

{
  outputColour = vec4( colour, 1.0f );
}


