/*
   Created by Alexander Louie on 10/24/2019
 *  Final Homeowrk: Textures
 *
 *  Demonstrates the use of textures
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <stdbool.h>
#include <math.h>
#include <time.h>

int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
GLuint texture[20]; // Texture names

//Person positions
double f_x = -3;
double f_y = 1.0;
double f_z = -2.0;

//Camera positions
double cam_x = 0;
double cam_y = 0;
double cam_z = 0;

//First person angles
double f_th = 0;
double f_ph = 0;

GLUquadric* qobj;

//Particle stuff

int ParticleCount = 200;

typedef struct {
  double Xpos;
  double Ypos;
  double Zpos;
  double Xmov;
  double Zmov;
  double Red;
  double Green;
  double Blue;
  double Direction;
  double Acceleration;
  double Deceleration;
  double Scalez;
  bool Visible;
}PARTICLES;

PARTICLES Particle1[500];
PARTICLES Particle2[500];
PARTICLES Particle3[500];
PARTICLES Particle4[500];

void glCreateParticles (double x, double y, double z, PARTICLES Particle[]) {
  int i;
  for (i = 1; i < ParticleCount; i++)
  {
    Particle[i].Xpos = x;
    Particle[i].Ypos = y;
    Particle[i].Zpos = z;
    Particle[i].Xmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * 
    rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1
    ) * rand()%11) + 1) * 0.005);
    Particle[i].Zmov = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * 
    rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1
    ) * rand()%11) + 1) * 0.005);
    Particle[i].Red = 1;
    Particle[i].Green = 0;
    Particle[i].Blue = 0;
    Particle[i].Scalez = 0.25;
    Particle[i].Direction = 0;
    Particle[i].Acceleration = (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1) * 
    rand()%11) + 1) * 0.005) - (((((((2 - 1 + 1) * rand()%11) + 1) - 1 + 1
    ) * rand()%11) + 1) * 0.005);
    Particle[i].Deceleration = 0.025;
  }
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

static void glDrawParticles (PARTICLES Particle[]) {
  int i;
  for (i = 1; i < ParticleCount; i++)
  {
    glPushMatrix();

    glTranslatef (Particle[i].Xpos, Particle[i].Ypos, Particle[i].Zpos);
    glRotatef (Particle[i].Direction - 90, 0, 0, 1);

    glScalef (Particle[i].Scalez, Particle[i].Scalez, Particle[i].Scalez);

      
    // glBlendFunc (GL_DST_COLOR, GL_ZERO);
    // glBindTexture (GL_TEXTURE_2D, texture[0]);

    // ball(0, 0, 0, .5);

    glBlendFunc (GL_ONE, GL_ONE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture[10]);

    if (i % 3 == 0) {
      glColor3f (1, 0.2, 0);
    } else if (i % 3 == 1){
      glColor3f(1, 0, 0);
    } else {
      glColor3f(1,1, 0);
    }
    glDisable(GL_CULL_FACE);
    glBegin (GL_TRIANGLES);
    glTexCoord2d (0, 0);
    glVertex3f (-1, -1, 0);
    glTexCoord2d (1, 0);
    glVertex3f (1, -1, 0);
    glTexCoord2d (0.5, 1);
    glVertex3f (1, 1, 0);
    glEnd();
    glEnable(GL_CULL_FACE);

    glPopMatrix();

  }
  }

void glUpdateParticles (double x, double y, double z, PARTICLES Particle[]) {
  int i;
  for (i = 1; i < ParticleCount; i++)
    {

    Particle[i].Ypos = Particle[i].Ypos + fabs(Particle[i].Acceleration) * .5;

    Particle[i].Xpos = Particle[i].Xpos + Particle[i].Xmov * .1;
    Particle[i].Zpos = Particle[i].Zpos + Particle[i].Zmov * .1;

    Particle[i].Direction = Particle[i].Direction + ((((((int
    )(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);

    if (Particle[i].Ypos < y - 0.1 || Particle[i].Ypos > y + 1.5 || Particle[i].Xpos > x + 0.5 || Particle[i].Xpos < x - 0.5 || Particle[i].Zpos < z - 0.5 || Particle[i].Zpos > z + 0.5)
    {
    Particle[i].Xpos = x;
    Particle[i].Ypos = y;
    Particle[i].Zpos = z;
    if (i % 2 == 1) {
      Particle[i].Red = 1;
      Particle[i].Green = 0;
      Particle[i].Blue = 0;      
    } else {
      Particle[i].Red = 1;
      Particle[i].Green = .2;
      Particle[i].Blue = 0;
    }

    Particle[i].Direction = 0;
    Particle[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5
    ) - 1 + 1) * rand()%11) + 1) * 0.01;
    Particle[i].Deceleration = 0.0025;
    }
  }
}


float * calculateNormal(float * v1, float * v2, float *v3, GLfloat * N) {
    GLfloat A[3] = {v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
    GLfloat B[3] = {v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};
    N[0] = (A[1] * B[2] - A[2] * B[1]) * -1;
    N[1] = (A[2] * B[0] - A[0] * B[2]) * -1;
    N[2] = (A[0] * B[1] - A[1] * B[0]) * - 1;
     

  

    float length = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);

    for(int i = 0; i < 3; i ++ ){
      N[i] = N[i] / length;
    }

    return N;

}

static void flatTop(double x,double y,double z, double x_r, double y_r, double z_r, double rotation, 
  double r,double d, int tex)
{
   int i,k;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[tex]);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotatef(rotation, x_r, y_r, z_r);
   glScaled(r,r,d);
   //  Head & Tail
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glColor3f(1.00,0.77,0.36);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double x_r, double y_r, double z_r,
                 double th, int tex)
{
  glPushMatrix();
   //  Save transformation

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[tex]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


   //  Offset
   glTranslated(x,y,z);
   glRotated(th,x_r,y_r,z_r);
   glScaled(dx,dy,dz);
    glColor3f(1,1,1);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 1, 0);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1, 1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(+1,-1, 1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,+1, 1);
   //  Back
   glTexCoord2d(1, 1);
   glNormal3f( 0, -1, 0);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(+1,-1,-1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(-1,-1,-1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(+1,+1,-1);
   //  Right
   glTexCoord2d(1, 1);
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(+1,-1,+1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(+1,-1,-1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(+1,+1,+1);
   //  Left
   glTexCoord2d(1, 1);
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1,-1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(-1,-1,+1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,+1,-1);
   //  Top
   glTexCoord2d(1, 1);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,+1,+1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(+1,+1,+1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,+1,-1);
   //  Bottom
   glTexCoord2d(1, 1);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,-1,-1);
   glTexCoord2f(1.0f, 0.0f); glVertex3f(+1,-1,-1);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void drawTorus(double r, double c,
               int rSeg, int cSeg, double x_t, double y_t, double z_t,
               double x_r, double y_r, double z_r, double rotate)
{
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[5]);
  glTranslated(x_t,y_t,z_t);
  glRotatef(rotate, x_r, y_r, z_r);
  glFrontFace(GL_CW);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  const double PI = 3.1415926535897932384626433832795;
  const double TAU = 2 * PI;

  for (int i = 0; i < rSeg; i++) {
    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= cSeg; j++) {
      for (int k = 0; k <= 1; k++) {
        double s = (i + k) % rSeg + 0.5;
        double t = j % (cSeg + 1);

        double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
        double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
        double z = r * sin(s * TAU / rSeg);

        double u = (i + k) / (float) rSeg;
        double v = t / (float) cSeg;

        glTexCoord2d(u, v);
        glNormal3f(2 * x, 2 * y, 2 * z);
        glVertex3d(2 * x, 2 * y, 2 * z);
      }
    }
    glEnd();
  }

  glFrontFace(GL_CCW);
  glPopMatrix();
}

static void cylinder(double x, double y, double z, double x_s, double y_s, 
  double z_s, double x_r, double y_r, double z_r, double rotation, int tex)
{
    const double PI = 3.14159;

    double i, resolution  = 0.1;
    double height = 1;
    double radius = 0.5;

    glPushMatrix();
    glTranslated(x, y, z);
    glRotatef(rotation, x_r, y_r, z_r);
    glScaled(x_s, y_s, z_s);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[tex]);

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, height, 0);
        for (i = 2 * PI; i >= 0; i -= resolution)

        {
            glNormal3f(0,0,1);
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(radius * cos(i), height, radius * sin(i));
        }
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(radius, height, 0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, 0, 0);  
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            glNormal3f(0,0,-1);
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
        }
    glEnd();

    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            glNormal3f(Cos(i * 57.29),0,Sin(i * 57.29));
            const float tc = ( i / (float)( 2 * PI ) );
            glTexCoord2f( tc, 0.0 );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
            glTexCoord2f( tc, 1.0 );
            glVertex3f(radius * cos(i), height, radius * sin(i));
        }
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f(radius, 0, 0);
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f(radius, height, 0);
    glEnd();

    glPopMatrix();
}


static void plane(float width, float height, float rotate, float x_r, float y_r, float z_r, float x_t, float y_t, float z_t, int tex, float repeat) {
   glPushMatrix();
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[tex]);
   glColor3f(1,1 ,1);
   glRotatef(rotate, x_r, y_r, z_r);
   glTranslated(x_t, y_t, z_t);
   glBegin(GL_QUADS);

   glNormal3f(0, 1, 0);

   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(-1.0 * width, 0, height);

   glTexCoord2f(repeat, 0.0f);
   glVertex3f( width, 0, height);

   glTexCoord2f(repeat, repeat);  
   glVertex3f( width, 0, -1.0 * height);

   glTexCoord2f(0.0f, repeat);
   glVertex3f(-1.0 * width, 0, -1.0 * height);


glEnd();
glDisable(GL_TEXTURE_2D);
glPopMatrix();
}


//SECTION FOR DRAWING DICE
static void icosahedron(double x, double y, double z, double x_s, double y_s, double z_s) {
   glPushMatrix();
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glTranslated(x,y,z);
   glScaled(x_s, y_s, z_s);
   double X = 1;
   double Z = 1.618; // Golden ratio for platonic solid

   //
   GLfloat vdata[12][3] = {    
   {-X, 0, Z}, {X, 0, Z}, {-X, 0, -Z}, {X, 0, -Z},    
   {0, Z, X}, {0, Z, -X}, {0, -Z, X}, {0, -Z, -X},    
   {Z, X, 0}, {-Z, X, 0}, {Z, -X, 0}, {-Z, -X, 0} 
   };

   for(int i = 0; i < 12; i ++) {
      for (int j = 0; j < 3; j ++) {
         vdata[i][j] = vdata[i][j];
      }
   }

   GLuint tindices[20][3] = { 
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

int i;

glBegin(GL_TRIANGLES);    
for (i = 0; i < 20; i++) {   

      GLfloat v1[3];
      GLfloat v2[3];
      GLfloat v3[3];
      for (int j = 0; j < 3; j ++) {
        v1[j] = vdata[tindices[i][0]][j];
        v2[j] = vdata[tindices[i][1]][j];
        v3[j] = vdata[tindices[i][2]][j];
      }
      GLfloat N[3];
      calculateNormal(v1, v2, v3, N);

   /* color information here */ 
      glNormal3fv(&N[0]);
      glTexCoord2f(0, 0); 
      glVertex3fv(&vdata[tindices[i][2]][0]);

      glTexCoord2f(0, 1);
      glVertex3fv(&vdata[tindices[i][1]][0]); 

      glTexCoord2f(0.5, 1); 
      glVertex3fv(&vdata[tindices[i][0]][0]); 
}
glEnd();

   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

// TODO ZL Reimplement with candle
static void cone(double x, double y, double z, double x_s, double y_s, double z_s, 
  double x_t, double y_t, double z_t, double rotation, int tex) {
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[tex]);
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rotation, x_t, y_t, z_t);
  double twopi = 2 * 3.14;
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 0, 0);
  GLfloat v1[3] = {0, 0, 0};
  for (double i = 0; i <= twopi + twopi/50; i += twopi / 50) {
    GLfloat N[3];
    GLfloat v2[3] = {2*sin(i), -2, 2*cos(i)};
    GLfloat v3[3] = {2*sin(i - (twopi / 50)), -2, 2*cos(i-(twopi/50))};
    glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
    calculateNormal(v1, v2, v3, N);
    glNormal3fv(&N[0]);

    glVertex3f(2 * sin(i), -2, 2 * cos(i));

  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

// TODO ZL Talk to a TA about proper lighting implementation
// static void candle(double x, double y, double z, double x_s, double y_s, double z_s,
//   double x_t, double y_t, double z_t, double rotation) {
//   glPushMatrix();
//   glTranslated(x, y, z);
//   glScaled(x_s, y_s, z_s);
//   glRotatef(rotation, x_t, y_t, z_t);
//   cone(0, 0, 0, .5, .5, .5, -1, 0, 0, 90, 8);
//   cylinder(0, -1, 0, 1, 1, 1, 0, 0, 0, 0, 8);
//   glPopMatrix();

//   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
//   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
//   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
//   //  Light direction
//   float Position[]  = {x, y + 1, z, 1};
//   //  Draw light position as ball (still no lighting here)
//   glColor3f(1,1,1);
//   ball(Position[0],Position[1],Position[2] , 0.1);
//   //  glColor sets ambient and diffuse color materials
//   //  Enable light 0
//   glEnable(GL_LIGHT0);
//   //  Set ambient, diffuse, specular components and position of light 0
//   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
//   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
//   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
//   glLightfv(GL_LIGHT0,GL_POSITION,Position);
//   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);

// }

void drawChairs(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rot, x_r, y_r, z_r);
  //Draw base
  cube(0, 0, 0, .3, .05, .3, 0, 1, 0, 0, 6);
  //Draw legs
  cube(-.25, -.35, -.25, .05, .3, .05, 0, 0, 0, 0, 6);
  cube(-.25, -.35, .25, .05, .3, .05, 0, 0, 0, 0, 6);
  cube(.25, -.35, -.25, .05, .3, .05, 0, 0, 0, 0, 6);
  cube(.25, -.35, .25, .05, .3, .05, 0, 0, 0, 0, 6);
  //Draw back
  cube(.25, .35, -.25, .05, .3, .05, 0, 0, 0, 0, 6);
  cube(.25, .35, .25, .05, .3, .05, 0, 0, 0, 0, 6);

  cube(.25, .5, 0, .05, .05, .2, 0, 0, 0, 0, 6);
  cube(.25, .3, 0, .05, .05, .2, 0, 0, 0, 0, 6);
  cube(.25, .1, 0, .05, .05, .2, 0, 0, 0, 0, 6);

  glPopMatrix();



}

void drawWallChains(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rot, x_r, y_r, z_r);

  drawTorus(.10, .30, 20, 8, -12, 5, 5, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 4.0, 4, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 3.0, 3, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 2.0, 2, 0, 1, 0, 90);

  drawTorus(.10, .30, 20, 8, -12, 5, -3, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 4.0, -2, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 3.0, -1, 0, 1, 0, 90);
  drawTorus(.10, .30, 20, 8, -12, 2.0, 0, 0, 1, 0, 90);

  glPopMatrix();
}

void drawBorders() {
   //Floor
   plane(12, 12, 0, 0, 0, 0, 0, -6, 0, 9, 1);
   //Right Wall
   plane(12, 6, 90, 1, 0, 0, 0, -12, 0, 14, 4);
   //Left Wall
   plane(12, 6, -90, 1, 0, 0, 0, -12, 0, 14, 4);
   //Forward Wall
   plane(6, 12, 90, 0, 0, 1, 0, -12, 0, 14, 4);
   //Backwards Wall
   plane(6, 12, -90, 0, 0, 1, 0, -12, 0, 14, 4);
   //Ceiling
   plane(12, 12, 180, 0, 0, 1, 0, -6, 0, 14, 4);
}

void drawShelf(double x, double y, double z) {
  glPushMatrix();
  glTranslated(x, y, z);
  //SideShelves
   cube(2.5, -1, 11, .3, 5, 1, 0, 0, 0, 0, 6);
   cube(-2.5, -1, 11, .3, 5, 1, 0, 0, 0, 0, 6);
   //BackBoard
   cube(0, -1, 12, 2.8, 5, .1, 0, 0, 0, 0, 6);

   //Shelves
   cube(0, -1, 11, 2.2, .1, 1, 0, 0, 0, 0, 6);
   cube(0, -6, 11, 2.2, .1, 1, 0, 0, 0, 0, 6);
   cube(0, 3.9, 11, 2.2, .1, 1, 0, 0, 0, 0, 6);

   glPopMatrix();

}

void drawMantle(double x, double y, double z, double x_s, double y_s, double z_s) {
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[6]);

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0, 0, 1);
    glVertex3f(2.0, 0, 0);
    glVertex3f(0.8, 1.0, 0);
    glVertex3f(0.8, -1.0, 0);  
    glVertex3f(-0.8, 1.0, 0);
    glVertex3f(-0.8,-1.0, 0);
    glVertex3f(-2.0, 0.0, 0);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0, 0, -1);
    glVertex3f(2.0, 0, -.5);
    glVertex3f(0.8, -1.0, -.5);  
    glVertex3f(0.8, 1.0, -.5);
    glVertex3f(-0.8,-1.0, -.5);
    glVertex3f(-0.8, 1.0, -.5);
    glVertex3f(-2.0, 0.0, -.5);
  glEnd();

  glBegin(GL_QUADS);

  //top
  glNormal3f(0, 1, 0);
  glVertex3f(0.8, 1.0, -.5);
  glVertex3f(-0.8, 1.0, -.5);
  glVertex3f(-0.8, 1.0, 0);
  glVertex3f(0.8, 1.0, 0);

  //bottom
  glNormal3f(0, -1, 0);
  glVertex3f(-0.8, -1.0, -.5);
  glVertex3f(0.8, -1.0, -.5);
  glVertex3f(0.8, -1.0, 0);
  glVertex3f(-0.8, -1.0, 0);


  //upperleft
  glNormal3f(1, 1, 0);
  glVertex3f(2.0, 0, -0.5);
  glVertex3f(0.8, 1.0, -0.5);
  glVertex3f(0.8, 1.0, 0);
  glVertex3f(2.0, 0, 0);


  //lowerleft
  glNormal3f(1, -1, 0);
  glVertex3f(0.8, -1.0, -0.5);
  glVertex3f(2.0, 0, -0.5);
  glVertex3f(2.0, 0, 0);
  glVertex3f(0.8, -1.0, 0);


  //upperright
  glNormal3f(-1, 1, 0);
  glVertex3f(-0.8, 1.0, -0.5);
  glVertex3f(-2.0, 0, -0.5);
  glVertex3f(-2.0, 0, 0);
  glVertex3f(-0.8, 1.0, 0);


  //lowerright
  glNormal3f(-1, -1, 0);
  glVertex3f(-2.0, 0, -0.5);
  glVertex3f(-0.8, -1.0, -0.5);
  glVertex3f(-0.8, -1.0, 0);
  glVertex3f(-2.0, 0, 0);

  glEnd();
  glPopMatrix();

}


void ringArtifact(double x, double y, double z) {
  glPushMatrix();
  glTranslated(x, y, z);
  ball(0, 0, 0, .1);
  drawTorus(.01, .30, 20, 10, 0, 0, 0, 1, 1, 0, zh);
  drawTorus(.01, .30, 20, 10, 0, 0, 0, 0, 1, 1, zh);
  drawTorus(.01, .30, 20, 10, 0, 0, 0, 1, 0, 1, zh);

  glPopMatrix();
}

void drawDoor(double x, double y, double z) {

  glPushMatrix();
  glTranslated(x, y, z);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[6]);

//TODO Ask TA about how to correctly implement this
  // glBegin(GL_TRIANGLE_FAN);
  //     double radius = 2;
  //     glTexCoord2f( 0.5, 0.5 );
  //     glVertex3f(0, 2, 0);  /* center */
  //     for (double i = 3.1415; i >= 0; i -= 3.1415 / 50)
  //     {
  //         glNormal3f(0,0,-1);
  //         glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
  //         // glVertex3f(0, .5 * cos(i), .5 * sin(i));
  //         glVertex3f(radius * cos(i),radius * sin(i) + 2, 0);
  //     }
  // glEnd();

  glBegin(GL_QUADS);

  glNormal3f(0, 0, -1);

  glTexCoord2f(0.0f, 0.0f); glVertex3f(2, -2, 0);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-2, -2, 0);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-2, 2, 0);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(2, 2, 0);


  glEnd();

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

}

void displayPedistool(double x, double y, double z, double x_s, double y_s, 
  double z_s, double x_r, double y_r, double z_r, double rotation) {
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rotation, x_r, y_r, z_r);
  cylinder(0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 10);
  cylinder(0, 2, 0, 1.5, .5, 1.5, 0, 0, 0, 1, 10);
  cylinder(0, 0, 0, 1.25, .5, 1.25, 0, 0, 0, 1, 10);
  glPopMatrix();

}

void drawSwordBlade(int rotated) {
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  if(rotated == 0) {
   glRotatef(180, 1, 0, 0); 
   glTranslated(0, -.1, -.1);
  }


  glBegin(GL_TRIANGLE_STRIP);
  GLfloat N[3];
  float v1[3] = {0, 0, 0};
  float v2[3] = {0, -.2, .05};
  float v3[3] = {-2, .05, .05};

  calculateNormal(v1, v2, v3, N);

  glTexCoord2f(0, 0);
  glNormal3f(-1 * N[0], -1 * N[1], -1 * N[2]);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, -.20, .05);
    glTexCoord2f(.5, 1);
    glVertex3f(-2, .05, .05);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2f(0, 0);
    glVertex3f(0, .1, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-2, .05, .05);
    glTexCoord2f(.5, 1);
    glVertex3f(0, .30, .05);

  glEnd();

  glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-2, .05, .05);
    glTexCoord2f(0.5, 1);
    glVertex3f(0, .05, .05);

    glTexCoord2f(0, 0);
    glVertex3f(0, .1, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, .05, .05);
    glTexCoord2f(.5, 1);
    glVertex3f(-2, .05, .05);
  glEnd();


  glPopMatrix();
}


void drawSword(float x, float y, float z, float x_s, float y_s, float z_s, float x_r, float y_r, float z_r, float rotation) {
  glPushMatrix();
  glTranslated(x, y, z);
  glRotated(rotation, x_r, y_r, z_r);
  glScaled(x_s, y_s, z_s);
  drawSwordBlade(0);
  drawSwordBlade(1);
  cube(0, .05, .05, .05, .3, .05, 0, 0, 0, 0, 6);
  cylinder(1.05, .05, .05, .1, 1, .1, 0, 0, 1, 90, 3);
  ball(1.1, .05, .05, .1);
  glPopMatrix();

}

void drawTable() {
  glPushMatrix();

  cube(2.5, -4.5, 2.5, .3, 1.5, .3, 0, 0, 0, 0, 6);
  cube(-2.5, -4.5, 2.5, .3, 1.5, .3, 0, 0, 0, 0, 6);
  cube(-2.5, -4.5, -2.5, .3, 1.5, .3, 0, 0, 0, 0, 6);
  cube(2.5, -4.5, -2.5, .3, 1.5, .3, 0, 0, 0, 0, 6);

  flatTop(0, -3, 0, 1, 0, 0, 90, 5, 0.2, 6);


  glPopMatrix();

}

void drawTorchClaw(double x, double y, double z, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();
  glTranslated(x, y, z);
  glRotatef(rot, x_r, y_r, z_r);
  cube(0, 1.09, 0, .05, .1, .001, 1, 0, 0, -20, 3);
  cube(0, 1.275, 0, .05, .1, .001, 1, 0, 0, 20, 3); 

  glPopMatrix();
}


void drawTorch(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rot, x_r, y_r, z_r);

  cone(0, 0, 0, .1, .5, .1, 1, 0, 0, 180, 6);
  cylinder(0, 1, 0, .4, .001, .4, 0, 0, 0, 0, 3);

  drawTorchClaw(0, 0, -.225, 0, 0, 0, 0);
  drawTorchClaw(0, 0, .225, 0, 1, 0, 180);

  drawTorchClaw(.217, 0, .10, 0, 1, 0, 240);
  drawTorchClaw(-.217, 0, -.10, 0, 1, 0, 60);

  drawTorchClaw(-0.195, 0, .1125, 0, 1, 0, 120);
  drawTorchClaw(0.195, 0, -.1125, 0, 1, 0, 300);

  drawTorus(.01, .11, 5, 8, 0, 1, 0, 1, 0,0 , 90);

  drawTorus(.01, .05, 5, 8, 0, .5, 0, 1, 0,0 , 90);

  cube(.215, .5, 0, .1, .015, .001, 0, 0, 0, 0, 3); 
  cube(.315, .5, 0, .001, .15, .07, 0, 0, 0, 0, 3);
  glPopMatrix(); 
}

void drawGamePeices(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot, int mode) {
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(x_s, y_s, z_s);
    glRotatef(rot, x_r, y_r, z_r);
    glEnable(GL_TEXTURE_2D);
    switch (mode){
      case 0 :
        glColor3f(0, 0, 1);
        break;
      case 1 :
        glColor3f(1, 0, 0);
        break;
      case 2:
        glColor3f(0, 1, 0);
        break;
      case 3:
        glColor3f(0.85882, 0.439216, 0.576471);
        break;
      default:
        glColor3f(0, 0, 1);
    }
    ball(0, 1.25, 0, .25);
    cylinder(0, 0, 0, 1, .1, 1, 0, 0, 0, 0, 3);
    cone(0, 1.1, 0, .25, .5, .25, 0, 0, 0, 0, 3);
    cylinder(.8, .5, 0, .2, .7, .2, 0, 0, 1, 90, 3);
    cylinder(-.1, .5, 0, .2, .7, .2, 0, 0, 1, 90, 3);
    drawSword(.8, .8, 0, .4, .4, .4, 0, 0, 1, -90);


    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void drawBoardGame() {
  plane(2, 2, 0, 0, 0, 0, 0, -2.7, 0, 12, 8);
  //DRAW ON BOARD PEICES
  //Blue peices
  drawGamePeices(1.3, -2.7, 0, .1, .1, .1, 0, 1, 0, 82, 0);
  drawGamePeices(.2, -2.7, 0, .1, .1, .1, 0, 1, 0, 27, 0);
  drawGamePeices(.98, -2.7, .2, .1, .1, .1, 0, 1, 0, 46, 0);
  drawGamePeices(.42, -2.7, .1, .1, .1, .1, 0, 1, 0, 16, 0);
  drawGamePeices(.05, -2.7, .4, .1, .1, .1, 0, 1, 0, 80, 0);
  drawGamePeices(.3, -2.7, .5, .1, .1, .1, 0, 1, 0, 20, 0);
  drawGamePeices(.2, -2.7, .1, .1, .1, .1, 0, 1, 0, 5, 0);
  drawGamePeices(0, -2.7, 0, .1, .1, .1, 0, 1, 0, 10, 0);
  //Red peices
  drawGamePeices(-.5, -2.7, .42, .1, .1, .1, 0, 1, 0, 20, 1);
  drawGamePeices(-.6, -2.7, -.65, .1, .1, .1, 0, 1, 0, 15, 1);
  drawGamePeices(-.74, -2.7, -.12, .1, .1, .1, 0, 1, 0, 46, 1);
  drawGamePeices(-.73, -2.7, .32, .1, .1, .1, 0, 1, 0, 98, 1);
  drawGamePeices(-.24, -2.7, .42, .1, .1, .1, 0, 1, 0, 124, 1);
  drawGamePeices(-.7, -2.7, .12, .1, .1, .1, 0, 1, 0, 41, 1);
  drawGamePeices(-.9, -2.7, -.23, .1, .1, .1, 0, 1, 0, 162, 1);
  //Green peices
  drawGamePeices(.5, -2.7, -.42, .1, .1, .1, 0, 1, 0, 20, 2);
  drawGamePeices(.6, -2.7, -.65, .1, .1, .1, 0, 1, 0, 15, 2);
  drawGamePeices(.74, -2.7, -.12, .1, .1, .1, 0, 1, 0, 46, 2);
  drawGamePeices(.73, -2.7, -.32, .1, .1, .1, 0, 1, 0, 98, 2);
  drawGamePeices(.24, -2.7, -.42, .1, .1, .1, 0, 1, 0, 124, 2);
  drawGamePeices(.7, -2.7, -.12, .1, .1, .1, 0, 1, 0, 41, 2);
  drawGamePeices(.9, -2.7, -.23, .1, .1, .1, 0, 1, 0, 162, 2);

  //Purple peices
  drawGamePeices(-.5, -2.7, -.42, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.1, -2.7, -1.2, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.6, -2.7, -.8, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.05, -2.7, -1.7, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.38, -2.7, -1.14, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.4, -2.7, -1.8, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.2, -2.7, -1.9, .1, .1, .1, 0, 1, 0, 20, 3);
  drawGamePeices(.6, -2.7, -1.2, .1, .1, .1, 0, 1, 0, 20, 3);

  //DRAW OFF BOARD PEICES
  //Blue
  drawGamePeices(4, -2.7, 0, .1, .1, .1, 1,0, 0, 90, 0);
  drawGamePeices(4, -2.7, .5, .1, .1, .1, 0,1, 0, 10, 0);
  drawGamePeices(4, -2.7, 0, .1, .1, .1, 1,0, 0, 90, 0);

}

void drawSwordMantle(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();

  glTranslated(x, y, z); 
  glRotatef(rot, x_r, y_r, z_r); 
  glScaled(x_s, y_s, z_s);

  drawMantle(0, 0, -11, 1, 1, 1);
  drawMantle(0, 0, -10.8, .8, .8, .8);
  drawSword(.8, .4, -10.7, 1, 1, 1, 0, 0, 1, 35);
  drawSword(-.7, .4, -10.7, 1, 1, 1, 0, 0, 1, 145);

  glPopMatrix();
}


void drawSpikes(double x, double y, double z, double x_s, double y_s, double z_s, double x_r, double y_r, double z_r, double rot) {
  glPushMatrix();

  glTranslated(x, y, z);
  glScaled(x_s, y_s, z_s);
  glRotatef(rot, x_r, y_r, z_r);



  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texture[11]);

  glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, .21);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.7, 0, .21);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.7, 2.4, .21); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 2.4, .21);
  glEnd();


  glBindTexture(GL_TEXTURE_2D, texture[15]);
  glBegin(GL_QUADS);



    //Left Column
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0, 1.8, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(.5, 2, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(.5, 0, 0);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0, 0, .2);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0, 1.8, .2);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0, 1.8, 0);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(.5, 0, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(.5, 2, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(.5, 2, .2);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(.5, 0, .2);

    //UpperLeft Column
    //TODO ZL CALCULATE NORMALS WITH NORMAL FUNCTION
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 1.8, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.5, 2.1, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.3, 2.6, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(.5, 2, 0);

    glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 1.8, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0, 1.8, .2);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.5, 2.1, .2);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.5, 2.1, 0);

    glTexCoord2f(0.0f, 0.0f);glVertex3f(-.3, 2.6, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-.3, 2.6, .2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(.5, 2, .2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(.5, 2, 0);

    //Middle Column
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 2.1, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.2, 2.1, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.4 ,2.6, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.3, 2.6, 0);

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 2.1, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.5, 2.1, .2);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.2 ,2.1, .2);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.2, 2.1, 0);

    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.4, 2.6, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.4, 2.6, .2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-.3, 2.6, .2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-.3, 2.6, 0);

    //Upper Right Column
    //TODO ZL CALCULATE NORMALS WITH NORMAL FUNCTION
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.2, 2.1, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.7, 1.8, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.2, 2., 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.4 ,2.6, 0);

    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.2, 2.1, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.2 ,2.1, .2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.7, 1.8, .2);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.7, 1.8, 0);

    glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.2, 2, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.2, 2, .2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.4, 2.6, .2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.4, 2.6, 0);


    //Right Column
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.7, 0, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.2, 0, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.2, 2, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.7, 1.8, 0);

    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.7, 1.8, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.7, 1.8, .2);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.7, 0, .2);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.7, 0, 0);

    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-2.2, 0, 0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-2.2, 0, .2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.2, 2, .2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.2, 2, 0);


    // glVertex3f()
  glEnd();

  for(double i = -.1; i >= -1.8; i -= 0.25) {
    cylinder(i, .2, .1, .2, .001, .2, 0, 0, 0, 0, 3);
    cylinder(i, .1, .1, .1, 2.2, .1, 0, 0, 0, 1, 3);
    cone(i, 0, .1, .05, .1, .05, 1, 0, 0, 180, 3);
  }

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();


}

void drawGateBorder() {

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=2.0;
   //  Eye position
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   //  Set perspective
   glLoadIdentity();
   cam_x = cos(f_ph) * cos(f_th);
   cam_y = sin(f_ph);
   cam_z = sin(f_th) * cos(f_ph);
   //First person camera shtuff
  gluLookAt(f_x, f_y, f_z, cam_x + f_x, cam_y + f_y, cam_z + f_z , 0,1,0);
   //  Light switch

   if (light)
   {
      //  Translate intensity to color vectors
   //  Set up lighting
  glEnable(GL_LIGHTING);
   float Ambient[]   = {0.2,0.2,0.2,1.0};
   float Diffuse[]   = {0.8,0.8,0.8,1.0};
   float Specular[]  = {1.0,1.0,1.0,1.0};
   float pos[]     = {10 , 2.5, 5 , 1.0};
   float pos2[] = {8, 2.5, -5, 1.0};
   glEnable(GL_NORMALIZE);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHT0);
   ball(pos[0], pos[1], pos[2], .1);
   glLightfv(GL_LIGHT0,GL_AMBIENT , Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE , Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR, Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,pos);

   glEnable(GL_LIGHT1);
   glLightfv(GL_LIGHT1,GL_AMBIENT , Ambient);
   glLightfv(GL_LIGHT1,GL_DIFFUSE , Diffuse);
   glLightfv(GL_LIGHT1,GL_SPECULAR, Specular);
   glLightfv(GL_LIGHT1,GL_POSITION,pos2);
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene
   // icosahedron(.5, 0);
  drawBorders();
  // drawMantle(0, 0, 0, 1, 1, 1);
  drawSwordMantle(0, 2.5, -.25, 1, 1, 1, 0, 1, 0, 90);


  displayPedistool(10, -6, 0, 1.5, 2, 1.5, 0, 0, 0, 0);
  displayPedistool(10, -6, -3, 1.5, 1.75, 1.5, 0, 0, 0, 0);
  displayPedistool(10, -6, 3, 1.5, 1.75, 1.5, 0, 0, 0, 0);

  drawChairs(0, -4, -5.5, 3, 3, 3, 0 ,1 ,0, 120);
  drawChairs(-5.5, -4, 0, 3, 3, 3, 0 ,1 ,0, 180);
  drawChairs(5.5, -4, 0, 3, 3, 3, 0 ,1 ,0, 10);
  drawChairs(0, -4, 5.5, 3, 3, 3, 0 ,1 ,0, 270);

  icosahedron(2, -2.65, 0, .05, .05, .05);

  ringArtifact(10, 0.5, 0);
  ringArtifact(10, -.5, 3);
  ringArtifact(10, -.5, -3);

  drawShelf(8, 0, 0);
  drawShelf(-8, 0, 0);
  drawWallChains(0, -2, -1, 1, 1, 1, 0, 0, 0, 0);

  drawTable();
  // drawDoor(0, 0, 11);
  drawBoardGame();

  drawTorch(10, 0, 5, 2, 2, 2, 0, 0, 0, 0);
  drawTorch(10, 0, -5, 2, 2, 2, 0, 0, 0, 0);
  drawTorch(-10, 0, 5, 2, 2, 2, 0, 1, 0, 180);
  drawTorch(-10, 0, -5, 2, 2, 2, 0, 1, 0, 180);

  drawSpikes(3, -6, 11., 3.5, 4, 4, 0, 0, 0, 0);
  // drawSpikes(0, 0, 0, 1, 1, 1, 0, 0, 0, 0);




  //Draw Torch Particles
  glUpdateParticles(10, 2, 5, Particle1);
  glDrawParticles(Particle1);

  glUpdateParticles(10, 2, -5, Particle2);
  glDrawParticles(Particle2);

  glUpdateParticles(-10, 2, 5, Particle3);
  glDrawParticles(Particle3);

  glUpdateParticles(-10, 2, -5, Particle4);
  glDrawParticles(Particle4);

  glDisable(GL_LIGHTING);
  glColor3f(1,1,1);

   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   // Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   // if (light)
   // {
   //    glWindowPos2i(5,25);
   //    Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   // }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
  if (ch == 27)
      exit(0);
   //  Reset view angle
  else if (ch == '0')
      th = ph = 0;
   //  Toggle texture mode
  else if (ch == 'm' || ch == 'M')
    mode = 1-mode;
  //  Toggle axes
  else if (ch == 'x' || ch == 'X')
    axes = 1-axes;
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
    light = 1-light;
  //  Toggle textures mode
  else if (ch == 't')
    ntex = 1-ntex;
  //  Repitition
  else if (ch=='+')
    rep++;
  else if (ch=='-' && rep>1)
    rep--;

  // Calculate change in x, y, and z then add to respective positions
  // Moves forwards in first person
  else if(ch == 'w'){
    double dx = cos(f_th)*cos(f_ph);
    double dy = sin(f_ph);
    double dz = sin(f_th)*cos(f_ph);

    f_x += 0.3 * dx;
    f_y += 0.3 * dy;
    f_z += 0.3 * dz;
  }
  // Calculate change in x, y, and z then add to respective positions
  // Moves backwards in first person
  else if(ch == 's'){
    double dx = cos(f_th)*cos(f_ph);
    double dy = sin(f_ph);
    double dz = sin(f_th)*cos(f_ph);

    f_x -= 0.3 * dx;
    f_y -= 0.3 * dy;
    f_z -= 0.3 * dz;
  }

  //Rotate First person view to the Left
  else if (ch == 'a') {
    f_th -= 0.3;
  }
  //Rotate First Person view to the right
  else if (ch == 'd') {
    f_th += 0.3;
  }

  //Rotate first person view upwards
  else if (ch == 'q') {
    f_ph += 0.3;
  }

  //Rotate first person view downwards
  else if (ch == 'e') {
    f_ph -= 0.3;
  }
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  srand(time(0));
  qobj = gluNewQuadric();
  gluQuadricNormals(qobj, GLU_SMOOTH);
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Alexander Louie Final");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   texture[0] = LoadTexBMP("crate.bmp");
   texture[1] = LoadTexBMP("grass.bmp");
   texture[2] = LoadTexBMP("rivet_steel_1.bmp");
   texture[3] = LoadTexBMP("metal.bmp");
   texture[4] = LoadTexBMP("wall.bmp");
   texture[5] = LoadTexBMP("chain.bmp");
   texture[6] = LoadTexBMP("wood.bmp");
   texture[7] = LoadTexBMP("dice.bmp");
   texture[8] = LoadTexBMP("candle.bmp");
   texture[9] = LoadTexBMP("floor.bmp");
   texture[10] = LoadTexBMP("marble.bmp");
   texture[11] = LoadTexBMP("stainglass.bmp");
   texture[12] = LoadTexBMP("boardgame.bmp");
   texture[13] = LoadTexBMP("flame.bmp");
   texture[14] = LoadTexBMP("walltwo.bmp");
   texture[15] = LoadTexBMP("brick.bmp");
   glCreateParticles(10, 2, 5, Particle1);
   glCreateParticles(10, 2, -5, Particle2);
   glCreateParticles(-10, 2, 5, Particle3);
   glCreateParticles(-10, 2, -5, Particle4);
   ErrCheck("init");

   glutMainLoop();
   return 0;
}

void cleanup() // call once when you exit program
{
  gluDeleteQuadric(qobj);
}
