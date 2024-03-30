// Phong shading demo
//
// Draw a Phong-shaded object


#include "headers.h"
#include "linalg.h"
#include "wavefront.h"
#include "gpuProgram.h"
#include "axes.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


wfModel *obj;			// the object
Axes *axes;
GPUProgram *gpuProg;		// vertex and fragment shaders


// Drawing function

float theta = 0;
float eyeDistance = 30;
float fovy = 0.61;		// about 35 degrees

bool paused = false;
bool blinnOrPhong = false;
bool showAxes = false;
bool showDiffuse = true;
bool showSpecular = true;


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}



void display()

{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set up the GPU

  gpuProg->activate();

  // model-view transform (i.e. OCS-to-VCS)

  vec3 eyePosition( 0, 1, eyeDistance );

  mat4 MV = translate( -1 * eyePosition )
          * rotate( theta, vec3(0.5,2,0) );

  gpuProg->setMat4( "MV", MV );

  // model-view-projection transform (i.e. OCS-to-CCS)

  mat4 MVP = perspective( fovy, 1.333, eyeDistance-10, eyeDistance+10 )
           * MV;

  gpuProg->setMat4( "MVP", MVP );

  // Light direction in VCS is above, to the right, and behind the
  // eye.

  vec3 lightDir = vec3(2,0.5,0.6).normalize();

  gpuProg->setVec3( "lightDir", lightDir );

  // For Blinn-Phong shading:
  //
  // halfDir = vector halfway between lightDir and viewing direction.
  // For a light at infinity, lightDir is constant, so this can be
  // computed once and passed as a uniform variable to the shaders.
  //
  // This vector should be in the VCS.

  vec3 halfDir = (lightDir + vec3(0,0,1)).normalize();

  gpuProg->setVec3( "halfDir", halfDir );

  gpuProg->setInt( "useBlinn", (blinnOrPhong ? 1 : 0) );

  // Provide some flags to the shaders

  gpuProg->setInt( "showDiffuse",  (showDiffuse ? 1 : 0) );
  gpuProg->setInt( "showSpecular", (showSpecular ? 1 : 0) );

  // Draw the objects

  obj->draw( gpuProg );

  // Done

  gpuProg->deactivate();

  // Axes

  if (showAxes) {
    MVP = MVP * scale( 8, 8, 8 );
    axes->draw( MVP );
  }

  glDisable( GL_BLEND );
}



// Update the object angle


void updateState( float elapsedSeconds )

{
  // Set angle based on elapsed time

  if (!paused)
    theta += elapsedSeconds * 0.3;
}



// Handle a key press


void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {

    case GLFW_KEY_ESCAPE:
      exit(0);

    case 'P':
      paused = !paused;
      break;

    case 'B':
      blinnOrPhong = !blinnOrPhong;
      break;

    case 'A':
      showAxes = !showAxes;
      break;

    case 'D':
      showDiffuse = !showDiffuse;
      break;

    case 'S':
      showSpecular = !showSpecular;
      break;

    case ',':
      if (paused)
	theta -= 0.2*M_PI/180.0;
      break;

    case '.':
      if (paused)
	theta += 0.2*M_PI/180.0;
      break;
      
    case GLFW_KEY_UP:
      eyeDistance /= 1.1;
      cout << "eye distance = " << eyeDistance << endl;
      break;
      
    case GLFW_KEY_DOWN:
      eyeDistance *= 1.1;
      cout << "eye distance = " << eyeDistance << endl;
      break;
      
    case GLFW_KEY_LEFT:
      fovy *= 1.1;
      cout << "fovy = " << fovy << endl;
      break;
      
    case GLFW_KEY_RIGHT:
      fovy /= 1.1;
      cout << "fovy = " << fovy << endl;
      break;

    case '/':
      cout << "p     - pause" << endl
	   << "b     - toggle Blinn/Phong" << endl
	   << "a     - show axes" << endl
	   << "d     - toggle diffuse" << endl
	   << "s     - toggle specular" << endl
	   << "<     - rotate left" << endl
	   << ">     - rotate right" << endl
	   << "up    - move closer" << endl
	   << "down  - move farther away" << endl
	   << "left  - zoom out" << endl
	   << "right - zoom in" << endl;
    }
  }
}



// Main program


int main( int argc, char **argv )

{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " shader_name" << endl
	 << "Valid shader names are: blinn blinnOrPhong depth diffuse flat gouraud none normals phong silhouette toon" << endl;
    exit(1);
  }

  // Set up GLFW

  GLFWwindow* window;

  if (!glfwInit()) {
    cerr << "GLFW failed to initialize" << endl;
    return 1;
  }
  
  glfwSetErrorCallback( errorCallback );
  
  // Open window (ask for OpenGL ES 3.0 or better)

#ifdef MACOS
  // glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#else
  glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#endif

  // Set up the window

  window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Shader demo", NULL, NULL);
  
  if (!window) {
    glfwTerminate();

#ifdef MACOS
    const char *descrip;
    int code = glfwGetError( &descrip );
    cerr << "GLFW code:  " << code << endl;
    cerr << "GLFW error: " << descrip << endl;
#endif
    
    return 1;
  }

  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 );
  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  glfwSetKeyCallback( window, keyCallback );

  // CWD is directory above

  chdir( ".." );

  // Set up world objects

  obj = new wfModel( "data/triceratops.obj", LINEAR );

  axes = new Axes();

  // Set up GPU

  char vsName[1000], fsName[1000];
  sprintf( vsName, "shaders/%s.vert", argv[1] );
  sprintf( fsName, "shaders/%s.frag", argv[1] );

  gpuProg = new GPUProgram( vsName, fsName );

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );

  glEnable( GL_DEPTH_TEST );

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
