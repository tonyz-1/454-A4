// demo5 vertex shader
//
// The matrix M transforms points from their own object coordinate system (OCS)
// to the clipping coordinate system (CCS).

#version 300 es

uniform mat4 M;

layout (location = 0) in vec3 vertPosition;

out mediump vec3 colour;


void main()

{
  gl_Position = M * vec4( vertPosition, 1 );

  colour = vec3( 0.7, 0.2, 0.6 );
}
