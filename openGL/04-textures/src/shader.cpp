 // Phong shading demo
//
// Draw a Phong-shaded object


#include "headers.h"
#include "linalg.h"
#include "wavefront.h"
#include "gpuProgram.h"
#include "axes.h"
#include "font.h"
#include "glerror.h"
#include "glverts.h"


#define SHADER_DIR     "../shaders/"
#define DEFAULT_SHADER "wavefront"

#define FREETYPE_LIB   "../data/FreeSans.ttf"   // only used if HAVE_FONTS is defined


GLFWwindow* window;
wfModel *obj;			// the object
Axes *axes;
GPUProgram *gpuProg;		// vertex and fragment shaders

int winWidth = 600;
int winHeight = 600;

// Drawing function

float theta = 0.75*M_PI;
float eyeDistance = 0;
float fovy = 0.1;

bool paused = false;
bool blinnOrPhong = false;
bool showAxes = false;
bool setX = true;

int mode = 0;			// for different modes in the fragment shader

char *modeString[] = {
  "plain texture",
  "texture coordinates",
  "no texturing",
  "'replace' mode",
  "'decal' mode",
  "'modulate' mode",
  "mipmap levels",
  ""
};

#define MIPMAP_MODE 6


TextureMode textureMode = NEAREST;
char *texModeString[] = { "nearest", "bilinear", "mipmap nearest", "mipmap linear (= trilinear)", "" };



#define NUM_MIPMAP_COLOURS 6

vec3 mipmapColours[NUM_MIPMAP_COLOURS] = {
  vec3( 129.0/255.0,  15.0/255.0, 124.0/255.0 ),
  vec3( 136.0/255.0,  86.0/255.0, 167.0/255.0 ),
  vec3( 140.0/255.0, 150.0/255.0, 198.0/255.0 ),
  vec3( 158.0/255.0, 188.0/255.0, 218.0/255.0 ),
  vec3( 191.0/255.0, 211.0/255.0, 230.0/255.0 ),
  vec3( 237.0/255.0, 248.0/255.0, 251.0/255.0 )
};


GLVerts *glverts = NULL;



// GLFW Error callback

void GLFWErrorCallback( int error, const char* description )

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

  float objDiam = (obj->max - obj->min).length();
  if (eyeDistance == 0)
    eyeDistance = 10*objDiam;

  vec3 eyePosition( 0, 0, eyeDistance );


  mat4 MV;

  if (setX)
    MV = translate( -1 * eyePosition )
       * rotate( theta+90, vec3(0.5,2,0).normalize() );
  else
    MV = translate( -1 * eyePosition )
       * rotate( 90, vec3(1,0,0) );
  
  gpuProg->setMat4( "MV", MV );

  // model-view-projection transform (i.e. OCS-to-CCS)

  mat4 MVP = perspective( fovy, winWidth/(float)winHeight, eyeDistance-0.6*objDiam, eyeDistance+0.6*objDiam )
           * MV;

  gpuProg->setMat4( "MVP", MVP );

  // Light direction in VCS is above, to the right, and behind the
  // eye.  That's in direction (2,0.5,0.6) since the view direction is
  // down the -z axis.

  vec3 lightDir(2,0.5,0.6);
  lightDir = lightDir.normalize();

  gpuProg->setVec3( "lightDir", lightDir );

  // halfDir = vector halfway between lightDir and viewing direction.
  // For a light at infinity, lightDir is constant, so this can be
  // computed once and passed as a uniform variable to the shaders.
  //
  // This vector should be in the VCS.

  vec3 halfDir = (lightDir + vec3(0,0,1)).normalize();

  gpuProg->setVec3( "halfDir", halfDir );

  gpuProg->setInt( "useBlinn", (blinnOrPhong ? 1 : 0) );

  // Set the mode

  gpuProg->setInt( "mode", mode );

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

  // Message

  char msg[100];
  sprintf( msg, "%s lookup - %s", texModeString[textureMode], modeString[mode] );
  render_text( msg, 10, 10, window );

  cout << "\r  " << msg << "                                               \r";
  cout.flush();

  // Mipmap level legend

  if (mode == MIPMAP_MODE) {

    if (glverts == NULL)
      glverts = new GLVerts();

    seq<vec3> box(4);
    box.add( vec3(0,0,0) );
    box.add( vec3(1,0,0) );
    box.add( vec3(1,1,0) );
    box.add( vec3(0,1,0) );

    for (int i=0; i<NUM_MIPMAP_COLOURS; i++) {
      mat4 VP = translate( 0.85, 1 - (NUM_MIPMAP_COLOURS-1-i)*0.075, 0 ) * scale(0.1,0.05,1);
      glverts->draw( box, mipmapColours[i], GL_TRIANGLE_FAN, VP );

      sprintf( msg, "%d", i );
      render_text( msg, (1.0-(1.0-0.85)/2.0)*winWidth-20, winHeight - (NUM_MIPMAP_COLOURS-1-i)*0.075/2*winHeight, window );
    }
  }
}


// Update the object angle upon idle


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

    case 'X':
      setX = !setX;
      if (!setX)
	theta = 0;
      break;

    case 'B':
      blinnOrPhong = !blinnOrPhong;
      break;

    case 'A':
      showAxes = !showAxes;
      break;

    case '<':
    case ',':
      if (paused)
	theta -= 0.2*M_PI/180.0;
      break;

    case '>':
    case '.':
      if (paused)
	theta += 0.2*M_PI/180.0;
      break;

    case '+':
    case '=':
      mode++;
      if (modeString[mode][0] == '\0')
	mode = 0;
      break;

    case '-':
    case '_':
      mode--;
      if (mode < 0) {
	do
	  mode++;
	while (modeString[mode][0] != '\0');
	mode--;
      }
      break;

    case ']':
      textureMode = textureMode + 1;
      if (texModeString[textureMode][0] == '\0')
	textureMode = 0;
      obj->initTextures( textureMode );
      break;

    case '[':
      textureMode--;
      if (textureMode < 0) {
	do
	  textureMode++;
	while (texModeString[textureMode][0] != '\0');
	textureMode--;
      }
      obj->initTextures( textureMode );
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

    default:

      cout << "ESC   - exit" << endl
	   << "P     - pause" << endl
	   << "X     - toggle rotation axis" << endl
	   << "B     - toggle Blinn|Phong" << endl
	   << "A     - toggle axes" << endl
	   << "<     - rotate CW one step" << endl
	   << ">     - rotate CCW one step" << endl
	   << "+     - increment rendering mode" << endl
	   << "-     - decrement rendering mode" << endl
	   << "]     - increment texture mode" << endl
	   << "[     - decrement texture mode" << endl
           << "up    - move in" << endl
	   << "down  - move out" << endl
	   << "right - zoom in" << endl
	   << "left  - zoom out" << endl
	;
    }
  }
}


void windowSizeCallback( GLFWwindow* window, int width, int height )

{
  winWidth = width;
  winHeight = height;
  glViewport(0, 0, width, height);
}



// Main program


int main( int argc, char **argv )

{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " model.obj [shader_base_name]" << endl;
    exit(1);
  }

  char *shader_base_name;
  
  if (argc < 3)
    shader_base_name = DEFAULT_SHADER;
  else
    shader_base_name = argv[2];

  // Set up GLFW

  glfwSetErrorCallback( GLFWErrorCallback );
  
  if (!glfwInit())
    return 1;

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

  // Set up the window

  window = glfwCreateWindow( winWidth, winHeight, "Texture demo", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    cerr << "GLFW failed to create a window" << endl;

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
  glfwSetWindowSizeCallback( window, windowSizeCallback );

  // Init fonts

  initFont( FREETYPE_LIB, 20 ); // 20 = font height in pixels

  // Set up world objects

  obj = new wfModel( argv[1], textureMode );

  // Set up GPU

  char vert[1000];
  char frag[1000];
  sprintf( vert, "%s%s.vert", SHADER_DIR, shader_base_name );
  sprintf( frag, "%s%s.frag", SHADER_DIR, shader_base_name );

  gpuProg = new GPUProgram( vert, frag, "main()" );

  axes = new Axes();

  // Instructions

  cout << endl
       << "Press + or - to change the rendering mode." << endl
       << "Press [ or ] to change the texturing mode." << endl
       << "Press ? for more key commands." << endl
       << endl;

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

    glClearColor( 1, 1, 1, 1 );
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
