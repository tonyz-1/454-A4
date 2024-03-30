// Depth fragment shader
//
// Show the depth of each fragment

#version 300 es

out mediump vec4 outputColour;
smooth in mediump float depth;

void main()

{
  outputColour = vec4( depth, depth, depth, 1.0 );
}	
