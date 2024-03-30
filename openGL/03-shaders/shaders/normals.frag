// Normals fragment shader
//
// Use the (xyz) normal as the (rgb) colour of each fragment 

#version 300 es

out mediump vec4 outputColour;
smooth in mediump vec3 normal;

void main()

{
  outputColour = vec4( normalize(normal), 1.0 );
}	
