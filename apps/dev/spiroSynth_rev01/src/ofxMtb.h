#ifndef OFX_MTB_H
#define OFX_MTB_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "ofMain.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>


#include "ofConstants.h"
#include "ofTypes.h"


void ofxMtbGetProject(double objX, double objY, double objZ,  double * winX, double * winY, double * winZ);

static void circleTable(double **sint,double **cost,const int n);
void ofxMtbDotSphere(GLdouble radius, GLint slices, GLint stacks);
void ofxMtbAnimatePoint(ofPoint *point, ofPoint * target, ofPoint *result, int frame);
void ofxMtbLineSphere(GLdouble radius, GLint slices, GLint stacks, float length);

void ofxMtbDrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);

bool ofKeyAlt();
bool ofKeyShift();
bool ofKeyControl();

void setOrthographicProjection(int _width, int _height);
void mtbSetupScreen();
void mtbDrawBitmapString( char *string, void *font, GLfloat x, GLfloat y );
void mtbDrawBitmapChar( char c, void *font, GLfloat x, GLfloat y );

void RGB2HSV( double *H, double *S, double *V, double R, double G, double B );
double max_color( double r, double g, double b );
double min_color( double r, double g, double b );

#endif