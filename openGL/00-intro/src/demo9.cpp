// OpenGL demo 9
//
// Draw a cube in perspective


#include "headers.h"
#include "gpuProgram.h"
#include "linalg.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


GPUProgram *myGPUProgram;

GLuint cubeVAO;

bool pauseAnim = false;


#define ROTATION_RATE (45.0/180.0*3.14159) // 45 degrees per second, but represented in radians per second


// The world state

struct tag_worldState {

  float len;    // side length
  float theta;  // angle in radians
  
} state = { 1.0, 0.0 };


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

  
void setupCubeVAO()

{
  // a cube made of triangles

  GLfloat cube_positions[3*36] = { 
    -1,-1,-1,
    -1,-1, 1,
    -1, 1, 1,

     1, 1,-1,
    -1,-1,-1,
    -1, 1,-1,
    
     1,-1, 1,
    -1,-1,-1,
     1,-1,-1,
    
     1, 1,-1,
     1,-1,-1,
    -1,-1,-1,
    
    -1,-1,-1,
    -1, 1, 1,
    -1, 1,-1,
     1,-1, 1,
    -1,-1, 1,
    -1,-1,-1,
    -1, 1, 1,
    -1,-1, 1,
     1,-1, 1,
     1, 1, 1,
     1,-1,-1,
     1, 1,-1,
     1,-1,-1,
     1, 1, 1,
     1,-1, 1,
     1, 1, 1,
     1, 1,-1,
    -1, 1,-1,
     1, 1, 1,
    -1, 1,-1,
    -1, 1, 1,
     1, 1, 1,
    -1, 1, 1,
     1,-1, 1
  };


  // Random colour for each cube vertex

  GLfloat cube_colours[3*36] = { 

    0.583,  0.771,  0.014,
    0.609,  0.115,  0.436,
    0.327,  0.483,  0.844,
    
    0.822,  0.569,  0.201,
    0.435,  0.602,  0.223,
    0.310,  0.747,  0.185,
    
    0.597,  0.770,  0.761,
    0.559,  0.436,  0.730,
    0.359,  0.583,  0.152,
    
    0.483,  0.596,  0.789,
    0.559,  0.861,  0.639,
    0.195,  0.548,  0.859,
    0.014,  0.184,  0.576,
    0.771,  0.328,  0.970,
    0.406,  0.615,  0.116,
    0.676,  0.977,  0.133,
    0.971,  0.572,  0.833,
    0.140,  0.616,  0.489,
    0.997,  0.513,  0.064,
    0.945,  0.719,  0.592,
    0.543,  0.021,  0.978,
    0.279,  0.317,  0.505,
    0.167,  0.620,  0.077,
    0.347,  0.857,  0.137,
    0.055,  0.953,  0.042,
    0.714,  0.505,  0.345,
    0.783,  0.290,  0.734,
    0.722,  0.645,  0.174,
    0.302,  0.455,  0.848,
    0.225,  0.587,  0.040,
    0.517,  0.713,  0.338,
    0.053,  0.959,  0.120,
    0.393,  0.621,  0.362,
    0.673,  0.211,  0.457,
    0.820,  0.883,  0.371,
    0.982,  0.099,  0.879
  };


  // Create a VAO for the cube

  glGenVertexArrays( 1, &cubeVAO );
  glBindVertexArray( cubeVAO );

  // store positions

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions), cube_positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store colours

  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
}


// Drawing function


void display()

{
  // model-view-projection transform

  const float distance = 8;
  
#if 1
  mat4 MVP
    = perspective( 30.0*M_PI/180.0, WINDOW_WIDTH/(float)WINDOW_HEIGHT, distance-2, distance+2 )
    * translate( 0, 0, -distance )
    * scale( state.len, state.len, state.len )
    * rotate( state.theta, vec3( 0, 1, 1 ) );
#else
  mat4 MVP
    = ortho( -2, 2,
	     -2*(WINDOW_HEIGHT/(float)WINDOW_WIDTH), 2*(WINDOW_HEIGHT/(float)WINDOW_WIDTH),
	     distance-2, distance+2 )
    
    * translate( 0, 0, -distance )
    * scale( state.len, state.len, state.len )
    * rotate( state.theta, vec3( 0, 1, 1 ) );
#endif

  myGPUProgram->activate();

  glUniformMatrix4fv( glGetUniformLocation( myGPUProgram->id(), "MVP"), 1, GL_TRUE, &MVP[0][0] );

  glBindVertexArray( cubeVAO );
  glDrawArrays( GL_TRIANGLES, 0, 36 );
  glBindVertexArray( 0 );

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

  window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Demo 9", NULL, NULL);
  
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
  myGPUProgram = new GPUProgram( "demo9-macos.vert", "demo9-macos.frag" );
#else
  myGPUProgram = new GPUProgram( "demo9.vert", "demo9.frag" );
#endif
  
  // Create the VAO for the generic cube

  setupCubeVAO();
  glEnable( GL_DEPTH_TEST );

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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear depth buffer

    display();

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}



