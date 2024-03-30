// Gouraud shading.  Phong shading is done at each vertex to get a
// colour for the vertex.  Then vertex colours are interpolated across
// the triangle.  This can miss highlights on the interior of the
// triangle.  But the fragment shader doesn't have to do anything, so
// it's fast.

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS
uniform vec3 lightDir;          // direction toward the light (in the VCS)
uniform int showDiffuse;
uniform int showSpecular;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

smooth out mediump vec3 colour;	// smooth = linear interpolation of colour

void main()

{
  // calc vertex position in CCS

  gl_Position = MVP * vec4( vertPosition, 1.0f );

  // assign (r,g,b) colour

  const mediump vec3 C = 2.0 * vec3( 0.33, 0.42, 0.18 );

  // calc normal in VCS

  mediump vec3 normal = vec3( MV * vec4( vertNormal, 0.0 ) );

  // Do the Phong (specular & diffuse) calculation at this vertex
  //
  // Following is the code from phong.frag (the per-fragment Phong
  // shader).

  mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
  mediump vec3 Ia  = vec3( 0.1, 0.1, 0.1 ); // ambient light
  mediump vec3 kd  = C;	                    // kd = surface colour from calling program
  mediump vec3 ks  = vec3( 0.4, 0.4, 0.4 ); // ks = specular coefficients
  mediump float shininess = 10.0;	    // shininess = specular exponent
  
  mediump vec3 N = normalize( normal );

  mediump float NdotL = dot( N, lightDir );

  // Compute the outgoing colour
  //
  // Iout = Ia + Ie + kd (N.L) Iin + ks (R.V)^n Iin

  mediump vec3 Iout = Ia;

  if (NdotL > 0.0) {

    // diffuse component

    if (showDiffuse == 1)
      Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 

    // specular component

    if (showSpecular == 1) {
      
      mediump vec3 R = normalize( (2.0 * NdotL) * N - lightDir );
      mediump vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS
      
      mediump float RdotV = dot( R, V );

      if (RdotV > 0.0)
	Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
    }
  }

  colour = Iout;
}
