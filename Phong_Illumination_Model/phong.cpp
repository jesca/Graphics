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

struct Vec3 {
    float x, y, z;
};


int plcount=0;
int dlcount=0;
float pl_array[5][6];
float dl_array[5][6];
float spec_coeff=0;
Vec3 rgb;

Vec3 ka = {0,0,0};
Vec3 kd = {0.0,0.0,0.0};
Vec3 ks = {0.0,0.0,0.0};
Vec3 view={0.0,0.0,1.0};


//****************************************************
// Spacebar Exit
//****************************************************

//http://stackoverflow.com/questions/6880863/keyboard-event-in-opengl-with-glut
void handleSpacebar(unsigned char key, int x, int y)
{
    if (key == 32)
    {
        exit(0);
    }
}

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

// Dot Product
float dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Norm (Vector length)
float norm(Vec3 vec) {
    return sqrt(sqr(vec.x)+sqr(vec.y)+sqr(vec.z));
}

// Normalize
void normalize(Vec3* vec) {
    float vecnorm = norm(*vec);
    vec->x = vec->x/vecnorm;
    vec->y = vec->y/vecnorm;
    vec->z = vec->z/vecnorm;
}

// Diffuse
void diffuse(Vec3* diffusePart, Vec3 I, Vec3 l, Vec3 n) {
    float lDotn = dot(l, n);
    diffusePart->x += kd.x * I.x * fmax(lDotn, 0.0f);
    diffusePart->y +=  kd.y * I.y * fmax(lDotn, 0.0f);
    diffusePart->z += kd.z * I.z * fmax(lDotn, 0.0f);
    
}




// spec
void spec(Vec3* spec, Vec3 I, Vec3 r, Vec3 v) {
    //    printf("dot %f\n", kd.y);
    float rDotv=dot(r,v);
    spec->x += ks.x*I.x * pow(max(rDotv,0.0f),spec_coeff);
    spec->y += ks.y*I.y *pow(max(rDotv,0.0f),spec_coeff);
    spec->z += ks.z*I.z * pow(max(rDotv,0.0f),spec_coeff);
}

/*
 // Specular
 void specular(Vec3* specPart, Vec3 I, Vec3 r, Vec3 v) {
 float rDotv = dot(r, v);
 float rDotvP = pow(rDotv, spec_coeff);
 specPart->x += fmax(ks.x * I.x * rDotvP, 0.0);
 specPart->y += fmax(ks.y * I.y * rDotvP, 0.0);
 specPart->z += fmax(ks.z * I.z * rDotvP, 0.0);
 }*/


void circle(float centerX, float centerY, float radius) {
    // Draw inner circle
    glBegin(GL_POINTS);
    // We could eliminate wasted work by only looping over the pixels
    // inside the sphere's radius.  But the example is more clear this
    // way.  In general drawing an object by loopig over the whole
    // screen is wasteful.
    Vec3 final_rgb_ambience ={0,0,0};
    
    int i,j;  // Pixel indices
    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));
    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));
    
    
    
    //add to ambient term
    
    for (int i = 0; i < plcount; i ++)
    {
        final_rgb_ambience.x += ka.x * (pl_array[i][3]);
        final_rgb_ambience.y += ka.y * (pl_array[i][4]);
        final_rgb_ambience.z += ka.z * (pl_array[i][5]);
    }
    
    for (int i = 0; i < dlcount; i ++)
    {
        final_rgb_ambience.x += ka.x * (dl_array[i][3]);
        final_rgb_ambience.y += ka.y * (dl_array[i][4]);
        final_rgb_ambience.z += ka.z * (dl_array[i][5]);
    }
    
    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            float dist = sqrt(sqr(x) + sqr(y));
            Vec3 dl_I;
            Vec3 dl_L;
            Vec3 dl_ref;
            Vec3 pl_I;
            Vec3 pl_L;
            Vec3 pl_ref;
            
            Vec3 final_rgb_diffuse = {0.0,0.0,0.0};
            Vec3 final_rgb_specular = {0.0,0.0,0.0};
            
            if (dist<=radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                x = x/radius;
                y = y/radius;
                z = z/radius;
                Vec3 n = {x,y,z};
                normalize(&n);  //normalizes vector
                
                
                for (int m = 0; m<dlcount; m++) {
                    //I = intensity r g b
                    dl_I.x =dl_array[m][3];
                    dl_I.y =dl_array[m][4];
                    dl_I.z =dl_array[m][5];
                    //L = direction xyz
                    dl_L.x= -dl_array[m][0];//direction
                    dl_L.y= -dl_array[m][1];
                    dl_L.z= -dl_array[m][2];
                    
                    
                    normalize(&dl_L);
                    float dldotn=dot(dl_L, n);
                    
                    float refx = -dl_L.x + 2*(dldotn)*n.x;
                    float refy = -dl_L.y + 2*(dldotn)*n.y;
                    float refz = -dl_L.z + 2*(dldotn)*n.z;
                    dl_ref.x=refx;
                    dl_ref.y=refy;
                    dl_ref.z=refz;
                    //diffuse reflection
                    diffuse(&final_rgb_diffuse, dl_I, dl_L, n);
                    spec(&final_rgb_specular, dl_I,dl_ref,view);
                    
                }
                
                for (int m = 0; m<plcount; m++) {
                    
                    //L = direction xyz
                    pl_L.x= pl_array[m][0]-x;
                    //     printf ("%s %f \n", "r in I ", pl_L.x);
                    
                    pl_L.y= pl_array[m][1]-y;
                    pl_L.z= pl_array[m][2]-z;
                    //I = intensity r g b
                    pl_I.x =pl_array[m][3];
                    pl_I.y =pl_array[m][4];
                    pl_I.z =pl_array[m][5];
                    
                    normalize(&pl_L);
                    
                    //r=d-2(d*nnorm)norm
                    float pldotn=dot(pl_L, n);
                    float refx = -pl_L.x + 2*(pldotn)*n.x;
                    float refy = -pl_L.y + 2*(pldotn)*n.y;
                    float refz = -pl_L.z + 2*(pldotn)*n.z;
                    pl_ref.x=refx;
                    pl_ref.y=refy;
                    pl_ref.z=refz;
                    spec(&final_rgb_specular, pl_I,pl_ref,view);
                    diffuse(&final_rgb_diffuse, pl_I,pl_L,n);
                }
                
                //  setPixel(i,j, final_rgb_specular.x+final_rgb_ambience.x+final_rgb_diffuse.x, final_rgb_specular.y+final_rgb_ambience.y+final_rgb_diffuse.y, final_rgb_specular.z+final_rgb_ambience.z+final_rgb_diffuse.z);
                setPixel(i,j, final_rgb_diffuse.x +final_rgb_specular.x+final_rgb_ambience.x, final_rgb_diffuse.y +final_rgb_specular.y+final_rgb_ambience.y, final_rgb_diffuse.z  +final_rgb_ambience.z);
                
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





int main(int argc, char *argv[]) {
    
    for (int i = 1; i < argc; i ++)
    {
        if (strcmp(argv[i], "-ka") == 0)
        {
            ka.x = atof(argv[i+1]);
            ka.y = atof(argv[i+2]);
            ka.z = atof(argv[i+3]);
        }
        
        else if (strcmp(argv[i], "-kd") == 0)
        {
            kd.x = atof(argv[i+1]);
            kd.y = atof(argv[i+2]);
            kd.z = atof(argv[i+3]);
        }
        
        else if (strcmp(argv[i], "-ks") == 0)
        {
            ks.x = atof(argv[i+1]);
            ks.y = atof(argv[i+2]);
            ks.z = atof(argv[i+3]);
        }
        
        else if (strcmp(argv[i], "-dl") == 0)
        {
            for(int m = 0; m < 6; m++)
            {
                dl_array[dlcount][m] = atof(argv[i + 1 + m]);
            }
            dlcount++;
            
        }
        else if (strcmp(argv[i], "-sp") == 0)
        {
            spec_coeff = atof(argv[i+1]);
            
        }
        
        else if (strcmp(argv[i], "-pl") == 0)
        {
            for(int m= 0; m < 6; m++)
            {
                pl_array[plcount][m] = atof(argv[i + 1 + m]);
            }
            plcount++;
        }
        
        
    }
    //This initializes glut
    glutInit(&argc, argv);
    
    //This tells glut to use a double-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    
    // testing functions here
    Vec3 test1 = {3, 4, 2};
    Vec3 test2 = {1, 2, 3};
    
    float dotp = dot(test1, test2);
    printf("should be 17.0: %f\n", dotp);
    
    float normtest = norm(test1);
    printf("should be 5.3851...: %f\n", normtest);
    
    
    normalize(&test1);
    printf("should be 0.557086: %f, \n", test1.x);
    printf("should be 0.742781: %f, \n", test1.y);
    printf("should be 0.371391: %f, \n", test1.z);
    
    normalize(&test2);
    Vec3 test3 = {0, 0, 0};
    Vec3 test4 = {3, 2, 1};
    diffuse(&test3, test4, test1, test2);
    printf("%f, \n", test3.x);
    printf("%f, \n", test3.y);
    printf("%f, \n", test3.z);
    
    
    
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
    
    //exit on spacebar
    glutKeyboardFunc(handleSpacebar);
    glutMainLoop();                           // infinite loop that will keep drawing and resizing
    // and whatever else
    
    return 0;
}
