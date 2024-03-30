// Toon fragment shader
//
// Quantize the colours

#version 300 es

uniform mediump vec3 lightDir;		// in VCS

smooth in mediump vec3 colour;
smooth in mediump vec3 normal;		// in VCS

out mediump vec4 outputColour;

void main()

{
  mediump float NdotL = dot( normalize(normal), lightDir );

  if (NdotL > 0.95)
    outputColour = 0.95 * vec4(colour,1.0); // 0.95 - 1.00  ->  0.95
  else if (NdotL > 0.70)
    outputColour = 0.70 * vec4(colour,1.0); // 0.70 - 0.95  ->  0.70
  else if (NdotL > 0.40)
    outputColour = 0.40 * vec4(colour,1.0); // 0.40 - 0.70  ->  0.40
  else
    outputColour = 0.10 * vec4(colour,1.0); // 0.00 - 0.40  ->  0.10

  outputColour.w = 1.0;
}	
