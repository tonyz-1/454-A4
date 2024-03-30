// OpenGL demo 5
//
// A square rotated in the vertex shader.


#include "headers.h"
#include "gpuProgram.h"


GPUProgram *myGPUProgram;

GLuint squareVAO;


#define ROTATION_RATE (45.0/180.0*3.14159)


// The world state

struct tag_worldState {

  float len;    // side length
  float theta;  // angle in radians
  
} state = { 0.5, 0.0 };


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// Key callback

void keyCallback( GLFWwindow* window, int key, 
		  int scancode, int action, int mods )
  
{
  // exit on ESC

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose( window, GL_TRUE );
}

  
// Set up a VAO for the square
//
// Same as demo4.cpp.


void setupSquareVAO()

{
  // VBO ID
  
  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );

  // VAO

  glGenVertexArrays( 1, &squareVAO );
  glBindVertexArray( squareVAO );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );
  glBindVertexArray( 0 );

  // This VBO stores a *generic* square which is later transformed.

  vec3 verts[6];

  verts[0] = vec3( -0.5, -0.5, 0 ); // first triangle
  verts[1] = vec3(  0.5, -0.5, 0 );
  verts[2] = vec3( -0.5,  0.5, 0 );

  verts[3] = vec3( -0.5,  0.5, 0 ); // second triangle
  verts[4] = vec3(  0.5, -0.5, 0 );
  verts[5] = vec3(  0.5,  0.5, 0 ); 

  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}



// Drawing function
//
// Find a transformation to apply to the static square that is defined
// in the squareVAO.  Pass that transformation to the vertex shader in
// a uniform variable.


void display()

{
  // Find the transform.  This is the same as in demo3, but with
  // additional scaling by state.len since the square is 1x1.

  mat4 S = scale( state.len, state.len, 1 );   // scale by square size 
  mat4 R = rotate( state.theta, vec3(0,0,1) ); // rotate by theta radians about (0,0,1)
  mat4 T = translate( vec3(  -0.5, 0.5, 0 ) ); // translate from the origin to (-0.5,0.5,0)

  // Since the object was initially centred at the origin, we don't
  // have to move it to the origin before rotating.
  
  mat4 M = T * R * S;

  // Activate the shaders
  //
  // This is really only necessary if multiple shaders are used at
  // different times in the rendering, but is provided here as an
  // example.

  myGPUProgram->activate();

  // Provide M to the vertex shader

  glUniformMatrix4fv( glGetUniformLocation( myGPUProgram->id(), "M"), 1, GL_TRUE, &M[0][0] );

  // Draw

  glBindVertexArray( squareVAO );
  glDrawArrays( GL_TRIANGLES, 0, 6 );
  glBindVertexArray( 0 );

  // Deactivate the shaders

  myGPUProgram->deactivate();
}



// Update the world state

void updateState( float elapsedSeconds )

{
  // increment angle

  state.theta += ROTATION_RATE * elapsedSeconds;

  // report angle (in degrees, on a single line)

  cout << '\r' << state.theta/3.14159*180 << "   ";
  cout.flush();
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

  window = glfwCreateWindow( 800, 600, "Demo 5", NULL, NULL);
  
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
  //
  // These shaders use a transformation matrix provided from the C++ code. 

  chdir( "../src" );

#ifdef MACOS
  myGPUProgram = new GPUProgram( "demo5-macos.vert", "demo5-macos.frag" );
#else
  myGPUProgram = new GPUProgram( "demo5.vert", "demo5.frag" );
#endif

  // Create the VAO for the generic square

  setupSquareVAO();

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    ftime( &thisTime );
    float elapsedSeconds =   (thisTime.time + thisTime.millitm / 1000.0)
                           - (prevTime.time + prevTime.millitm / 1000.0);
    prevTime = thisTime;

    // Update the world state

    updateState( elapsedSeconds );

    // Clear, display, and check for events

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    display();
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}
