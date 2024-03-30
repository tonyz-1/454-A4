// flat shading
//
// Phong shading is done at the vertices to get the vertex colour.
// Then the colour of one vertex is used for the whole triangle

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

  const vec3 C = 2.0 * vec3( 0.33, 0.42, 0.18 );

  // calc normal in VCS

  vec3 normal = vec3( MV * vec4( vertNormal, 0.0 ) );

  // Do the Phong (specular & diffuse) calculation at this vertex
  //
  // Following is the code from phong.frag (the per-fragment Phong
  // shader).

  vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
  vec3 Ia  = vec3( 0.0, 0.0, 0.0 ); // ambient light
  vec3 kd  = C;	  	            // kd = surface colour from calling program
  vec3 ks  = vec3( 0.8, 0.8, 0.8 ); // ks = specular coefficients
  float shininess = 400.0;	    // shininess = specular exponent

  vec3 N = normalize( normal );

  float NdotL = dot( N, lightDir );

  // Compute the outgoing colour

  vec3 Iout = Ia;

  if (NdotL > 0.0) {

    // diffuse component

    Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 

    // specular component

    vec3 R = normalize( (2.0 * NdotL) * N - lightDir );
    vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS

    float RdotV = dot( R, V );

    if (RdotV > 0.0)
      Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
  }

  colour = Iout;
}
