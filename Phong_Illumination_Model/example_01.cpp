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
float spec_coeff;
Vec3 rgb;

// float ambiencergb[3]={0,0,0};
Vec3 ka = {0,0,0};
Vec3 kd = {0,0,0};
Vec3 ks = {0,0,0};


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
    return sqrt(dot(vec, vec));
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
    diffusePart->x = fmax(kd.x * I.x * lDotn, 0);
    diffusePart->y = fmax(kd.y * I.y * lDotn, 0);
    diffusePart->z = fmax(kd.z * I.z * lDotn, 0);
    
}

// Specular
void specular(Vec3* specPart, Vec3 I, Vec3 r, Vec3 v, float p) {
    float rDotv = dot(r, v);
    float rDotvP = pow(rDotv, p);
    specPart->x = fmax(ks.x * I.x * rDotvP, 0);
    specPart->y = fmax(ks.y * I.y * rDotvP, 0);
    specPart->z = fmax(ks.z * I.z * rDotvP, 0);
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
    
    Vec3 final_rgb_diffuse = {0,0,0};
    Vec3 final_rgb_ambience = ka;
    Vec3 final_rgb_specular = {0,0,0};;
    Vec3 final_rgb = {0,0,0};
    
    //AMBIANCE
    // ka * I ... ka=.1kd
    if (dlcount>1) {
        for (int k = 0; k < dlcount; k++)
        {
            
//            final_rgb_ambience[1] += ka[0] * (dl_array[k][3]);
//            final_rgb_ambience[2] += ka[1] * (dl_array[k][4]);
//            final_rgb_ambience[3] += ka[2] *(dl_array[k][5]);
        }
        
    }
    
    
    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            float dist = sqrt(sqr(x) + sqr(y));
            Vec3 dl_I;
            Vec3 dl_L;
            Vec3 pl_I;
            Vec3 pl_L;
            
            if (dist<=radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                Vec3 n = {x,y,z};
                normalize(&n);
                
                for (int m = 0; m<dlcount; m++) {
                    //I = intensity r g b
                    dl_I.x =dl_array[m][3];
                    dl_I.y =dl_array[m][4];
                    dl_I.z =dl_array[m][5];
                    //L = direction xyz
                    dl_L.x= dl_array[m][0];//direction
                    dl_L.y= dl_array[m][1];
                    dl_L.z= dl_array[m][2];
                    
                    normalize(&dl_L);
                    diffuse(&final_rgb_diffuse, dl_I, dl_L, n);

                    
                    
                }
                for (int m = 0; m<plcount; m++) {
                    //I = intensity r g b
                    pl_I.x =pl_array[m][3];
                    pl_I.y =pl_array[m][4];
                    pl_I.z =pl_array[m][5];
                    //L = direction xyz
                    pl_L.x= pl_array[m][0];//direction
                    pl_L.y= pl_array[m][1];
                    pl_L.z= pl_array[m][2];
                    
                    normalize(&pl_L);
                    diffuse(&final_rgb_diffuse, pl_I, pl_L, n);
                    
                    
                    
                }
                
                
//                printf("x = %f \n", final_rgb_diffuse.x);
//                printf("y = %f \n", final_rgb_diffuse.y);
//                printf("z = %f \n", final_rgb_diffuse.z);
            
                
            final_rgb.x += final_rgb_diffuse.x + final_rgb_ambience.x + final_rgb_specular.x;
            final_rgb.y += final_rgb_diffuse.y + final_rgb_ambience.y + final_rgb_specular.y;
            final_rgb.z += final_rgb_diffuse.z + final_rgb_ambience.z + final_rgb_specular.z;
                
            setPixel(i,j, final_rgb_diffuse.x, final_rgb_diffuse.y, final_rgb_diffuse.z);

                
                
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
        printf("%s \n", fxn);
        //ambient; -kx r g b
        if ((strcmp(fxn, "-ka") == 0)) {
            //  printf ("%s \n", "reached first if");
            //update rgb values for ambience
            printf ("%s \n", fxn);
            
            
//            for (int arg=2; arg<=4; arg++){
//                ka.x=atof(argv[arg]);
//                arg++;
//            }
//            for (int arg=2; arg<=4; arg++){
//                ka.y=atof(argv[arg]);
//                arg++;
//            }
//            for (int arg=2; arg<=4; arg++){
//                ka.z=atof(argv[arg]);
//                arg++;
//            }
            ka.x=atof(argv[2]);
            ka.y=atof(argv[3]);
            ka.z=atof(argv[4]);
            a+=4;
        }
        
        else if ((strcmp(fxn, "-kd") == 0)) {
            kd.x=atof(argv[2]);
            kd.y=atof(argv[3]);
            kd.z=atof(argv[4]);
            a+=4;
        }
        
        else if ((strcmp(fxn, "-ks") == 0)) {
            printf ("%s \n", fxn);
            
            ks.x=atof(argv[2]);
            ks.y=atof(argv[3]);
            ks.z=atof(argv[4]);
            a+=4;
        }
        
        
        
        //• -dl x y z r g b    -- x y z r g b values stored in a 2 dimensional array, accessed by pl_array[point light number][0-5, with 0 being x and b being 5]
        else if (strcmp(fxn, "-dl") == 0) {
            // for (int dl=0, dl<5;dl++) {
            // if plcount[dl]==false {
            
            for (int adddl=0; adddl<6; adddl++){
                //   printf ("%s \n", argv[a+1+adddl]);
                //   printf ("%d \n", dlcount);
                //   printf ("%d \n", adddl);
                
                
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
            printf("a = %d\n", a);
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
    
    glutMainLoop();                           // infinite loop that will keep drawing and resizing
    // and whatever else
    
    return 0;
}

    
    
    
    
    
