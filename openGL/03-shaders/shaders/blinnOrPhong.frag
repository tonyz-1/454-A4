// Switchable Blinn or Phong fragment shader
//
// Phong shading with diffuse and specular components

#version 300 es

uniform mediump vec3 lightDir;
uniform mediump vec3 halfDir;
uniform int useBlinn;

smooth in mediump vec3 colour;
smooth in mediump vec3 normal;

out mediump vec4 outputColour;

void main()

{
  mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
  mediump vec3 Ia  = vec3( 0.0, 0.0, 0.0 ); // ambient light
  mediump vec3 kd  = colour;		    // kd = surface colour from calling program
  mediump vec3 ks  = vec3( 0.4, 0.4, 0.4 ); // ks = specular coefficients
  mediump float shininess = 400.0;	    // shininess = specular exponent

  mediump vec3 N = normalize( normal );

  mediump float NdotL = dot( N, lightDir );

  // Compute the outgoing colour

  mediump vec3 Iout = Ia;

  if (NdotL > 0.0) {

    // diffuse component

    Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 

    // specular component

    if (useBlinn == 1) {

      mediump float NdotH = dot( N, halfDir );

      if (NdotH > 0.0)
        Iout += pow( NdotH, 4 * shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );

    } else {
      
      mediump vec3 R = (2.0 * NdotL) * N - lightDir;
      mediump vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS

      mediump float RdotV = dot( R, V );

      if (RdotV > 0.0)
	Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
    }
  }

  outputColour = vec4( Iout, 1.0f );
}


