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
    float ka[3]={0,0,0};
    float kd[3]={0,0,0};
    float ks[3]={0,0,0};
    float viewx = 0.0, viewy = 0.0, viewz = 1.0;



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
void diffuse(float I[], float l[], float n[]) {
    normalize(3, l);
    normalize(3, n);
    float lDotn = dot(3, l, n);
    for(int i = 0; i < 3; i++) {
        I[i] =I[i]* lDotn*ka[i];
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
 
 
 
    //AMBIANCE
    // ka * I ... ka=.1kd
     if (dlcount>1) {
            for (int k = 0; k < dlcount; k++)
             {

             final_rgb_ambience[0] += ka[0] * (dl_array[k][3]);
             final_rgb_ambience[1] += ka[1] * (dl_array[k][4]);
             final_rgb_ambience[2] += ka[2] *(dl_array[k][5]);
            }
           
    }

 

    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            float dist = sqrt(sqr(x) + sqr(y));
            float final_rgb_diffuse[3];

    float final_rgb_ambience[3];
    float final_rgb_specular[3];

            float dl_I[3];
            float dl_L[3];
            float dn_dotproduct;
  

            if (dist<=radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                float xyz[3]={x,y,z};
                normalize(3,xyz);
                            float length = sqrt(sqr(x) + sqr(y) + sqr(z));

            float normalx = x/length;
                float normaly = y/length;
                float normalz = z/length;
/*
                float normalx = x/length;
                float normaly = y/length;
                float normalz = z/length;
                
                for (int ctr = 0; ctr < numpointlights; ctr ++)
                {
                    float lightx = (pointlight[ctr][0]*radius - x);
                    float lighty = (pointlight[ctr][1]*radius - y);
                    float lightz = (pointlight[ctr][2]*radius - z);
                    float lightlength = sqrt(sqr(lightx) + sqr(lighty) + sqr(lightz));
                    lightx /= lightlength;
                    lighty /= lightlength;
                    lightz /= lightlength;
                    float dotproduct = lightx * normalx + lighty * normaly + lightz * normalz;
                    float reflectedx = -lightx + 2*(dotproduct)*normalx;
                    float reflectedy = -lighty + 2*(dotproduct)*normaly;
                    float reflectedz = -lightz + 2*(dotproduct)*normalz;
                    
                    
                    totaldiffuser += diffred * pointlight[ctr][3] * max(dotproduct, 0.0f);
                    totaldiffuseg += diffgreen * pointlight[ctr][4] * max(dotproduct, 0.0f);
                    totaldiffuseb += diffblue * pointlight[ctr][5] * max(dotproduct, 0.0f);
                    
                    
                    dotproduct = reflectedx * viewx + reflectedy * viewy + reflectedz * viewz;
                    totalspecr += specred * pointlight[ctr][3] * pow(max(dotproduct, 0.0f), specpower);
                    totalspecg += specgreen * pointlight[ctr][4] * pow(max(dotproduct, 0.0f), specpower);
                    totalspecb += specblue * pointlight[ctr][5] * pow(max(dotproduct, 0.0f), specpower);
                }
                
                for (int ctr = 0; ctr < numdirectionallights; ctr ++)
                {
                    float lightx = -directionallight[ctr][0];
                    float lighty = -directionallight[ctr][1];
                    float lightz = -directionallight[ctr][2];
                    float lightlength = sqrt(sqr(lightx) + sqr(lighty) + sqr(lightz));
                    lightx /= lightlength;
                    lighty /= lightlength;
                    lightz /= lightlength;
                    float dotproduct = lightx * normalx + lighty * normaly + lightz * normalz;
                    float reflectedx = -lightx + 2*(dotproduct)*normalx;
                    float reflectedy = -lighty + 2*(dotproduct)*normaly;
                    float reflectedz = -lightz + 2*(dotproduct)*normalz;
                    totaldiffuser += diffred * directionallight[ctr][3] * max(dotproduct, 0.0f);
                    totaldiffuseg += diffgreen * directionallight[ctr][4] * max(dotproduct, 0.0f);
                    totaldiffuseb += diffblue * directionallight[ctr][5] * max(dotproduct, 0.0f);
                    dotproduct = reflectedx * viewx + reflectedy * viewy + reflectedz * viewz;
                    totalspecr += specred * directionallight[ctr][3] * pow(max(dotproduct, 0.0f), specpower);
                    totalspecg += specgreen * directionallight[ctr][4] * pow(max(dotproduct, 0.0f), specpower);
                    totalspecb += specblue * directionallight[ctr][5] * pow(max(dotproduct, 0.0f), specpower);
                }*/

                      for (int m = 0; m<dlcount; m++) {
                            //I = intensity r g b
                          
                            dl_I[0]=dl_array[m][3];
                            dl_I[1]=dl_array[m][4];
                            dl_I[2]=dl_array[m][5];
                        //L = direction xyz
                            dl_L[0]=-dl_array[m][0];//direction 
                            dl_L[1]=-dl_array[m][1];
                            dl_L[2]=-dl_array[m][2];   
                            normalize(3,dl_L);

                            //should already be normalized
                            dn_dotproduct=dot(3, dl_L,xyz);
                      //    printf ("%s %f \n", "dotproduct: ", dn_dotproduct);
                            final_rgb_diffuse[0]+=(kd[0]*dl_I[0]*max(dn_dotproduct,0.0f));
                            final_rgb_diffuse[1]+=(kd[1]*dl_I[1]*max(dn_dotproduct,0.0f));
                            final_rgb_diffuse[2]+=(kd[2]*dl_I[2]*max(dn_dotproduct,0.0f));

                    }
                      printf ("%s %f \n", "total dif r: ", final_rgb_diffuse[0]);
                  printf ("%s %f \n", "total d g: ", final_rgb_diffuse[1]);

                  printf ("%s %f \n", "total d b: ", final_rgb_diffuse[2]);
                
        for (int ctr = 0; ctr < plcount; ctr ++)
                {
                    float lightx = (pl_array[ctr][0]*radius - x);
                    float lighty = (pl_array[ctr][1]*radius - y);
                    float lightz = (pl_array[ctr][2]*radius - z);
                    float lightlength = sqrt(sqr(lightx) + sqr(lighty) + sqr(lightz));
                    lightx /= lightlength;
                    lighty /= lightlength;
                    lightz /= lightlength;
                    float dotproduct = lightx * normalx + lighty * normaly + lightz * normalz;
                    float reflectedx = -lightx + 2*(dotproduct)*normalx;
                    float reflectedy = -lighty + 2*(dotproduct)*normaly;
                    float reflectedz = -lightz + 2*(dotproduct)*normalz;
                    
                    
                  //  final_rgb_diffuse[0] += (kd[0] * pl_array[ctr][3] * max(dotproduct, 0.0f));
                  //  final_rgb_diffuse[1] += kd[1] * pl_array[ctr][4] * max(dotproduct, 0.0f);
                  //  final_rgb_diffuse[2] += kd[2] * pl_array[ctr][5] * max(dotproduct, 0.0f);
                    
                    
                    dotproduct = reflectedx * viewx + reflectedy * viewy + reflectedz * viewz;
                    final_rgb_specular[0] += ks[0] * pl_array[ctr][3] * pow(max(dotproduct, 0.0f), spec_coeff);
                    final_rgb_specular[1] += ks[1] * pl_array[ctr][4] * pow(max(dotproduct, 0.0f), spec_coeff);
                    final_rgb_specular[2] += ks[2] * pl_array[ctr][5] * pow(max(dotproduct, 0.0f), spec_coeff);
                }
  
 
                     printf ("%s %f \n", "total spec r: ", final_rgb_specular[0]);
                  printf ("%s %f \n", "total spec g: ", final_rgb_specular[1]);

                  printf ("%s %f \n", "total spec b: ", final_rgb_specular[2]);
                  

                setPixel(i,j, final_rgb_diffuse[0]+final_rgb_specular[0], final_rgb_diffuse[1]+final_rgb_specular[1],final_rgb_diffuse[2]+final_rgb_specular[2]);
            

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

 
     
 

  
 

  for (int a=1; a<argc;a++) {

 
        if (strcmp(argv[a], "-ka") == 0)
        {
            ka[0] = atof(argv[a+1]);
            ka[1] = atof(argv[a+2]);
            ka[2] = atof(argv[a+3]);
        }

 
 if (strcmp(argv[a], "-kd") == 0)
        {
            kd[0] = atof(argv[a+1]);
            kd[1] = atof(argv[a+2]);
            kd[2] = atof(argv[a+3]);
        }

 

    if (strcmp(argv[a], "-ks") == 0)
        {
            ks[0] = atof(argv[a+1]);
            ks[1] = atof(argv[a+2]);
            ks[2] = atof(argv[a+3]);
        }

        
    else if (strcmp(fxn, "-dl") == 0) {
     // for (int dl=0, dl<5;dl++) {
       // if plcount[dl]==false {

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

 
       
  }
    
  //This initializes glut
  glutInit(&argc, argv);
  
  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
/*
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
    }*/
 
 


  


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








