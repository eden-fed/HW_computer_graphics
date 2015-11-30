#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>

#include <vector>
#include <Windows.h>
#include <assert.h>


#include "Utils.h"
#include "Renderer.h"
#include "Obj Parser/wavefront_obj.h"
#include "Line.h"
#include "MeshModel.h"
#include "Object.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include "BBox.h"""


LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;//for the timing 
LARGE_INTEGER Frequency;

#define PI 3.14159265359

/*int g_StartX = 0;
int g_StartY = 0;
int g_EndX = 0;
int g_EndY = 0;
int g_Op = 0;*/
unsigned int g_Color = 0xff0000ff;

int g_Swidth = 1366;
int g_Sheight = 768;

double g_near = 0.01;
double g_far = 10000;
double g_fovy = 60;

double g_translationX = 0.0;
double g_translationY = 0.0;
double g_translationZ = 0.0;
double g_scale = 1.0;
double g_xRotation = 0.0;
double g_yRotation = 0.0;
double g_zRotation = 0.0;

double g_quaternion[4] = { 0.0, 0.0, 0.0, 1.0 };

//global veriables for glut functions
bool g_normals = false;
bool g_bbox = false;
bool g_projection = true;
double g_normals_size = 5.0;

Object sceneObject;
MeshModel model;
BBox box;
bool clear = true;

void TW_CALL loadOBJModel(void* clientData);
void TW_CALL centerCamera(void* clientData);
void TW_CALL applyTranslation(void* clientData);
void TW_CALL applyScale(void* clientData);
void TW_CALL applyXrotation(void* clientData);
void TW_CALL applyYrotation(void* clientData);
void TW_CALL applyZrotation(void* clientData);

void initScene();
void initGraphics(int argc, char *argv[]);
void drawScene();
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);


int main(int argc, char *argv[])
{
	// Initialize openGL, glut, glew
	initGraphics(argc, argv);
	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);

	//initialize the timer frequency
	QueryPerformanceFrequency(&Frequency);

	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	//send 'glutGetModifers' function pointer to AntTweakBar.
	//required because the GLUT key event functions do not report key modifiers states.
	//TwGLUTModifiersFunc(glutGetModifiers);


	atexit(Terminate);  //called after glutMainLoop ends


						// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color

	//***********************************************************************************
	//add 'g_Scale' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	//TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
	//***********************************************************************************


	TwAddButton(bar, "LoadOBJ", loadOBJModel, NULL, "help='button to load obf file'");
	TwAddVarRW(bar, "showNormals", TW_TYPE_BOOLCPP, &g_normals, " help='boolean variable to indicate if to show normals or not.' ");
	TwAddVarRW(bar, "normalsSize", TW_TYPE_DOUBLE, &g_normals_size, " min=0.01 max=100 step=0.01 help='Change notmals size (20=original size).' ");
	TwAddVarRW(bar, "showBbox", TW_TYPE_BOOLCPP, &g_bbox, " help='boolean variable to indicate if to show the bbox or not.' ");
	TwAddVarRW(bar, "projectionType", TW_TYPE_BOOLCPP, &g_projection, " help='true = orthographic, false = perspective.' ");
	TwAddVarRW(bar, "near", TW_TYPE_DOUBLE, &g_near, " keyIncr=z keyDecr=Z .' ");
	TwAddVarRW(bar, "far", TW_TYPE_DOUBLE, &g_far, " keyIncr=z keyDecr=Z .' ");
	TwAddVarRW(bar, "fovy", TW_TYPE_DOUBLE, &g_fovy, " keyIncr=z keyDecr=Z ' ");

	//point the camera to the center of the model 
	TwAddButton(bar, "centerCamera", centerCamera, NULL, "help='point the camera to the center of the model'");

	TwAddVarRW(bar, "translate X", TW_TYPE_DOUBLE, &g_translationX, "min=-15 max=15 keyIncr=z keyDecr=Z .' ");
	TwAddVarRW(bar, "translate Y", TW_TYPE_DOUBLE, &g_translationY, "min=-15 max=15 keyIncr=z keyDecr=Z .' ");
	TwAddVarRW(bar, "translate Z", TW_TYPE_DOUBLE, &g_translationZ, "min=-15 max=15 keyIncr=z keyDecr=Z .' ");
	TwAddButton(bar, "apply translation", applyTranslation, NULL, "help='apply translation'");

	TwAddVarRW(bar, "scale", TW_TYPE_DOUBLE, &g_scale, " min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z .' ");
	TwAddButton(bar, "apply scale", &applyScale, NULL, "help='apply scale'");

	TwAddVarRW(bar, "x-rotation", TW_TYPE_DOUBLE, &g_xRotation, "min = -360 max = 360 step=1 keyIncr=z keyDecr=Z .' ");
	TwAddButton(bar, "apply x rotation", &applyXrotation, NULL, " help='apply scale'");

	TwAddVarRW(bar, "y-rotation", TW_TYPE_DOUBLE, &g_yRotation, "min = -360 max = 360 step=1 keyIncr=z keyDecr=Z .' ");
	TwAddButton(bar, "apply y rotation", &applyYrotation, NULL, " help='apply scale'");

	TwAddVarRW(bar, "z-rotation", TW_TYPE_DOUBLE, &g_zRotation, "min = -360 max = 360 step=1 keyIncr=z keyDecr=Z .' ");
	TwAddButton(bar, "apply z rotation", &applyZrotation, NULL, " help='apply scale'");

	//time display - don't delete
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}


void TW_CALL loadOBJModel(void *data)
{
	std::wstring str = getOpenFileName();

	Wavefront_obj objScene;
	bool result = objScene.load_file(str);



	if (result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
		//store the values in Object, MeshModel...
		//draw the object for the first time
		MeshModel m(objScene);
		model = m;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
	clear = false;
	glutPostRedisplay();
}

void TW_CALL centerCamera(void* clientData) {
	//code for centering the camera

}
void TW_CALL applyTranslation(void* clientData) {
	if (g_translationX != 0.0 || g_translationY != 0.0 || g_translationZ != 0.0) {
		Matrix4x4 mat = Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_translationX, g_translationY, g_translationZ, 1);
		//sceneObject.getMshMdl().transformMshMdl(mat);
		model.transformMshMdl(mat);
	}
	glutPostRedisplay();
}
void TW_CALL applyScale(void* clientData) {

	if (g_scale != 1.0) {
		Matrix4x4 mat = Matrix4x4(g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, 1);
		//sceneObject.getMshMdl().transformMshMdl(mat);
		model.transformMshMdl(mat);
	}
	glutPostRedisplay();
}
/*void TW_CALL applyXrotation(void* clientData) {
	if (g_xRotation != 0.0) {
		double teta = g_xRotation*PI / 180.0;
		Vector4 center=model.getCentroid();
		Matrix4x4 moveToCenter= Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -center[0], -center[1], -center[2], 1);
		Matrix4x4 moveFromCenter = Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, center[0], center[1], center[2], 1);
		Matrix4x4 mat = Matrix4x4(1, 0, 0, 0,
							      0, cos(teta), sin(teta), 0,
								  0, -sin(teta), cos(teta), 0,
								  0, 0, 0, 1);
		model.transformNormals(mat);
		mat = moveToCenter*mat;
		mat = mat*moveFromCenter;
		model.transformMshMdl(mat);
	}
	glutPostRedisplay();
}*/
void TW_CALL applyXrotation(void* clientData) {
	if (g_xRotation != 0.0) {
		double teta = g_xRotation*PI / 180.0;
		Matrix4x4 mat = Matrix4x4(1, 0, 0, 0,
								  0, cos(teta), sin(teta), 0,
								  0, -sin(teta), cos(teta), 0,
							  	  0, 0, 0, 1);
		model.transformMshMdl(mat);
		model.transformNormals(mat);
	}
	glutPostRedisplay();
}
void TW_CALL applyYrotation(void* clientData) {
	if (g_yRotation != 0.0) {
		double teta = g_yRotation*PI / 180.0;
		Matrix4x4 mat = Matrix4x4(cos(teta), 0, -sin(teta), 0,
							  	  0, 1, 0 ,0,
								  sin(teta),0, cos(teta), 0,
								  0, 0, 0, 1);
		model.transformMshMdl(mat);
		model.transformNormals(mat);
	}
	glutPostRedisplay();
}
void TW_CALL applyZrotation(void* clientData) {
	if (g_zRotation != 0.0) {
		double teta = g_zRotation*PI / 180.0;
		Matrix4x4 mat = Matrix4x4(cos(teta), sin(teta), 0, 0,
								  -sin(teta), cos(teta), 0, 0,
								  0, 0, 1, 0,
								  0, 0, 0, 1);
		model.transformMshMdl(mat);
		model.transformNormals(mat);
	}
	glutPostRedisplay();
}
//do not change this function unless you really know what you are doing!
void initGraphics(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_Swidth, g_Sheight);
	glutCreateWindow("Computer Graphics Skeleton using AntTweakBar and free GLUT");
	glutCreateMenu(NULL);

	// Initialize OpenGL
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		assert(0);
		return;
	}
}


/*drawScene of hw1
void drawScene()
{
Coordinate startCrd(g_StartX, g_StartY, g_Color);
Coordinate endCrd(g_EndX, g_EndY, g_Color);
Line ln(startCrd, endCrd);

switch (g_Op) {
case 0:
ln.drawline();
break;
case 1:
ln.setStartCrd(300, 200, g_Color);
ln.setEndCrd(600, 200, g_Color);
ln.drawline();
ln.setStartCrd(600, 200, g_Color);
ln.setEndCrd(600, 400, g_Color);
ln.drawline();
ln.setStartCrd(600, 400, g_Color);
ln.setEndCrd(300, 400, g_Color);
ln.drawline();
ln.setStartCrd(300, 400, g_Color);
ln.setEndCrd(300, 200, g_Color);
ln.drawline();
ln.setStartCrd(300, 400, g_Color);
ln.setEndCrd(450, 500, g_Color);
ln.drawline();
ln.setStartCrd(600, 400, g_Color);
ln.setEndCrd(450, 500, g_Color);
ln.drawline();
break;
case 2:
ln.setStartCrd(300, 200, g_Color);
ln.setEndCrd(450, 500, g_Color);
ln.drawline();
ln.setStartCrd(450, 500, g_Color);
ln.setEndCrd(600, 200, g_Color);
ln.drawline();
ln.setStartCrd(600, 200, g_Color);
ln.setEndCrd(300, 200, g_Color);
ln.drawline();
ln.setStartCrd(300, 400, g_Color);
ln.setEndCrd(600, 400, g_Color);
ln.drawline();
ln.setStartCrd(600, 400, g_Color);
ln.setEndCrd(450, 100, g_Color);
ln.drawline();
ln.setStartCrd(450, 100, g_Color);
ln.setEndCrd(300, 400, g_Color);
ln.drawline();

break;
}

}*/
void drawScene() {
	//draw the scene with new values
}

//this will make sure that integer coordinates are mapped exactly to corresponding pixels on screen
void glUseScreenCoordinates(int width, int height)
{
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Callback function called by GLUT to render screen
void Display()
{
	/*static int counter = 0;
	std::cout << "C: " << counter << std::endl;
	counter++;*/

	glClearColor(0, 0, 0, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//time measuring - don't delete
	QueryPerformanceCounter(&StartingTime);

	if (!clear) {
		//drawScene();
		Object sceneObject(model, { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
#define test
#ifdef test
		Camera cam;
		sceneObject.getMshMdl().transformMshMdl(cam.getViewMtrx());
		sceneObject.getMshMdl().transformMshMdl(cam.getPerspectiveMtrx());
		Matrix4x4 matTest(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1366 / 2, 768 / 2, 0, 1);
		sceneObject.getMshMdl().transformMshMdl(matTest);

		//sceneObject.drawObject();
		if (g_bbox) {
			box.setVertices(sceneObject.getMshMdl());
			box.drawBox();
		}
		if (g_normals) {
			sceneObject.drawNormals(g_normals_size);
		}
		sceneObject.drawObject();
		//sceneObject.drawObjectTriangles();
#endif

	}

	//time measuring - don't delete
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	// Draw tweak bars
	TwDraw();

	//swap back and front frame buffers
	glutSwapBuffers();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	glUseScreenCoordinates(width, height);

	g_Swidth = width;
	g_Sheight = height;

	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
	glutPostRedisplay();
}



void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	//glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	//glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
}


void Keyboard(unsigned char k, int x, int y)
{
	TwEventKeyboardGLUT(k, x, y);
	glutPostRedisplay();
}


void Special(int k, int x, int y)
{
	TwEventSpecialGLUT(k, x, y);
	glutPostRedisplay();
}


// Function called at exit
void Terminate(void)
{
	TwTerminate();
}


