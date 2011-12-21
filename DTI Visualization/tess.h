#include <Windows.h>
#include <gl/glut.h>

#ifndef CALLBACK 
#define CALLBACK
#endif

GLUtesselator *tessObj = NULL;

void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}

void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte *estring;

	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}

void CALLBACK endCallback(void)
{
	glEnd();
}

void CALLBACK vertexCallback(GLvoid *vertex)
{
	glVertex2dv((GLdouble*)vertex);
}

void CALLBACK combineCallback(GLdouble coords[3], 
							  GLdouble *vertex_data[4],
							  GLfloat weight[4], GLdouble **dataOut )
{
	GLdouble *vertex;
	int i;

	vertex = (GLdouble *) malloc(2 * sizeof(GLdouble));

	vertex[0] = coords[0];
	vertex[1] = coords[1];
	*dataOut = vertex;
}

void initTess (void) 
{
	tessObj = gluNewTess();
	
	gluTessCallback(tessObj, GLU_TESS_VERTEX, 
		(void (__stdcall *) (void)) &glVertex2dv);
	gluTessCallback(tessObj, GLU_TESS_BEGIN, 
		(void (__stdcall *) (void)) &beginCallback);
	gluTessCallback(tessObj, GLU_TESS_END, 
		(void (__stdcall *) (void)) &endCallback);
	gluTessCallback(tessObj, GLU_TESS_ERROR, 
		(void (__stdcall *) (void)) &errorCallback);
	gluTessCallback(tessObj, GLU_TESS_COMBINE, 
		(void (__stdcall *) (void)) &combineCallback);

	gluTessProperty(tessObj, GLU_TESS_WINDING_RULE,
		GLU_TESS_WINDING_NONZERO);
}
