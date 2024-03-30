Here are the shaders and what to look at in each:

1. diffuse

   See how (N dot L) is applied.

2. phong, blinn, blinnOrPhong

   Compare Phong and Blinn calculations.

3. flat, gouraud, phong

   Compare flat shading, Gouraud shading, and per-pixel Phong shading.

3. depth

   Show pixel depths by computing the NDCS depth of each vertex, then
   interpolating these depths across the primitive to provide fragment
   depths.

4. normals

   Show normals in both the OCS and VCS.

5. silhouettes

   Find pixels on the silhouette.  There pixels have a normal
   approximately perpendicular to the view direction toward the pixel.
   So the fragment shader needs to know the VCS coordinates of each
   fragment, which are computed in the vertex shader and interpolated
   into the fragment shader.

6. toon

   Quantize intensities to simulate cartoon shading.
