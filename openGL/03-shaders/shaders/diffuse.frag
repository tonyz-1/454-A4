// diffuse fragment shader

#version 300 es

uniform mediump vec3 lightDir; // direction *toward* light in VCS

flat in mediump vec3 colour;
smooth in mediump vec3 normal;

out mediump vec4 outputColour;


void main()

{
  mediump float NdotL = dot( normalize(normal), lightDir );

  if (NdotL < 0.0)
    NdotL = 0.0;

  mediump vec3 diffuseColour = NdotL * colour;

  outputColour = vec4( diffuseColour, 1.0 );
}
