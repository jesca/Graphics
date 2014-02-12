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
Vec3 view={0,0,1};


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

<<<<<<< HEAD
 


// spec  
void spec(Vec3* spec, Vec3 I, Vec3 r, Vec3 v) {
    //    printf("dot %f\n", kd.y);
    float rDotv=dot(r,v);
       spec->x = ks.x*I.x * pow(max(rDotv,0.0f),spec_coeff);
       spec->y = ks.y*I.y *pow(max(rDotv,0.0f),spec_coeff);
       spec->z = ks.z*I.z * pow(max(rDotv,0.0f),spec_coeff);

=======
// Specular
void specular(Vec3* specPart, Vec3 I, Vec3 r, Vec3 v, float p) {
    float rDotv = dot(r, v);
    float rDotvP = pow(rDotv, p);
    specPart->x = fmax(ks.x * I.x * rDotvP, 0);
    specPart->y = fmax(ks.y * I.y * rDotvP, 0);
    specPart->z = fmax(ks.z * I.z * rDotvP, 0);
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
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

    
    
    for (i=0;i<viewport.w;i++) {
        for (j=0;j<viewport.h;j++) {
            
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            float dist = sqrt(sqr(x) + sqr(y));
<<<<<<< HEAD
            Vec3 dl_I;   
            Vec3 dl_L;  
=======
            Vec3 dl_I;
            Vec3 dl_L;
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
            Vec3 pl_I;
            Vec3 pl_L;
            
             
            if (dist<=radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(radius*radius-dist*dist);
                Vec3 n = {x,y,z};
<<<<<<< HEAD
                normalize(&n);  //normalizes vector
                
                
=======
                normalize(&n);
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
                
                for (int m = 0; m<dlcount; m++) {
                    //I = intensity r g b
                    dl_I.x =dl_array[m][3];
                    dl_I.y =dl_array[m][4];
                    dl_I.z =dl_array[m][5];
                    //L = direction xyz
<<<<<<< HEAD
                    dl_L.x= -dl_array[m][0];//direction
                    dl_L.y= -dl_array[m][1];
                    dl_L.z= -dl_array[m][2];
=======
                    dl_L.x= dl_array[m][0];//direction
                    dl_L.y= dl_array[m][1];
                    dl_L.z= dl_array[m][2];
                    
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
                    normalize(&dl_L);
                    diffuse(&final_rgb_diffuse, dl_I, dl_L, n);

                    
                    
<<<<<<< HEAD
                    
                    //diffuse reflection
                    float tempdot = dl_L.z * n.x + dl_L.y * n.y + n.z * dl_L.z;
                    float rx = dl_L.x + 2*(tempdot)*n.x;
                    float ry = dl_L.y + 2*(tempdot)*n.y;
                    float rz = dl_L.z + 2*(tempdot)*n.z;
                //    final_rgb_specular.x+=ks.x*dl_array[m][3]*max(tempdot,0.0f);
                 //    printf ("%s %f \n", " r spec: ", final_rgb_specular.x);
                //    final_rgb_specular.y+=ks.y*dl_array[m][4]*max(tempdot,0.0f);
                 //   printf ("%s %f \n", " gspec: ", final_rgb_specular.y);

                //    final_rgb_specular.z+=ks.z*dl_array[m][5]*max(tempdot,0.0f);
                    
                    diffuse(&final_rgb_diffuse, dl_I, dl_L, n);

                   

                }
                
                
                for (int m = 0; m < plcount; m++)
                {
                  
                    //I r g b
                    pl_I.x =pl_array[m][3];
                    pl_I.y =pl_array[m][4];
                    pl_I.z =pl_array[m][5];
                    //dir x y z 
                    pl_L.x= (pl_array[m][0]);//direction
                    pl_L.y= (pl_array[m][1]);
                    pl_L.z= (pl_array[m][2]);
                    normalize(&pl_L);
                 
                    float pldotn =dot(n,pl_L);
=======
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
                    
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
                    
                    //r=d-2(d*nnorm)norm -- REFLECTION will be passed back to pl_L to spec function
                    float rx = -pl_L.x + 2*(pldotn)*n.x;
                    float ry = -pl_L.y + 2*(pldotn)*n.y;
                    float rz = -pl_L.z + 2*(pldotn)*n.z;
                    pl_L.x=rx;
                    pl_L.y=ry;
                    pl_L.z=rz;
                    
                    // final_rgb_diffuse.x+=kd.x*pl_array[m][3]*max(pldotn,0.0f);
                   // final_rgb_diffuse.y+=kd.y*pl_array[m][4]*max(pldotn,0.0f);
                    //final_rgb_diffuse.z+=kd.z*pl_array[m][5]*max(pldotn,0.0f);

                  
//                   spec(&final_rgb_specular, pl_I,pl_L,view);

                }

                
                
<<<<<<< HEAD
            
            spec(&final_rgb_specular, pl_I,pl_L,view);
            diffuse(&final_rgb_diffuse, dl_I, dl_L, n);
 
        //        printf ("%s %f \n", "r spec final ", final_rgb_specular.x);
        //        printf ("%s %f \n", " g spec: ", final_rgb_specular.y);
            final_rgb.x = final_rgb_diffuse.x + final_rgb_ambience.x + final_rgb_specular.x;
            final_rgb.y = final_rgb_diffuse.y + final_rgb_ambience.y + final_rgb_specular.y;
            final_rgb.z = final_rgb_diffuse.z + final_rgb_ambience.z + final_rgb_specular.z;
            setPixel(i,j, final_rgb_specular.x, final_rgb_specular.y, final_rgb_specular.z);

               // printf ("%s %f \n", "r spec final ", final_rgb.x);
                 //     printf ("%s %f \n", " g final: ", final_rgb.y);
=======
//                printf("x = %f \n", final_rgb_diffuse.x);
//                printf("y = %f \n", final_rgb_diffuse.y);
//                printf("z = %f \n", final_rgb_diffuse.z);
            
                
            final_rgb.x += final_rgb_diffuse.x + final_rgb_ambience.x + final_rgb_specular.x;
            final_rgb.y += final_rgb_diffuse.y + final_rgb_ambience.y + final_rgb_specular.y;
            final_rgb.z += final_rgb_diffuse.z + final_rgb_ambience.z + final_rgb_specular.z;
                
            setPixel(i,j, final_rgb_diffuse.x, final_rgb_diffuse.y, final_rgb_diffuse.z);

                
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
                
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
        
        
  
        else if (strcmp(fxn, "-dl") == 0) {
            
            for (int adddl=0; adddl<6; adddl++){
                dl_array[dlcount][adddl]=atof(argv[a+1+adddl]);
            }
            dlcount++;
            a+=7;
            
        }
        
        //-pl x y z r g b
        else if (strcmp(fxn, "-pl") == 0) {
<<<<<<< HEAD
=======
            // for (int pl=0, pl<5;pl++) {
            // if plcount[pl]==false {
            printf("a = %d\n", a);
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
            for (int addpl=0; addpl<6; addpl++){
                pl_array[plcount][addpl]=atof(argv[a+1+addpl]);
            }
            plcount++;
            
            a+=7;
            
        }
        
        
        //specular: -sp v
        else if ((strcmp(fxn,"-sp"))) {
            spec_coeff=atof(argv[a+1]);
            a+=2;
        }
        //don't get rid of this..without this it runs out of bounds and seg faults
        else if ((strcmp(fxn,"-none"))) {
            spec_coeff=atof(argv[a+1]);
            a+=2;
        }
        
    }
    
    
<<<<<<< HEAD
 
=======
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
    
    
>>>>>>> 5b6a1f474458b07ae3486b383033174ec4c3eeaf
    
    
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

    
    
    
    
    
