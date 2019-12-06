#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);

static void flatTop(double x,double y,double z, double x_r, double y_r, double z_r, double rotation, 
  double r,double d, int tex);

static void drawTorus(double r, double c,
               int rSeg, int cSeg, double x_t, double y_t, double z_t,
               double x_r, double y_r, double z_r, double rotate);


static void cylinder(double x, double y, double z, double x_s, double y_s, 
  double z_s, double x_r, double y_r, double z_r, double rotation, int tex);

static void plane(float width, float height, float rotate, float x_r, float y_r, float z_r, float x_t, float y_t, float z_t, int tex, float repeat);

static void icosahedron(double x, double y, double z, double x_s, double y_s, double z_s);

static void cone(double x, double y, double z, double x_s, double y_s, double z_s, 
  double x_t, double y_t, double z_t, double rotation, int tex);

#ifdef __cplusplus
}
#endif

#endif
