// phong shader
//
// All Phong calculations are done here

#version 300 es

uniform mediump vec3 lightDir;	// already in the VCS
uniform int showDiffuse;
uniform int showSpecular;

smooth in mediump vec3 colour;
smooth in mediump vec3 normal_vcs;
smooth in mediump vec3 position_vcs;

out mediump vec4 outputColour;

void main()

{
  // The following constants would normally be provided as uniforms by the C++ program

  mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light
  mediump vec3 Ia  = vec3( 0.1, 0.1, 0.1 ); // ambient light
  mediump vec3 kd  = colour;		    // kd = surface colour from calling program
  mediump vec3 ks  = vec3( 0.4, 0.4, 0.4 ); // ks = specular coefficients
  mediump float shininess = 10.0;	    // shininess = specular exponent

  // Since the normal is interpolated, it might not be unit length.  So normalize it.

  mediump vec3 N = normalize( normal_vcs );
  mediump vec3 V = normalize( -1.0 * position_vcs );       // Looking toward the eye at (0,0,0) in the VCS 

  mediump float NdotL = dot( N, lightDir );

  // Compute the outgoing colour

  mediump vec3 Iout = Ia;

  if (NdotL > 0.0) {  // light is above the surface

    // diffuse component

    if (showDiffuse == 1)
      Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 

    // specular component

    if (showSpecular == 1) {
      
      mediump vec3 R = (2.0 * NdotL) * N - lightDir;

      mediump float RdotV = dot( R, V );

      if (RdotV > 0.0)
	Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
    }
  }

  outputColour = vec4( Iout, 1.0f );
}


