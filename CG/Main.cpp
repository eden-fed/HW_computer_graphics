#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>

#include <vector>
#include <Windows.h>
#include <assert.h>
#include <math.h>

#include "Utils.h"
#include "Renderer.h"
#include "Obj Parser/wavefront_obj.h"
#include "Line.h"
#include "MeshModel.h"
#include "Object.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include "BBox.h"
#include "Z_Buffer.h"
#include "Shader.h"
#include "Light.h"


LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;//for the timing 
LARGE_INTEGER Frequency;

#define PI 3.14159265358979323846  
#define OBJ_SPACE 0
#define WORLD_SPACE 1
#define TEST_SPACE(x) ((x)==0 ? (1) : (-1))

unsigned int g_Color = 0xff0000ff;

double g_Swidth = 1366;
double g_Sheight = 768;

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

//global veriables for glut functions
bool g_reset = false;
bool g_centerCam = false;
bool g_showCrdSystem = false;
bool g_normals = false;
bool g_bbox = false;
bool g_projectionType = true;
bool g_space = false;//initialize to object space
double g_normals_size = 1; 

double g_ambient = 0.1;
double g_diffuse = 0.6;
double g_specular = 0.7;
double g_specularExp = 32;
double g_xLightPosition = 0.0;
double g_yLightPosition = 0.0;
double g_zLightPosition = 0.0;
double g_xLightDirection = 0.0;
double g_yLightDirection = 0.0;
double g_zLightDirection = -5;
bool g_lightType = false;
unsigned int g_lightIntensity = 0xffffff;
unsigned int g_ambientLight = 0xffffff;
Light g_light1;
Light g_light2;
Z_Buffer g_zBuffer(g_Swidth, g_Sheight);
bool g_mesh = false;
eShadingType g_shadingType = FLAT;
TwType shadingType;

//red=0xff0000ff

Object sceneObject;
//MeshModel model;
BBox box;
Matrix4x4 transform;
Matrix4x4 axisTransform;
Matrix4x4 saveCamera(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 0, 0, 0, 1);
bool clear = true;

void TW_CALL loadOBJModel(void* clientData);
void TW_CALL applyTranslation(void* clientData);
void TW_CALL applyScale(void* clientData);
void TW_CALL applyXrotation(void* clientData);
void TW_CALL applyYrotation(void* clientData);
void TW_CALL applyZrotation(void* clientData);
void TW_CALL applyLight1(void* clientData);
void TW_CALL applyLight2(void* clientData);


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
	TwDefine(" TweakBar size='200 600' color='96 216 224' "); // change default tweak bar size and color

	//***********************************************************************************
	//add 'g_Scale' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	//TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
	//***********************************************************************************


	TwAddButton(bar, "LoadOBJ", loadOBJModel, NULL, "help='button to load obf file'");
	TwAddSeparator(bar, NULL, NULL);

	TwAddVarRW(bar, "reset", TW_TYPE_BOOLCPP, &g_reset, "help='reset everything'");
	TwAddVarRW(bar, "OW space", TW_TYPE_BOOLCPP, &g_space, " help='true=transforma in world space ,false=transform in object space' ");
	TwAddVarRW(bar, "OW Crd System", TW_TYPE_BOOLCPP, &g_showCrdSystem, " help='boolean variable to indicate if to show WO coordinate system or not.' ");
	TwAddVarRW(bar, "showBbox", TW_TYPE_BOOLCPP, &g_bbox, " help='boolean variable to indicate if to show the bbox or not.' ");

	TwAddVarRW(bar, "showNormals", TW_TYPE_BOOLCPP, &g_normals, " help='boolean variable to indicate if to show normals or not.' group='normals'");
	TwAddVarRW(bar, "normalsSize", TW_TYPE_DOUBLE, &g_normals_size, " min=0.1 max=100 step=0.1 keyIncr=t keyDecr=T help='Change notmals size (20=original size).' group='normals'");
	TwAddVarRW(bar, "projectionType", TW_TYPE_BOOLCPP, &g_projectionType, " help='true = orthographic, false = perspective.' group='camera'");
	TwAddVarRW(bar, "near", TW_TYPE_DOUBLE, &g_near, "step=0.01 keyIncr=n keyDecr=N  group='camera'");
	TwAddVarRW(bar, "far", TW_TYPE_DOUBLE, &g_far, "step=0.1 keyIncr=f keyDecr=F  group='camera'");
	TwAddVarRW(bar, "fovy", TW_TYPE_DOUBLE, &g_fovy, "step=0.1 keyIncr=v keyDecr=V  group='camera'");

	TwAddVarRW(bar, "centerCamera", TW_TYPE_BOOLCPP, &g_centerCam, "help='point the camera to the center of the model'  group='camera'");

	TwAddVarRW(bar, "translate X", TW_TYPE_DOUBLE, &g_translationX, "min=-30 max=30 step=1 keyIncr=right keyDecr=left   group='tranfromations' ");
	TwAddVarRW(bar, "translate Y", TW_TYPE_DOUBLE, &g_translationY, "min=-30 max=30 step=1 keyIncr=up keyDecr=down   group='tranfromations' ");
	TwAddVarRW(bar, "translate Z", TW_TYPE_DOUBLE, &g_translationZ, "min=-30 max=30 step=1 keyIncr=> keyDecr=<   group='tranfromations' ");
	TwAddButton(bar, "apply translation", applyTranslation, NULL, "help='apply translation' group='tranfromations' ");

	TwAddVarRW(bar, "scale", TW_TYPE_DOUBLE, &g_scale, " min=0.01 max=2.5 step=0.01 keyIncr=+ keyDecr=-   group='tranfromations' ");
	TwAddButton(bar, "apply scale", &applyScale, NULL, "help='apply scale' group='tranfromations' ");

	TwAddVarRW(bar, "x-rotation", TW_TYPE_DOUBLE, &g_xRotation, "min = -360 max = 360 step=1 keyIncr=x keyDecr=X   group='tranfromations' ");
	TwAddButton(bar, "apply x rotation", &applyXrotation, NULL, " help='apply scale' group='tranfromations' ");

	TwAddVarRW(bar, "y-rotation", TW_TYPE_DOUBLE, &g_yRotation, "min = -360 max = 360 step=1 keyIncr=y keyDecr=Y   group='tranfromations' ");
	TwAddButton(bar, "apply y rotation", &applyYrotation, NULL, " help='apply scale'  group='tranfromations' ");

	TwAddVarRW(bar, "z-rotation", TW_TYPE_DOUBLE, &g_zRotation, "min = -360 max = 360 step=1 keyIncr=z keyDecr=Z   group='tranfromations' ");
	TwAddButton(bar, "apply z rotation", &applyZrotation, NULL, " help='apply scale' group='tranfromations' ");

	TwAddVarRW(bar, "ambient", TW_TYPE_DOUBLE, &g_ambient, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "diffuse", TW_TYPE_DOUBLE, &g_diffuse, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specular", TW_TYPE_DOUBLE, &g_specular, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specularExp", TW_TYPE_DOUBLE, &g_specularExp, "min = 0 max = 1000 step=1 keyIncr=z keyDecr=Z   group='material' ");

	TwAddVarRW(bar, "x-position", TW_TYPE_DOUBLE, &g_xLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-position", TW_TYPE_DOUBLE, &g_yLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-position", TW_TYPE_DOUBLE, &g_zLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "x-direction", TW_TYPE_DOUBLE, &g_xLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-direction", TW_TYPE_DOUBLE, &g_yLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-direction", TW_TYPE_DOUBLE, &g_zLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "point/directional", TW_TYPE_BOOLCPP, &g_lightType, "help='false=point, true=directional'  group='light'");
	TwAddVarRW(bar, "light intensity", TW_TYPE_COLOR32, &g_lightIntensity, " coloralpha=true colormode=rgb group='light'");
	TwAddButton(bar, "apply on light 1", &applyLight1, NULL, " help='apply scale' group='light' ");
	TwAddButton(bar, "apply on light 2", &applyLight2, NULL, " help='apply scale' group='light' ");
	TwAddVarRW(bar, "ambient light intensity", TW_TYPE_COLOR32, &g_ambientLight, "coloralpha=true colormode=rgb group='light' ");

	shadingType = TwDefineEnum("ShadingType", NULL, 0);
	TwAddVarRW(bar, "shading type", shadingType, &g_shadingType, " enum='0 {FLAT}, 1 {GOURAUD}, 2 {PHONG}' group='shading'");
	TwAddVarRW(bar, "show mesh model", TW_TYPE_BOOLCPP, &g_mesh, "help='false=shading, true=meshmodel'  group='shading'");

	TwAddSeparator(bar, NULL, NULL);

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
		transform.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the model matrix
		axisTransform.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the model matrix
		box.setVertices(m);
		sceneObject.setModel(m, transform);

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

void TW_CALL applyTranslation(void* clientData) {
	if (g_translationX != 0.0 || g_translationY != 0.0 || g_translationZ != 0.0) {
		Matrix4x4 mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_translationX, g_translationY, g_translationZ, 1);
		if (g_space){//world space
			//transform *= mat;
			sceneObject.setMtrx(sceneObject.getMtrx()*mat);
		}
		else {//object space
			//transform = mat*transform;
			sceneObject.setMtrx(mat*sceneObject.getMtrx());
		}
		glutPostRedisplay();
	}
}
void TW_CALL applyScale(void* clientData) {

	if (g_scale != 1.0) {
		Matrix4x4 mat(g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, 1);
		if (g_space) {//world space
			sceneObject.setMtrx(sceneObject.getMtrx()*mat);
		}
		else {//object space
			sceneObject.setMtrx(mat*sceneObject.getMtrx());
		}
		glutPostRedisplay();
	}
}

void TW_CALL applyXrotation(void* clientData) {
	double teta = g_xRotation*PI / 180.0;
	Matrix4x4 mat(1, 0, 0, 0,
		0, cos(teta), sin(teta), 0,
		0, -sin(teta), cos(teta), 0,
		0, 0, 0, 1);
	if (g_xRotation != 0.0) {
		if (!g_space)//object space
		{
			sceneObject.setMtrx(mat*sceneObject.getMtrx());
			axisTransform = mat*axisTransform;
		}
		else {//world space
			sceneObject.setMtrx(sceneObject.getMtrx()*mat);
			axisTransform *= mat;

		}
		glutPostRedisplay();
	}
}
void TW_CALL applyYrotation(void* clientData) {
	double teta = g_yRotation*PI / 180.0;
	Matrix4x4 mat(cos(teta), 0, -sin(teta), 0,
		0, 1, 0, 0,
		sin(teta), 0, cos(teta), 0,
		0, 0, 0, 1);
	if (g_yRotation != 0.0) {
		if (!g_space) {
			sceneObject.setMtrx(mat*sceneObject.getMtrx());
			axisTransform = mat*axisTransform;
		}
		else {
			sceneObject.setMtrx(sceneObject.getMtrx()*mat);
			axisTransform *= mat;
		}
		glutPostRedisplay();
	}
}
void TW_CALL applyZrotation(void* clientData) {
	double teta = g_zRotation*PI / 180.0;
	Matrix4x4 mat(cos(teta), sin(teta), 0, 0,
		-sin(teta), cos(teta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	if (g_zRotation != 0.0) {
		if (!g_space) {
			sceneObject.setMtrx(mat*sceneObject.getMtrx());
			axisTransform = mat*axisTransform;
		}
		else {
			sceneObject.setMtrx(sceneObject.getMtrx()*mat);
			axisTransform *= mat;
		}
		glutPostRedisplay();
	}
}
void TW_CALL applyLight1(void* clientData) {
	Vector4 D(g_xLightDirection, g_yLightDirection, g_zLightDirection, 1);
	Vector4 P(g_xLightPosition, g_yLightPosition, g_zLightPosition, 1);
	g_light1.setDirection(D);
	g_light1.setPosition(P);
	g_light1.setIntensity(g_lightIntensity);
	if(g_lightType)
		g_light1.setType(_DIRECTION);
	else 
		g_light1.setType(_POINT);

	glutPostRedisplay();

}
void TW_CALL applyLight2(void* clientData) {
	Vector4 D(g_xLightDirection, g_yLightDirection, g_zLightDirection, 1);
	Vector4 P(g_xLightPosition, g_yLightPosition, g_zLightPosition, 1);
	g_light2.setDirection(D);
	g_light2.setPosition(P);
	g_light2.setIntensity(g_lightIntensity);
	if (g_lightType)
		g_light2.setType(_DIRECTION);
	else g_light2.setType(_POINT);
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

void drawScene() {
	if (g_reset) {
		sceneObject.getMtrx().setAllValues(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		axisTransform = sceneObject.getMtrx();

		g_ambientLight = 0xffffff;
		g_light1.setDefaults();
		g_light2.setDefaults();

		g_reset = !g_reset;
	}
	//initial position of object
	Matrix4x4 modelMtrx(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, -5, 1);

	//transforming object with all required transformations
	modelMtrx = sceneObject.getMtrx()*modelMtrx;

	//creating a camera object
	Camera cam({ 0, 0, 0, 1 }, (sceneObject.getMshMdl().getCentroid())*modelMtrx, { 0,1,0,1 });
	
	//pointing camera at object
	if (g_centerCam) {
		saveCamera.setAllValues(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
		saveCamera*=cam.getViewMtrx();
		g_centerCam = false;
	}
	modelMtrx *= saveCamera;

	//creating projection matrix and aplying it
	cam.setProjectionMatrix(g_fovy, g_near, g_far, (eProjectionType)g_projectionType, 1);

	//view to screen matrix
	Matrix4x4 v2sMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_Swidth / 2, g_Sheight / 2, 0, 1);

	Matrix4x4 projectionMtrx(cam.getProjectionMtrx()* v2sMatrix);

	//creating a mesh model and apllying all the transformations
	MeshModel model = sceneObject.getMshMdl();
	model.transformMshMdl(modelMtrx);

	if (g_mesh) {
		model.transformMshMdl(projectionMtrx);
		model.drawModelEdges();
	}
	else {
		Vector4 eyePosition(g_Swidth / 2, g_Sheight / 2, 0, 1);
		g_zBuffer.emptyBuffer();
		Shader shader(g_shadingType);
		model.material.setAll(g_ambient, g_diffuse, g_specular, g_specularExp);
		shader.draw(model, g_ambientLight, g_light1, g_light2, g_zBuffer, eyePosition, projectionMtrx);
		g_zBuffer.drawBuffer();
	}

	modelMtrx *= projectionMtrx;

	//show bounding box
	if (g_bbox) {
		BBox box2 = box;
		box2.transformBox(modelMtrx);
		box2.drawBox();
	}
	//show normals
	if (g_normals) {
		model.drawNormals(g_normals_size);
	}
	//show coordinate systems
	if (g_showCrdSystem) {
		model.calcCentroid();
		sceneObject.drawObjectCrdSystem(axisTransform, model.getCentroid(), g_Swidth / 2, g_Sheight / 2);
	}
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

	glClearColor(0, 0, 0, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//time measuring - don't delete
	QueryPerformanceCounter(&StartingTime);

	if (!clear) {
		drawScene();
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

	//resizing the object to fit the screen size
	double scale=((width/ g_Swidth)*(height/ g_Sheight));
	Matrix4x4 mat(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
	sceneObject.setMtrx(sceneObject.getMtrx()*mat);
	g_Swidth = width;
	g_Sheight = height;

	g_zBuffer.reshape(width, height);


	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
	glutPostRedisplay();
}



void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
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


