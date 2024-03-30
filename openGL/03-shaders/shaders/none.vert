// no shading
//

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS
uniform vec3 lightDir;          // direction toward the light (in the VCS)

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

flat out mediump vec3 colour;		// flat!

void main()

{
  // calc vertex position in CCS

  gl_Position = MVP * vec4( vertPosition, 1.0f );

  // assign (r,g,b) colour

  colour = 2.0 * vec3( 0.33, 0.42, 0.18 );
}
