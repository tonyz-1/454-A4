// OpenGL demo 7
//
// Drag an object with the mouse.


#include "headers.h"
#include "gpuProgram.h"

int windowWidth  = 700;
int windowHeight = 700;


GPUProgram *myGPUProgram;

GLuint squareVAO;

bool pauseAnim = false;


// The world state

struct tag_worldState {

  float len;           // side length
  float theta;         // angle in radians
  float x, y;	       // position in world [-1,1]x[-1,1] coordinates
  float initX, initY;  // position in world upon most recent mouse click
  
} state = { 0.20, 0.0, -0.5, 0.5 };


// The mouse state

struct tag_mouseState {

  float initX, initY;   //  mouse position upon first click

} mouseState;


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
      
    case GLFW_KEY_ESCAPE:	// ESC closes window
      glfwSetWindowShouldClose( window, GL_TRUE );
      break;
      
    case 'P':			// P pauses/unpauses
      pauseAnim = !pauseAnim;
      break;
    }
}



// Mouse motion callback
//
// Only enabled when mouse button is down (which is done in mouseButtonCallback())

void mousePositionCallback( GLFWwindow* window, double xpos, double ypos )
  
{
  // Find amount mouse has travelled, measured in pixels

  float mouseDiffX = xpos - mouseState.initX; 
  float mouseDiffY = ypos - mouseState.initY;

  // Convert to world coordinates in [-1,1]x[-1,1]
  //
  // Note that these are *differences*, not absolute values, so
  // constant terms in the absolute values cancel out.

  float worldDiffX = (mouseDiffX / (float) windowWidth)  * 2;
  float worldDiffY = (mouseDiffY / (float) windowHeight) * 2;

  // Translate object from its *initial* position by the worldDiff

  state.x = state.initX + worldDiffX;
  state.y = state.initY - worldDiffY; // subtract because mouse y coords are flipped
}



// Mouse button callback

void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )

{
  if (button == GLFW_MOUSE_BUTTON_LEFT)

    if (action == GLFW_PRESS) {

      // get and store initial mouse position
      
      double x, y;
      glfwGetCursorPos(window, &x, &y );
      mouseState.initX = x;
      mouseState.initY = y;

      // store position of square upon this click

      state.initX = state.x;
      state.initY = state.y;

      // enable mouse movement events
      
      glfwSetCursorPosCallback( window, mousePositionCallback );
      
    } else // (action == GLFW_RELEASE)

      // disable mouse movement events
      
      glfwSetCursorPosCallback( window, NULL );
}
 
  
// Set up a VAO for the square

void setupSquareVAO()

{
  glGenVertexArrays( 1, &squareVAO );
  glBindVertexArray( squareVAO );

  // VBO has 6 vertices with 2 entries each for position and colour.

  vec3 buffer[12];

  vec3 *verts   = &buffer[0];
  vec3 *colours = &buffer[6];

  verts[0] = vec3( -0.5, -0.5, 0 ); // v0
  verts[1] = vec3(  0.5, -0.5, 0 ); // v1
  verts[2] = vec3( -0.5,  0.5, 0 ); // v2

  verts[3] = vec3( -0.5,  0.5, 0 ); // v2
  verts[4] = vec3(  0.5, -0.5, 0 ); // v1
  verts[5] = vec3(  0.5,  0.5, 0 ); // v3

  colours[0] = vec3( .88, .81, .47 ); // v0-v1-v2 is light yellow/tan
  colours[1] = vec3( .88, .81, .47 );
  colours[2] = vec3( .88, .81, .47 );

  colours[3] = vec3( .69, .44, .24 ); // v1-v2-v3 is darker tan/orange
  colours[4] = vec3( .69, .44, .24 );
  colours[5] = vec3( .69, .44, .24 );

  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW );

  // Attribute 0 is position

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  // Attribute 1 is colour

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*3*6) );
  glEnableVertexAttribArray( 1 );

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
  mat4 R = rotate( state.theta, vec3(0,0,1) ); // rotate by theta radians about (0,0,1)
  mat4 T = translate( vec3( state.x, state.y, 0 ) ); // translate from the origin to current (x,y)

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

void updateState( float elapsedSeconds, float currentTime )

{
  if (pauseAnim)
    return;

  // vary angle sinusoidally

  state.theta = 0.5 * sin( (float) currentTime );
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

  window = glfwCreateWindow( windowWidth, windowHeight, "Demo 6", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 );
  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Callbacks

  glfwSetKeyCallback( window, keyCallback ); // key
  glfwSetMouseButtonCallback( window, mouseButtonCallback ); // mouse button 

  // Vertex and fragment shaders

  chdir( "../src" );

#ifdef MACOS
  myGPUProgram = new GPUProgram( "demo6-macos.vert", "demo6-macos.frag" );
#else
  myGPUProgram = new GPUProgram( "demo6.vert", "demo6.frag" );
#endif

  // VAO for a generic square

  setupSquareVAO();

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );
  float startTime = prevTime.time + prevTime.millitm / 1000.0;

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    ftime( &thisTime );
    float currentTime = (thisTime.time + thisTime.millitm / 1000.0) - startTime;
    float elapsedSeconds = (thisTime.time + thisTime.millitm / 1000.0) - (prevTime.time + prevTime.millitm / 1000.0);
    prevTime = thisTime;

    // Update the world state

    updateState( elapsedSeconds, currentTime );

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
