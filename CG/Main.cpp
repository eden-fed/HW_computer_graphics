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


LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;//for the timing 
LARGE_INTEGER Frequency;


/*int g_StartX = 0;
int g_StartY = 0;
int g_EndX = 0;
int g_EndY = 0;
int g_Op = 0;*/
unsigned int g_Color = 0xff0000ff;
bool g_normals = false;
bool g_bbox = false;

double g_quaternion[4] = {0.0, 0.0, 0.0, 1.0};


void TW_CALL loadOBJModel(void* clientData);
//void TW_CALL showNormals(void* clientData);
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
/*  **********gui for hw1*********
	//add 'g_StartX' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "StartX", TW_TYPE_INT32, &g_StartX, " min=0.00 max=1366 step=1 keyIncr=z keyDecr=Z help='Start X position (0.0=original size).' ");

	//add 'g_StartY' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "StartY", TW_TYPE_INT32, &g_StartY, " min=0.00 max=768 step=1 keyIncr=x keyDecr=X help='Start Y position (0.0=original size).' ");

	//add 'g_EndX' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "EndX", TW_TYPE_INT32, &g_EndX, " min=0.00 max=1366 step=1 keyIncr=a keyDecr=A help='End X position (0.0=original size).' ");

	//add 'g_EndY' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "EndY", TW_TYPE_INT32, &g_EndY, " min=0.00 max=768 step=1 keyIncr=s keyDecr=S help='End Y position (0.0=original size).' ");

	//add 'g_EndY' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "Color", TW_TYPE_COLOR32, &g_Color, " coloralpha = true help='RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red' ");

	//add 'g_EndY' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "Operation", TW_TYPE_INT32, &g_Op, " min=0.00 max=2 step=1 keyIncr=z keyDecr=Z help='Operation: 0-reg line, 1-house, 2-star.' ");*/

	TwAddButton(bar, "LoadOBJ",loadOBJModel, NULL, "help='button to load obf file'");
	//TwAddButton(bar, "showNormals", showNormals, NULL, "help='button to indicate if to show normals or not'");
	TwAddVarRW(bar, "showNormals", TW_TYPE_BOOLCPP, &g_normals, " help='boolean variable to indicate if to show normals or not.' ");
	TwAddVarRW(bar, "showBbox", TW_TYPE_BOOLCPP, &g_bbox, " help='boolean variable to indicate if to show the bbox or not.' ");

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

	//store the values in Object, MeshModel...

	if(result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;

}

/*void TW_CALL showNormals(void* clientData)
{
	static bool show = true;
	std::cout << "show=" << show << std::endl;
	show = false;

}*/

//do not change this function unless you really know what you are doing!
void initGraphics(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1366, 768);
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
	if(err != GLEW_OK)
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
void drawScene(){}

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
// 	static int counter = 0;
// 	std::cout << "C: " << counter << std::endl;
// 	counter++;

    glClearColor(0, 0, 0, 1); //background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//time measuring - don't delete
	QueryPerformanceCounter(&StartingTime);

 	drawScene();

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

	//////////////////////////////////////
	///////add your reshape code here/////////////



	//////////////////////////////////////

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


