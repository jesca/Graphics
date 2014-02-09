
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>


#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h, f; // width and height
    // f is the cmd line option differentiating ambient, specular, diffuse
    float r, b, g;
    //rgb are values also passed in from cmd line
};


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;




//****************************************************
// Simple init function
//****************************************************
void initScene(){

  // Nothing to do here for this simple example.

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Draw a filled circle.  
//****************************************************


void circle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);
  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

   int i,j;  // Pixel indices
    float r,g,b;
    int ka;
    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));
    
    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));
    r=viewport.r;
    g=viewport.g;
    b=viewport.b;
    ka=.2;
    
    
    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            
            float dist = sqrt(sqr(x) + sqr(y));
            
            if (dist<=radius) {
                
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                
                if (viewport.f==1)
                {
                  r=ka*r;
                  g=ka*g;
                  b=ka*b;
                }


                setPixel(i,j, r, g, b);
                

                // This is amusing, but it assumes negative color values are treated reasonably.
                // setPixel(i,j, x/radius, y/radius, z/radius );
            }
            
            
        }
    }
    
    
    glEnd();
}

void sphere(float centerX, float centerY, float centerZ, float radius) {
    // Draw inner circle
    glBegin(GL_POINTS);
    
    // We could eliminate wasted work by only looping over the pixels
    // inside the sphere's radius.  But the example is more clear this
    // way.  In general drawing an object by loopig over the whole
    // screen is wasteful.
    
    int i,j;  // Pixel indices
    int r,g,b,ka;
    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));
    
    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));
    r=viewport.r;
    g=viewport.g;
    b=viewport.b;
    ka=1;
    
    
    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            
            float dist = sqrt(sqr(x) + sqr(y));
            
            if (dist<=radius) {
                
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                
                if (viewport.f==1)
                {
                  r=ka*r;
                  g=ka*g;
                  b=ka*b;
                }

                setPixel(i,j, r, g, b);
                

                // This is amusing, but it assumes negative color values are treated reasonably.
                // setPixel(i,j, x/radius, y/radius, z/radius );
            }
            
            
        }
    }
    
    
    glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Start drawing
  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************



int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);
  
  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
/*
int i;
  for (i=1; i<= 3; i++) {
    printf("\narg%d=%s", i, argv[i]);
 }*/





  const char *fxn=argv[1];
  viewport.f;
  viewport.r=atoi(argv[2]);
  viewport.g=atoi(argv[3]);
  viewport.b=atoi(argv[4]);

  

//ambience
  if (strcmp(fxn, "-ka") == 0) {
    viewport.f=1;
       printf ("Characters: %c %c \n", *fxn, 65);
}
   if (strcmp(fxn, "-ks") == 0) {
    viewport.f=2;
       printf ("Characters: %c %c \n", *fxn, 65);
}
//diffusion
if (strcmp(fxn, "-kd") == 0) {
    viewport.f=3;
       printf ("Characters: %c %c \n", *fxn, 65);

}
 
 



  /*

    if (fxn.compare("-kd")==1) {
    viewport.f=2;

           printf ("Decimals: %d %ld\n", viewport.f, 650000L);

  }
    if (fxn.compare("-ks")==1) {
        viewport.f=3;


   }
  else{ 
      std::cerr << "--NOT reading :[." << std::endl;

}*/


 


  


  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  glutReshapeFunc(myReshape);				// function to run when the window gets resized

  glutMainLoop();							// infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}








