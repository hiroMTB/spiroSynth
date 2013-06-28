#include "ofxMtb.h"
//#include "math.h"

#ifdef TARGET_WIN32
#define GLUT_BUILDING_LIB
#include "glut.h"
#endif
#ifdef TARGET_OSX
#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_LINUX
#include <GL/glut.h>
#endif

static void circleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*M_PI/(double)n;

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        //_glut_fatal("Failed to allocate memory in circleTable");
    }

    /* Compute cos and sin around the circle */

    for (i=0; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}


void ofxMtbGetProject(double objX, double objY, double objZ,  double * winX, double * winY, double * winZ)
{
	
	// get model view matrix
	double modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	
	//get projection matrix
	double projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	// get view port
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// calc
	gluProject(objX,objY,objZ,modelview,projection,viewport, winX, winY,winZ);
}



/*
 *	Draws a solid sphere
 */
void ofxMtbDotSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks and slices are drawn. */

    double r;
    double x,y,z;

    /* Pre-computed circle */
        
    double *sint1,*cost1;
    double *sint2,*cost2;
    circleTable(&sint1,&cost1,-slices  );
    circleTable(&sint2,&cost2, stacks*2);

    /* Draw a line loop for each stack */

    for (i=1; i<stacks; i++)
    {
        z = cost2[i];
        r = sint2[i];

        //glBegin(GL_LINE_LOOP);
		glBegin(GL_POINTS);

            for(j=0; j<=slices; j++)
            {
                x = cost1[j];
                y = sint1[j];

                glNormal3d(x,y,z);
                glVertex3d(x*r*radius,y*r*radius,z*radius);
            }

        glEnd();
    }

    /* Draw a line loop for each slice */

    for (i=0; i<slices; i++)
    {
        //glBegin(GL_LINE_STRIP);
		glBegin(GL_POINTS);

            for(j=0; j<=stacks; j++)
            {
                x = cost1[i]*sint2[j];
                y = sint1[i]*sint2[j];
                z = cost2[j];

                glNormal3d(x,y,z);
                glVertex3d(x*radius,y*radius,z*radius);
            }

        glEnd();
    }

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}



/*
 *  Draws a solid sphere
 */
void ofxMtbLineSphere(GLdouble radius, GLint slices, GLint stacks, float lengthRate)
{
    int i,j;
	float rate = lengthRate;

    /* Adjust z and radius as stacks and slices are drawn. */

    double r;
    double x,y,z;

    /* Pre-computed circle */        
    double *sint1,*cost1;
    double *sint2,*cost2;
    circleTable(&sint1,&cost1,-slices  );
    circleTable(&sint2,&cost2, stacks*2);

    /* Draw a line loop for each stack */

    for (i=1; i<stacks; i++)
    {
        z = cost2[i];
        r = sint2[i];

        glBegin(GL_LINES);
		//glBegin(GL_POINTS);

            for(j=0; j<=slices; j++)
            {
                x = cost1[j];
                y = sint1[j];

                //glNormal3d(x,y,z);
                glVertex3d(x*r*radius,y*r*radius,z*radius);
				glVertex3d(x*r*radius*rate, y*r*radius*rate,z*radius*rate);
				//glVertex3d(0,0,0);
            }

        glEnd();
    }

    /* Draw a line loop for each slice */

    for (i=0; i<slices; i++)
    {
        glBegin(GL_LINES);
		//glBegin(GL_POINTS);

            for(j=0; j<=stacks; j++)
            {
                x = cost1[i]*sint2[j];
                y = sint1[i]*sint2[j];
                z = cost2[j];

                glNormal3d(x,y,z);
                glVertex3d(x*radius,y*radius,z*radius);
				glVertex3d(x*radius*rate, y*radius*rate,z*radius*rate);
				//glVertex3d(0,0,0);
            }

        glEnd();
    }

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}

/** /
static void __gluMultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4],
		      GLdouble out[4])
{
    int i;

    for (i=0; i<4; i++) {
	out[i] = 
	    in[0] * matrix[0*4+i] +
	    in[1] * matrix[1*4+i] +
	    in[2] * matrix[2*4+i] +
	    in[3] * matrix[3*4+i];
    }
}
/**/

/*
bool gluProject(GLdouble objx, GLdouble objy, GLdouble objz, 
	      const GLdouble modelMatrix[16], 
	      const GLdouble projMatrix[16],
              const GLint viewport[4],
	      GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
    double in[4];
    double out[4];

    in[0]=objx;
    in[1]=objy;
    in[2]=objz;
    in[3]=1.0;
    __gluMultMatrixVecd(modelMatrix, in, out);
    __gluMultMatrixVecd(projMatrix, out, in);
    if (in[3] == 0.0) return(GL_FALSE);
    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];
    // Map x, y and z to range 0-1
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;

    // Map x,y to viewport
    in[0] = in[0] * viewport[2] + viewport[0];
    in[1] = in[1] * viewport[3] + viewport[1];

    *winx=in[0];
    *winy=in[1];
    *winz=in[2];
    return(GL_TRUE);
}
*/

/*
bool gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
		const GLdouble modelMatrix[16], 
		const GLdouble projMatrix[16],
        const GLint viewport[4],
	    GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
    double finalMatrix[16];
    double in[4];
    double out[4];

    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    //Map x and y from window coordinates
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    // Map to range -1 to 1
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    return(GL_TRUE);
}
*/

/*
bool gluUnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
	      const GLdouble modelMatrix[16], 
	      const GLdouble projMatrix[16],
	      const GLint viewport[4],
	      GLclampd nearVal, GLclampd farVal,		    
	      GLdouble *objx, GLdouble *objy, GLdouble *objz,
	      GLdouble *objw)
{
    double finalMatrix[16];
    double in[4];
    double out[4];

    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=clipw;

    //Map x and y from window coordinates
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];
    in[2] = (in[2] - nearVal) / (farVal - nearVal);

    // Map to range -1 to 1
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    *objw = out[3];
    return(GL_TRUE);
}
*/


//
//	draw arc function. founded in http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=260223
//
void ofxMtbDrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments) 
{

	//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
	float theta = arc_angle / float(num_segments - 1);

	float tangetial_factor = tanf(theta);

	float radial_factor = cosf(theta);

	//we now start at the start angle
	float x = r * cosf(start_angle);
	float y = r * sinf(start_angle); 

	//since the arc is not a closed curve, this is a strip now
	glBegin(GL_LINE_STRIP);
	for(int ii = 0; ii < num_segments; ii++)
	{ 
		glVertex2f(x + cx, y + cy);

		float tx = -y; 
		float ty = x; 

		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 

		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}



//
// these key detection is from http://www.openframeworks.cc/forum/viewtopic.php?f=6&t=927
//
bool ofKeyAlt() {
    return (glutGetModifiers() & GLUT_ACTIVE_ALT);
}

bool ofKeyShift() {
    return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

bool ofKeyControl() {
    return (glutGetModifiers() & GLUT_ACTIVE_CTRL);
}


// from Bullet, DemoApplication.cpp
void setOrthographicProjection(int _width, int _height)
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, _width, 0, _height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, - _height, 0);
}



void mtbSetupScreen()
{
	int w, h;

	w = ofGetWidth();
	h = ofGetHeight();

	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;

	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);

	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
}

//
// from http://www.wakayama-u.ac.jp/~wuhy/GSS/font-bitmap.c
//
// use following keyword for param font
// GLUT_BITMAP_8_BY_13			8×13 pixel の固定幅フォント
// GLUT_BITMAP_9_BY_15			9×15 pixel の固定幅フォント
// GLUT_BITMAP_TIMES_ROMAN_10	10ポイントの Times Roman フォント
// GLUT_BITMAP_TIMES_ROMAN_24	24ポイントの Times Roman フォント
// GLUT_BITMAP_HELVETICA_10		10ポイントの Helvetica フォント
// GLUT_BITMAP_HELVETICA_12		12ポイントの Helvetica フォント
// GLUT_BITMAP_HELVETICA_18		18ポイントの Helvetica フォント
//
void mtbDrawBitmapString( char *string, void *font, GLfloat x, GLfloat y )
{
  /* 現在のラスターポジションの保存 */
  glPushAttrib(GL_CURRENT_BIT);
    
  glRasterPos2f(x, y);

  /* ビットマップ文字列の描画 */
  while (*string) 
    glutBitmapCharacter(font, *string++);

  /* 保存したラスターポジションのロード */
  glPopAttrib();

}

void mtbDrawBitmapChar( char c, void *font, GLfloat x, GLfloat y )
{
  /* 現在のラスターポジションの保存 */
  glPushAttrib(GL_CURRENT_BIT);
    
  glRasterPos2f(x, y);

  /* ビットマップ文字列の描画 */
  glutBitmapCharacter(font, c);

  /* 保存したラスターポジションのロード */
  glPopAttrib();

}

void RGB2HSV( double *H, double *S, double *V, double R, double G, double B )
{
    double Z;
    double r,g,b;
    *V = max_color( R, G, B );
    Z  = min_color( R, G, B );
    if( *V != 0.0 )
        *S = ( *V - Z ) / *V;
    else
        *S = 0.0;
    if( ( *V - Z ) != 0 ){
        r = ( *V - R ) / ( *V - Z );
        g = ( *V - G ) / ( *V - Z );
        b = ( *V - B ) / ( *V - Z );
    }
    else{
        r = 0;
        g = 0;
        b = 0;
    }
    if( *V == R )
        *H = 60 * ( b - g );		// 60 = PI/3
    else if( *V == G )
        *H = 60 * ( 2 + r - b );
    else
        *H = 60 * ( 4 + g - r );
    if( *H < 0.0 )
        *H = *H + 360;
}

double max_color( double r, double g, double b )
{
    double ret;
    if( r > g ){
        if( r > b )
            ret = r;
        else
            ret = b;
    }
    else{
        if( g > b )
            ret = g;
        else
            ret = b;
    }
    return ret;
}
double min_color( double r, double g, double b )
{
    double ret;
    if( r < g ){
        if( r < b )
            ret = r;
        else
            ret = b;
    }
    else{
        if( g < b )
            ret = g;
        else
            ret = b;
    }
    return ret;
}