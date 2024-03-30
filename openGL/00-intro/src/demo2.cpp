// OpenGL demo 2
//
// Draw a triangle.
//
// Handle the ESC key.
//
// For the moment, ignore the reference to the "GPU program".  Later
// demos will look into that.


#include "headers.h"
#include "gpuProgram.h"
#include "linalg.h"


GPUProgram *myGPUProgram;	// pointer to GPU program object

GLuint triangleVBO;		// the index of the VBO that contains the triangle object



// Error callback
//
// This is good to have, as it will trap internal GLFW errors.


void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}



// Key callback
//
// Exit when the ESC key is pressed


void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose( window, GL_TRUE ); // the window is closed *later*, in the main loop
}



// Define triangle
//
// This sends three vertices to the GPU to store in a VBO on the GPU.


void createTriangle()

{
  // Define three vertices in 3D
  //
  // Default screen is [-1,+1] x [-1,+1]

  vec3 verts[3];

  verts[0] = vec3( 0.0, 0.0, 0.0 ); 
  verts[1] = vec3( 0.5, 0.0, 0.0 );
  verts[2] = vec3( 0.0, 0.5, 0.0 );

  // Create a Vertex Buffer Object (VBO) on the GPU and get its index

  glGenBuffers( 1, &triangleVBO );

  // Tell OpenGL to use this VBO as the *current* Array Buffer

  glBindBuffer( GL_ARRAY_BUFFER, triangleVBO );

  // Fill the *current* array buffer with the three vertices
  //
  // GL_STATIC_DRAW = This data won't be changed later.
  
  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

  // Unbind the array buffer so that other parts of the code cannot
  // mistaken operate on it.  A value of 0 below causes unbinding.

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}


  
// Drawing function


void display()

{
  // Use the triangle that was previously stored in the triangle VBO.

  glBindBuffer( GL_ARRAY_BUFFER, triangleVBO );

  // Tell the GPU about the "attribute 0" data in the *currently bound* VBO (i.e. the triangle VBO)
  //
  //  0 = attribute 0
  //  3 = number of component in attribute (3 for x,y,z)
  //  GL_FLOAT = type of a component
  //  GL_FALSE = don't normalize the attributes
  //  0 = stride between attributes (0 = tightly packed in the VBO)
  //  0 = offset to the first attribute within the VBO
  //
  // Note that we could define more attributes (1,2,3, ...) for things
  // like colour, texture coordinate, normal, etc.

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // The GPU reads a number of attributes from the VBO.  The following
  // allows the shader on the GPU to have access to attribute 0.

  glEnableVertexAttribArray( 0 );

  // Do the actual drawing
  //
  //  GL_TRIANGLES = use every three attributes as one triangle
  //  0 = offset to first vertex
  //  3 = total number of vertices

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawArrays( GL_TRIANGLES, 0, 3 );

  // Don't leave attributes or the VBO enabled

  glDisableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}



// Main program


int main( int argc, char **argv )

{
  // Trap all errors (do this *before* creating the window)

  glfwSetErrorCallback( errorCallback );
  
  // Init everything

  GLFWwindow* window;

  if (!glfwInit())
    return 1;
  
  // Open window

#ifdef MACOS
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#else
  glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#endif

  window = glfwCreateWindow( 800, 600, "Demo 2", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 );
  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Define a key handler (do this *after* creating the window)

  glfwSetKeyCallback( window, keyCallback );

  // Set up vertex and fragment shaders

  chdir( "../src" );

#ifdef MACOS
  myGPUProgram = new GPUProgram( "demo2-macos.vert", "demo2-macos.frag" );
#else
  myGPUProgram = new GPUProgram( "demo2.vert", "demo2.frag" );
#endif

#ifdef MACOS
  // Ignore this.  It's an idiosyncracy of MacOS.  More about VAOs in demo4.
  GLuint VAO;
  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );
#endif

  myGPUProgram->activate();

  // Create the triangle in a VBO on the GPU

  createTriangle();

  // Main loop

  while (!glfwWindowShouldClose( window )) {
    glClearColor( 1.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    display();
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}


