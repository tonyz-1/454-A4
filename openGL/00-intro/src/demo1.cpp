// OpenGL demo 1
//
// Open a window.


#include "headers.h"


// Drawing function

void display()

{
}


// Main program

int main( int argc, char **argv )

{
  // Set up GLFW (= Graphics Library Framework)

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

  window = glfwCreateWindow( 800, 600, "Demo 1", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    return 1;
  }

  // Make the following changes apply to *this* window
  
  glfwMakeContextCurrent( window );

  // redraw at most every 1 screen scan
  
  glfwSwapInterval( 1 );
  
  // Set OpenGL function bindings

  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Run

  while (!glfwWindowShouldClose( window )) {

    // Clear the screen

    glClearColor( 0.0, 0.0, 0.0, 0.0 ); // (try different colours)
    glClear( GL_COLOR_BUFFER_BIT );

    // Draw the scene

    display();

    // Swap buffers

    glfwSwapBuffers( window );
    
    // Check for new events

    glfwWaitEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}

