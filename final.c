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
unsigned int texture[7]; // Texture names

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
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

void cylinder(double x_r, double y_r, double z_r, 
   double dx, double dy, double dz,
   double R, double G, double B, 
   double radius, double height, double rotate)
{
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();
   glTranslated(x_r,y_r,z_r);
   glScaled(dx,dy,dz);
   glRotatef(rotate, 1, 0, 0);

    double x              = 0.0;
    double y              = 0.0;
    double angle          = 0.0;
    double angle_stepsize = 0.1;
    double PI = 3.14;

    /** Draw the tube */
    glColor3ub(R-40,G-40,B-40);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.5, 0.5);
    angle = 0.0;
        while( angle < 2*3.14 ) {
            const float u = (angle / (float) 2 * 3.14);
            glTexCoord2f(u, 0.0);
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(x, y, 0);
            glVertex3f(x, y , height);
            glTexCoord2f(u, height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f(radius, 0.0, height);
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    glColor3ub(R-40,G-40,B-40);
    glBegin(GL_POLYGON);
    angle = 2 * PI;
        while( angle >= 0 ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glTexCoord2f(0.5 * cos(angle) + 0.5, 0.5 * sin(angle) + 0.5);
            glNormal3f(0, 0, -1);
            glVertex3f(x, y , 0);
            angle = angle - angle_stepsize;
        }
        glVertex3f(radius, 0.0, 0);
    glEnd();

    //top circle
    glColor3ub(R - 20,G - 20,B - 20);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glTexCoord2f(0.5 * cos(angle) + 0.5, 0.5 * sin(angle) + 0.5);
            glNormal3f(0, 0, 1);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


static void plane(float width, float height, float pos) {
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glColor3f(1,1 ,1);
   glBegin(GL_QUADS);

   glNormal3f(0, 1, 0);

   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(-1.0 * width, pos, height);

   glTexCoord2f(1.0f, 0.0f);
   glVertex3f( width, pos, height);

   glTexCoord2f(1.0f, 1.0f);  
   glVertex3f( width, pos, -1.0 * height);

   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(-1.0 * width, pos, -1.0 * height);


glEnd();
glDisable(GL_TEXTURE_2D);
}

static void icosahedron(double scale, double origin) {
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   double X = scale;
   double Z = 1.618 * scale; // Golden ratio for platonic solid

   //
   GLfloat vdata[12][3] = {    
   {-X, 0, Z}, {X, 0, Z}, {-X, 0, -Z}, {X, 0, -Z},    
   {0, Z, X}, {0, Z, -X}, {0, -Z, X}, {0, -Z, -X},    
   {Z, X, 0}, {-Z, X, 0}, {Z, -X, 0}, {-Z, -X, 0} 
   };

   for(int i = 0; i < 12; i ++) {
      for (int j = 0; j < 3; j ++) {
         vdata[i][j] = vdata[i][j] + origin;
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

      GLfloat A[3] = {v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
      GLfloat B[3] = {v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};
      GLfloat N[3] = {
        (A[1] * B[2] - A[2] * B[1]) * -1,
        (A[2] * B[0] - A[0] * B[2]) * -1,
        (A[0] * B[1] - A[1] * B[0]) * - 1
      };

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

  gluLookAt(f_x, f_y, f_z, cam_x + f_x, cam_y + f_y, cam_z + f_z , 0,1,0);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
      // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,{1,1,1,1});
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene
   // icosahedron(.5, 0);
   plane(4, 4, -1);
   icosahedron(.5, -1);
   cylinder(2, 0, -1, .1, .1, .1, 0, 0, 1, 6, 10, 90);
   cylinder(3, 0, -2, .1, .1, .1, 0, 0, 1, 6, 10, 90);
   cylinder(.5, 0, -1, .1, .1, .1, 0, 0, 1, 6, 10, 90);
   // coin(0, 0, 0, 1, 1);
   //  Draw axes - no lighting from here on
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
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
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

    f_x += 0.1 * dx;
    f_y += 0.1 * dy;
    f_z += 0.1 * dz;
  }
  // Calculate change in x, y, and z then add to respective positions
  // Moves backwards in first person
  else if(ch == 's'){
    double dx = cos(f_th)*cos(f_ph);
    double dy = sin(f_ph);
    double dz = sin(f_th)*cos(f_ph);

    f_x -= 0.1 * dx;
    f_y -= 0.1 * dy;
    f_z -= 0.1 * dz;
  }

  //Rotate First person view to the Left
  else if (ch == 'a') {
    f_th -= 0.1;
  }
  //Rotate First Person view to the right
  else if (ch == 'd') {
    f_th += 0.1;
  }

  //Rotate first person view upwards
  else if (ch == 'q') {
    f_ph += 0.1;
  }

  //Rotate first person view downwards
  else if (ch == 'e') {
    f_ph -= 0.1;
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
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Alexander Louie Homework 6");
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
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
