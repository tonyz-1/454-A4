// normals vertex shader

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

smooth out mediump vec3 normal;

void main()

{
  gl_Position = MVP * vec4( vertPosition, 1.0f );

  //normal = vertNormal;		// normal in the OCS

  normal = vec3( MV * vec4( vertNormal, 0.0 ) ); // normal in the VCS
}
