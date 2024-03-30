// OpenGL demo 8
//
// 3D tetrahedron


#include "headers.h"
#include "gpuProgram.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


GPUProgram *myGPUProgram;

GLuint squareVAO;

bool pauseAnim = false;


#define ROTATION_RATE (45.0/180.0*3.14159) // 45 degrees per second, but represented in radians per second


// The world state

struct tag_worldState {

  float len;    // side length
  float theta;  // angle in radians
  
} state = { 0.75, 0.0 };


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// Key callback

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS)
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose( window, GL_TRUE );
      break;
    case 'P': // Note that GLFW always returns the uppercase letter, even if the lowercase letter was pressed.
      pauseAnim = !pauseAnim;
      break;
    }
}

  
// Set up a VAO for a tetrahedron

void setupTetrahedronVAO()

{
  glGenVertexArrays( 1, &squareVAO );
  glBindVertexArray( squareVAO );

  // 3 vertices for each of 4 triangles.  Put all positions first, followed by all colours.

  vec3 v0( 1, 0, 0 );
  vec3 v1( cos(0.667*M_PI), sin(0.667*M_PI), 0 );
  vec3 v2( cos(1.333*M_PI), sin(1.333*M_PI), 0 );
  vec3 v3( 0, 0, 1 );

  vec3 c0( .00, .45, .70 ); // yellowish
  vec3 c1( .94, .89, .26 ); // blueish
  vec3 c2( .33, .80, .40 ); // greenish
  vec3 c3( .70, .70, .70 ); // light grey

  vec3 buffer[] = {
    v0, v1, v3,
    v1, v2, v3,
    v2, v0, v3,
    v0, v2, v1,

    c0, c0, c0,
    c1, c1, c1,
    c2, c2, c2,
    c3, c3, c3
  };


  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW );

  // Attribute 0 is position

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  // Attribute 1 is colour

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*3*12) );
  glEnableVertexAttribArray( 1 );

  // Unbind everything

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
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
  mat4 R = rotate( state.theta, vec3(1,1,1) ); // rotate by theta radians about (0,0,1)
  mat4 T = translate( vec3(  0, 0, -10 ) ); // translate from the origin to (-0.5,0.5,0)

  // Viewing matrix: identity
  
  mat4 V = identity4();

  // Projection matrix (30 degree FOV, 1.0 aspect)

  mat4 P = perspective( 30.0/180.0*M_PI, 1.0, 1.0, 1000.0 );

  // Since the object was initially centred at the origin, we don't
  // have to move it to the origin before rotating.
  
  mat4 MVP = P * V * T * R * S;

  // Activate the shaders
  //
  // This is really only necessary if multiple shaders are used at
  // different times in the rendering, but is provided here as an
  // example.

  myGPUProgram->activate();

  // Provide M to the vertex shader

  glUniformMatrix4fv( glGetUniformLocation( myGPUProgram->id(), "M"), 1, GL_TRUE, &MVP[0][0] );

  // Draw

  glBindVertexArray( squareVAO );
  glDrawArrays( GL_TRIANGLES, 0, 12 );
  glBindVertexArray( 0 );

  // Deactivate the shaders

  myGPUProgram->deactivate();
}



// Update the world state

void updateState( float elapsedSeconds )

{
  if (pauseAnim)
    return;

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

  window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Demo 6", NULL, NULL);
  
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
  myGPUProgram = new GPUProgram( "demo6-macos.vert", "demo6-macos.frag" );
#else
  myGPUProgram = new GPUProgram( "demo6.vert", "demo6.frag" );
#endif

  // Create the VAO for the generic tetrahedron

  setupTetrahedronVAO();

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

#if 0
    glClear( GL_COLOR_BUFFER_BIT );
#else
    glEnable( GL_DEPTH_TEST );				 // enable depth test
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear depth buffer
#endif

    display();
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}
