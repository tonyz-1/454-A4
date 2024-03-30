// OpenGL demo 6
//
// Add another vertex attribute (colour) to the rotating square.
//
// Add a pause when 'p' is pressed


#include "headers.h"
#include "gpuProgram.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


GPUProgram *myGPUProgram;

GLuint squareVAO;

bool pauseAnim = false;


#define ROTATION_RATE (45.0/180.0*3.14159)


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

void keyCallback( GLFWwindow* window, int key, 
		  int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS)
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose( window, GL_TRUE );
      break;
    case 'P': // Note that GLFW always returns the uppercase letter, 
              // even if the lowercase letter was pressed.
      pauseAnim = !pauseAnim;
      break;
    }
}

  
// Set up a VAO for the square
//
// Here the square is centred at the origin and has sides of length 1.
//
// It's usually best to center your objects at the origin, aligned
// with the axes, and with a "unit" size.

void setupSquareVAO()

{
  glGenVertexArrays( 1, &squareVAO );
  glBindVertexArray( squareVAO );

  // VBO has 6 vertices with 2 entries each for position and colour.
  //
  // All the positions are defined first, followed by all the colours.
  //
  // Vertex layout:  3 ----- 2
  //                 |       |
  //                 |       |
  //                 0 ----- 1  

#if 0

  // Put all positions first, followed by all colours

  vec3 buffer[12];

  vec3 *verts   = &buffer[0];
  vec3 *colours = &buffer[6];

  verts[0] = vec3( -0.5, -0.5, 0 ); // v0
  verts[1] = vec3(  0.5, -0.5, 0 ); // v1
  verts[2] = vec3( -0.5,  0.5, 0 ); // v2

  verts[3] = vec3( -0.5,  0.5, 0 ); // v2
  verts[4] = vec3(  0.5, -0.5, 0 ); // v1
  verts[5] = vec3(  0.5,  0.5, 0 ); // v3

  colours[0] = vec3( .00, .00, .00 ); // v0 (black)
  colours[1] = vec3( .94, .89, .26 ); // v1 (yellowish)
  colours[2] = vec3( .00, .45, .70 ); // v2 (blueish)

  colours[3] = vec3( .00, .45, .70 ); // v2 (yellowish)
  colours[4] = vec3( .94, .89, .26 ); // v1 (blueish)
  colours[5] = vec3( 1.0, 1.0, 1.0 ); // v3 (white)

  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW );

  // Attribute 0 is position

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  // Attribute 1 is colour
  //
  // Note tha sizeof(float)*3*6 is the offset, in bytes, from the
  // start of the buffer to attribute 1 of the first vertex.

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 
			 0, 
			 (void*) (sizeof(float)*3*6) );

  glEnableVertexAttribArray( 1 );

#else

  // Put (position,colour) together for each vertex
  //
  // Could, alternatively, use a 'struct' to encapsulate all the
  // attributes of a vertex, then have an array of these structs.

  vec3 buffer[12];

  buffer[0]  = vec3( -0.5, -0.5, 0 ); // v0
  buffer[1]  = vec3( .00, .00, .00 ); // v0 (black)
  
  buffer[2]  = vec3(  0.5, -0.5, 0 ); // v1
  buffer[3]  = vec3( .94, .89, .26 ); // v1 (yellowish)
  
  buffer[4]  = vec3( -0.5,  0.5, 0 ); // v2
  buffer[5]  = vec3( .00, .45, .70 ); // v2 (blueish)
  
  buffer[6]  = vec3( -0.5,  0.5, 0 ); // v2
  buffer[7]  = vec3( .00, .45, .70 ); // v2 (yellowish)
  
  buffer[8]  = vec3(  0.5, -0.5, 0 ); // v1
  buffer[9]  = vec3( .94, .89, .26 ); // v1 (blueish)
  
  buffer[10] = vec3(  0.5,  0.5, 0 ); // v3
  buffer[11] = vec3( 1.0, 1.0, 1.0 ); // v3 (white)

  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW );

  // Attribute 0 is position
  //
  // Note that 2*3*sizeof(float) is the distance, in bytes, between
  // attribute 0 of the first vertex and attribute 0 of the second
  // vertex.  The special value of 0 for THIS parameter (not the last
  // parameter) means that the attributes are tightly packed, one
  // after another in the buffer.

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 
			 2*3*sizeof(float), 
			 0 );

  glEnableVertexAttribArray( 0 );

  // Attribute 1 is colour
  //
  // Note that the last parameter, 3*sizeof(float), is the offset, in
  // bytes, from the start of the buffer to attribute 1 of the first
  // vertex.

  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 
			 2*3*sizeof(float), 
			 (void*) (3*sizeof(float)) );
  
  glEnableVertexAttribArray( 1 );

#endif

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

  mat4 S = scale( state.len, state.len, 1 );  // scale by square size 
  mat4 R = rotate( state.theta, vec3(0,0,1) ); // rotate by theta radians about (0,0,1)
  mat4 T = translate( vec3(  -0.25, 0.5, 0 ) ); // translate from the origin to (-0.5,0.5,0)

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
