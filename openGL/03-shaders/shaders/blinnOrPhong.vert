// phong shader
//
// Nothing is done in the vertex shader.  All calculations are in the
// fragment shader.

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

smooth out mediump vec3 colour;
smooth out mediump vec3 normal;

void main()

{
  // calc vertex position in CCS

  gl_Position = MVP * vec4( vertPosition, 1.0f );

  // assign (r,g,b) colour

  colour = 2.0 * vec3( 0.33, 0.42, 0.18 );

  // calc normal in VCS
  //
  // To do this, apply the non-translational parts of MV to the model
  // normal.  The 0.0 as the fourth component of the normal ensures
  // that no translational component is added.

  normal = vec3( MV * vec4( vertNormal, 0.0 ) );
}
