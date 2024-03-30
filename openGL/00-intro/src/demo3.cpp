// OpenGL demo 3
//
// An animation with a rotating square.
//
// Three variants are included.  Change the '#if' lines in display()
// to see the variants.


#include "headers.h"
#include "gpuProgram.h"


GPUProgram *myGPUProgram;


#define ROTATION_RATE (45.0/180.0*3.14159) // 45 degrees per second, but represented in radians per second


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

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit on ESC
    glfwSetWindowShouldClose( window, GL_TRUE );
}

  
// Drawing function

void display()

{
  // Define two triangles to make a square

  vec3 verts[6];

#if 0

  // Ignoring angle: a static square

  verts[0] = vec3( 0.0, 0.0, 0.0 ); // first triangle
  verts[1] = vec3( state.len, 0.0, 0.0 );
  verts[2] = vec3( 0.0, state.len, 0.0 );

  verts[3] = vec3( 0.0, state.len, 0.0 ); // second triangle
  verts[4] = vec3( state.len, 0.0, 0.0 );
  verts[5] = vec3( state.len, state.len, 0.0 ); 

#else

  #if 0

  // Taking theta into account
  //
  //  (x90,y90) ----- (x45,y45)
  //      |               |
  //      |               |
  //      |               |
  //      |               |
  //      |               |
  //    (0,0) -------- (x0,y0)

  float x0 = state.len * cos(state.theta);
  float y0 = state.len * sin(state.theta);

  float x45 = sqrt(2)*state.len * cos(state.theta+M_PI/4.0);
  float y45 = sqrt(2)*state.len * sin(state.theta+M_PI/4.0);

  float x90 = state.len * cos(state.theta+M_PI/2.0);
  float y90 = state.len * sin(state.theta+M_PI/2.0);

  verts[0] = vec3( 0.0, 0.0, 0.0 ); // first triangle
  verts[1] = vec3(  x0,  y0, 0.0 );
  verts[2] = vec3( x90, y90, 0.0 );

  verts[3] = vec3( x90, y90, 0.0 ); // second triangle
  verts[4] = vec3(  x0,  y0, 0.0 );
  verts[5] = vec3( x45, y45, 0.0 );

  #else

  // Using the linalg code to transform vertices
  //
  // Vertex layout:  3 ----- 2
  //                 |       |
  //                 |       |
  //                 0 ----- 1  

  vec4 v0(   0,   0, 0, 1 );
  vec4 v1( state.len,   0, 0, 1 );
  vec4 v2( state.len, state.len, 0, 1 );
  vec4 v3(   0, state.len, 0, 1 );

  mat4 R     = rotate( state.theta, vec3(0,0,1) );      // rotate by theta radians about (0,0,1)
  mat4 Tto   = translate( vec3( -state.len/2, -state.len/2, 0 ) );	// translate to the origin
  mat4 Tfrom = translate( vec3(  state.len/2,  state.len/2, 0 ) );	// translate from the origin

  // Is Tto = - Tfrom ?

  mat4 m = Tfrom * R * Tto;	// used on v as: (Tfrom * R * Tto) * v

  verts[0] = ( m * v0 ).toVec3();
  verts[1] = ( m * v1 ).toVec3();
  verts[2] = ( m * v3 ).toVec3();

  verts[3] = ( m * v3 ).toVec3();
  verts[4] = ( m * v1 ).toVec3();
  verts[5] = ( m * v2 ).toVec3();

  #endif

#endif

  // Set up VBO
  //
  // Note that we're not creating a VBO ahead of time, but are
  // creating a new, temporary VBO every time display() is called.
  // This is inefficient.

  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

  // Declare vertex attributes

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  glEnableVertexAttribArray( 0 );

  // Draw

  glDrawArrays( GL_TRIANGLES, 0, 6 ); // 6 vertices now (previously was 3 for a single triangle)

  // Disable vertex attributes

  glDisableVertexAttribArray( 0 );

  // Destroy this temporary VBO

  glDeleteBuffers( 1, &VBO );
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

  window = glfwCreateWindow( 800, 600, "Demo 3", NULL, NULL);
  
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

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    ftime( &thisTime );
    float elapsedSeconds = (thisTime.time + thisTime.millitm / 1000.0) - (prevTime.time + prevTime.millitm / 1000.0);
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


