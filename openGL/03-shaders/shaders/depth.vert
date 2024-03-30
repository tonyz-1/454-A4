// show the depth of each pixel
// 
// Calculate depth per-vertex from the vertex position in the CCS

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

smooth out mediump float depth;

void main()

{
  // calc vertex position in CCS

  vec4 ccs_pos = MVP * vec4( vertPosition, 1.0f );

  gl_Position = ccs_pos;

  depth = 0.5 * ((ccs_pos.z / ccs_pos.w) + 1.0);
}
