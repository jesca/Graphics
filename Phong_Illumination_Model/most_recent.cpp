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
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h, func; // width and height
    // f is the cmd line option differentiating ambient, specular, diffuse
   // bool plcount[]={false,false,false,false,false}; //5 point lights total


};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;


    int plcount=0;
    int dlcount=0;
    float pl_array[5][6];
    float dl_array[5][6];
    float spec_coeff;
    float rgb[3];

   // float ambiencergb[3]={0,0,0};
    float rambience;
    float gambience;
    float bambience;
    float rgbdiffuse[3]={0,0,0};
    float rgbspec[3]={0,0,0};


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
// Norm (Vector length)
float norm(int length, float vec[]) {
    float sum = 0;
    for (int i = 0; i<length; i++) {
        sum += vec[i]*vec[i];
    }
    return sqrt(sum);
}

 
// Normalize
void normalize(int length, float vec[]) {
    float vecnorm = norm(length, vec);
    for (int i = 0; i<length; i++) {
        vec[i] = vec[i]/vecnorm;
    }
}



// Dot Product
float dot(int length, float vec1[], float vec2[]) {
    float sum = 0;
    for (int i = 0; i<length; i++) {
        sum += vec1[i]*vec2[i];
    }
    return sum;
}


 
// Diffuse
void diffuse(float pixelColor[], float l[], float n[]) {
    normalize(3, l);
    normalize(3, n);
    float lDotn = dot(3, l, n);
    for(int i = 0; i < 3; i++) {
        pixelColor[i] *= lDotn;
    }
}

// Specular
void specular(float pixelColor[], float r[], float v[], float p) {
    normalize(3, r);
    normalize(3, v);
    float rDotv = dot(3, r, v);
    float rDotvP = pow(rDotv, p);
    for(int i = 0; i < 3; i++) {
        pixelColor[i] *= rDotvP;
    }
}


 



void circle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);
  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

   int i,j;  // Pixel indices
    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));
    
    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));
        
    float final_rgb_diffuse[3];
    float final_rgb[3];
    float final_rgb_ambience[3];



   float dl_dir[3]; // directional light direction
    float dl_color[3];

    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            float dist = sqrt(sqr(x) + sqr(y));

            if (dist<=radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                float xyz[3]={x,y,z};
                normalize(3,xyz);

                 
                float dn_dotproduct;

            for (int i = 0; i < dlcount; i++)
             {
             final_rgb_ambience[i] += rambience * dl_dir[i][3];
             final_rgb_ambience[i] += gambience * dl_dir[i][4];
             final_rgb_ambience[i] += bambience * dl_dir[i][5];
          }



                //Diffuse component max(kd*I*(ˆl·nˆ),0)
                if (dlcount>0) {
                    for (int m = 0; m<dlcount; m++) {
                            dl_dir[0]=dl_array[m][0];
                            dl_dir[1]=dl_array[m][1];
                            dl_dir[2]=dl_array[m][2];
                            dl_color[0]=dl_array[m][3];
                            dl_color[1]=dl_array[m][4];
                            dl_color[2]=dl_array[m][5];
                            dl_color[3]=dl_array[m][5];
                            dn_dotproduct=dot(3, dl_dir,xyz);
                            for (int i=0; i<3; i++) {
                                final_rgb_diffuse[i]+=max(dl_dir[i]*dl_color[i]*dn_dotproduct,0.0f);
                            }
                    }
                }

                for (x=0; x<3; x++) {
                    final_rgb[x]+=final_rgb_diffuse[x] + final_rgb_ambience[x];
                }
                


                setPixel(i,j, final_rgb[0], final_rgb[1], final_rgb[2]);
                

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

  glClear(GL_COLOR_BUFFER_BIT);             // clear the color buffer

  glMatrixMode(GL_MODELVIEW);                   // indicate we are specifying camera transformations
  glLoadIdentity();                     // make sure transformation is "zero'd"


  // Start drawing
  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);

  glFlush();
  glutSwapBuffers();                    // swap buffers (we earlier set double buffer)
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************

/*-ka r g b
This is the ambient color coefficients of the sphere material. The parameters r g b are numbers
between 0 and 1 inclusive.

• -kd r g b
This is the diffuse color coefficients of the sphere material. The parameters r g b are numbers
between 0 and 1 inclusive.

• -ks r g b
This is the specular color coefficients of the sphere material. The parameters r g b are numbers
between 0 and 1 inclusive.

• -sp v
This is the power coefficient on the specular term. It is a number between 0 and max_float.

• -pl x y z r g b
This adds a point light to the scene. The x y z values are the location of the light. The r g b
values are it's color. Note that the x y z values are relative to the sphere. That is, the center of
the sphere is at the origin and the radius of the sphere defines one unit of length. The Y direction
is UP, the X direction is to the right on the screen, and the Z direction is "in your face." The
r g b value are between 0 and max_float, NOT between 0 and 1 (that is, the r g b values encode
the brightness of the light).

• -dl x y z r g b
This adds a directional light to the scene. The x y z values are the direction that the light
points in. The r g b values are it's color. See -pl for coordinate system notes
*/

//ambience

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

  for (int a=1; a<argc;) {
    const char *fxn=argv[a];

 
    //ambient; -kx r g b
    if ((strcmp(fxn, "-ka") == 0)) {
     //  printf ("%s \n", "reached first if");
    //update rgb values for ambience
                   printf ("%s \n", fxn);

        for (int color=0; color<3; color++){
            for (int arg=2; arg<=4; arg++){
            rgbambience[color]=atof(argv[arg]);
            arg++;
        }
        }
         a+=4;
        }

        else if ((strcmp(fxn, "-kd") == 0)) {
        for (int color=0; color<3; color++){
                               printf ("%s \n", fxn);

            for (int arg=2; arg<=4; arg++){
            rgbdiffuse[color]=atof(argv[arg]);
            arg++;
        }
        }
         a+=4;
        }

        else if ((strcmp(fxn, "-ks") == 0)) {
                   printf ("%s \n", fxn);

        for (int color=0; color<3; color++){
            for (int arg=2; arg<=4; arg++){
            rgbspec[color]=atof(argv[arg]);
            arg++;
        }
        }
         a+=4;
        }

  
        
//• -dl x y z r g b    -- x y z r g b values stored in a 2 dimensional array, accessed by pl_array[point light number][0-5, with 0 being x and b being 5]
    else if (strcmp(fxn, "-dl") == 0) {
     // for (int dl=0, dl<5;dl++) {
       // if plcount[dl]==false {
                           printf ("%s \n", fxn);

          for (int adddl=0; adddl<6; adddl++){
          dl_array[dlcount][adddl]=atof(argv[a+1+adddl]);
        }
        dlcount++;
        a+=7;

      }


    //specular: -sp v
    else if ((strcmp(fxn,"-sp"))) {
      spec_coeff=atof(argv[a+1]);
      a+=2;
    }

    //can have a total of 5 point lights, 5 direction lights, total 10 

    //-pl x y z r g b 
     else if (strcmp(fxn, "-pl") == 0) {
     // for (int pl=0, pl<5;pl++) {
       // if plcount[pl]==false {
          for (int addpl=0; addpl<6; addpl++){
          pl_array[plcount][addpl]=atof(argv[a+1+addpl]);
        }
        plcount++;
        a+=7;
    }
 

      else {
                std::cerr << "--arg not recognized" << std::endl;

      }
  }


    // testing functions here
    float test1[] = {3, 4, 2};
    float test2[] = {1, 2, 3};
    float dotp = dot(3, test1, test2);
    printf("should be 17.0: %f\n", dotp);
    
    float normtest = norm(3, test1);
    printf("should be 5.3851...: %f\n", normtest);
    
    normalize(3, test1);
    for(int i = 0; i < 3; i++) {
        printf("%f, \n", test1[i]);
    }
 
 


  


  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();                          // quick function to set up scene

  glutDisplayFunc(myDisplay);               // function to run when its time to draw something
  glutReshapeFunc(myReshape);               // function to run when the window gets resized

  glutMainLoop();                           // infinite loop that will keep drawing and resizing
  // and whatever else

  return 0;
}








