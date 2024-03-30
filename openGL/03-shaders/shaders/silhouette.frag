// Silhouette fragment shader
//
// Highlight the fragments on the silhouette

#version 300 es

smooth in mediump vec3 normal;
smooth in mediump vec3 position;

out mediump vec4 outputColour;

void main()

{
  mediump vec3 N = normalize(normal);	// normal in VCS
  mediump vec3 V = normalize(vec3(0.0,0.0,0.0) - position); // VCS direction from fragment position toward the viewpoint
  mediump float NdotV = abs(dot(N,V));

  if (abs(NdotV) < 0.2) // radians
    outputColour = vec4( 1.0, 0.0, 0.0, 1.0 );
  else
    outputColour = vec4( 1.0, 1.0, 1.0, 1.0 );
}
