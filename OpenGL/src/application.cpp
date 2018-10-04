#define GLM_ENABLE_EXPERIMENTAL
#define arrayLength(x) sizeof(x)/sizeof(float)
#define degToRad(x) glm::radians(x)
#define radToDeg(x) glm::degrees(x)
#define ROTATION_SPEED 30.0f
#define WINDOW_HEIGHT 1500
#define WINDOW_WIDTH 3000
#define MAGNIFICATION 25.0f
#define toPixel(x) x*MAGNIFICATION/WINDOW_WIDTH
#define DIVISIONSIZE 0.5f
#define BYTEPERVAL 12
#define NUMOFVALS 12
#define BUFLEN 144
#define UDPPORT 10002
#define IPADDRESS "0.0.0.0"
#define NUMOFQVALS 5
#define NUMVERTICESPERBOUND 74
#define NUMBOUND 45

#include <glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtx/projection.hpp>
#include <iostream>
#include <Primitives/Vertex.h>
#include <Primitives/ShapeGenerator.h>
#include <arm.h>
#include <Camera.h>
#include <WS2tcpip.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")


int MODE = 0; //0: split screen, 1: single view, 2: Replay
int LBound = 1;
int RBound = 0;


using glm::vec3;
using glm::mat4;

//Static Globals
GLuint programID;
GLuint numIndices;
int boundariesDrawn = 0;

float workspaceRadius[NUMBOUND+1] = { 0.0000,0.0106,0.0414,0.0932,
							  0.1665,0.2619,0.3737,0.5111,
							  0.6698,0.8535,1.0489,1.2709,
							  1.5314,1.7859,2.0799,2.3926,
							  2.7299,3.1100,3.4915,3.8981,
							  4.3468,4.8231,5.3229,5.8314,
							  6.3914,6.9959,7.6254,8.2797,
							  8.9699,9.7149,10.4737,11.2767,
							  12.1517,13.0477,14.0197,15.0569,
							  16.1613,17.2814,18.5329,19.8498,
							  21.2745,22.8617,24.4966,26.3730,
							  28.4652,30.8547};

//UDP data
double packet[NUMOFVALS];

//Camera
Camera camera;
float camX = 0;
float camY = 0;

//Buffer IDs
GLuint outerTubeVertexBufferID;
GLuint outerTubeIndexBufferID;
GLuint innerTubeVertexBufferID;
GLuint innerTubeIndexBufferID;
GLuint midTubeVertexBufferID;
GLuint midTubeIndexBufferID;
GLuint trocarBaseVertexBufferID;
GLuint trocarBaseIndexBufferID;
GLuint trocarHoleVertexBufferID;
GLuint trocarHoleIndexBufferID;
GLuint xVertexBufferID;
GLuint xIndexBufferID;
GLuint yVertexBufferID;
GLuint yIndexBufferID;
GLuint zVertexBufferID;
GLuint zIndexBufferID;
GLuint triangleVertexBufferID;
GLuint triangleIndexBufferID;
GLuint boundaryVertexBufferIDs[NUMBOUND];
GLuint boundaryIndexBufferIDs[NUMBOUND];
GLuint RboundaryVertexBufferIDs[NUMBOUND];
GLuint RboundaryIndexBufferIDs[NUMBOUND];

//Transfomration Matrices
GLuint fullTransformationUniformLocation;
mat4 fullTransformMatrix;
mat4 viewToProjectionMatrix;
mat4 worldToViewMatrix;
mat4 worldToProjectionMatrix;


//Copy memory to GPU
void sendDataToOpenGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//outer
	ShapeData shape = ShapeGenerator::makeCylinder(toPixel(2.3114f / 2.0f), toPixel(DIVISIONSIZE), 0.5f, 0.5f, 0.5f, 1.0f);
	glGenBuffers(1, &outerTubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, outerTubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);
	glGenBuffers(1, &outerTubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerTubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_DYNAMIC_DRAW);
	numIndices = shape.numIndices;
	shape.cleanup();

	//middle
	shape = ShapeGenerator::makeCylinder(toPixel(1.8542f / 2.0f), toPixel(DIVISIONSIZE), 0.75f, 0.75f, 0.75f, 1.0f);
	glGenBuffers(1, &midTubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, midTubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &midTubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, midTubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_DYNAMIC_DRAW);
	shape.cleanup();

	//inner
	shape = ShapeGenerator::makeCylinder(toPixel(1.4478f / 2.0f), toPixel(DIVISIONSIZE), 1.0f, 1.0f, 1.0f, 1.0f);
	glGenBuffers(1, &innerTubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, innerTubeVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &innerTubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, innerTubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_DYNAMIC_DRAW);
	shape.cleanup();

	//trocar base
	shape = ShapeGenerator::makeTrocar(toPixel(9.5f / 2), toPixel(30.0f), 0.25f, 0.25f, 0.25f, 1.0f);
	glGenBuffers(1, &trocarBaseVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, trocarBaseVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &trocarBaseIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trocarBaseIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//trocar holes
	shape = ShapeGenerator::makeTrocar(toPixel(2.55f / 2.0f), toPixel(20.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	glGenBuffers(1, &trocarHoleVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, trocarHoleVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &trocarHoleIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trocarHoleIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//coordinate System

	//x 
	shape = ShapeGenerator::makeArrowCylinder(1.0f, 0.35f, 0.35f, 0.5f);
	glGenBuffers(1, &xVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, xVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &xIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, xIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//y
	shape = ShapeGenerator::makeArrowCylinder(0.35f, 1.0f, 0.35f, 0.5f);
	glGenBuffers(1, &yVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, yVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &yIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, yIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//z
	shape = ShapeGenerator::makeArrowCylinder(0.35f, 0.35f, 1.0f, 0.5f);
	glGenBuffers(1, &zVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, zVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &zIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, zIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//Tracing Shape
	shape = ShapeGenerator::makeCircle(toPixel(10.0f));
	glGenBuffers(1, &triangleVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &triangleIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	shape.cleanup();

	//Workspace boundaries
	//left
	for (int i = 0; i < NUMBOUND; i++)
	{
		shape = ShapeGenerator::makeBoundary(toPixel(workspaceRadius[i+1]), toPixel(workspaceRadius[i]), toPixel(0.125), 0.15f, 0.15f, 1.0f, 0.35f);
		glGenBuffers(1, &(boundaryVertexBufferIDs[i]));
		glBindBuffer(GL_ARRAY_BUFFER, boundaryVertexBufferIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &(boundaryIndexBufferIDs[i]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundaryIndexBufferIDs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
		shape.cleanup();
	}

	//right 
	for (int i = 0; i < NUMBOUND; i++)
	{
		shape = ShapeGenerator::makeBoundary(toPixel(workspaceRadius[i + 1]), toPixel(workspaceRadius[i]), toPixel(0.125), 1.0f, 0.15f, 0.15f, 0.65f);
		glGenBuffers(1, &(RboundaryVertexBufferIDs[i]));
		glBindBuffer(GL_ARRAY_BUFFER, RboundaryVertexBufferIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &(RboundaryIndexBufferIDs[i]));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RboundaryIndexBufferIDs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
		shape.cleanup();
	}

}

//Changing Views
void adjustCamera()
{
	if (GetAsyncKeyState('A') & 0x8000)
		camera.strafeLeft();
	if (GetAsyncKeyState('S') & 0x8000)
		camera.moveBack();
	if (GetAsyncKeyState('D') & 0x8000)
		camera.strafeRight();
	if (GetAsyncKeyState('W') & 0x8000)
		camera.moveForward();
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		camera.moveUp();
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		camera.moveDown();
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		camX++;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		camX--;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		camY--;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		camY++;
	if (GetAsyncKeyState('P') & 0x8000)
	{
		if (MODE == 0)
		{
			MODE = 1;
			Sleep(150);
		}
		else if (MODE == 1)
		{
			MODE = 0;
			Sleep(100);
			//system("pause");
		}
		else if (MODE == 2)
		{
			MODE = 0;
			Sleep(150);
		}
	}
	if (GetAsyncKeyState('I') & 0x8000)
	{
		if (LBound == 0)
		{
			LBound = 1;
			Sleep(150);
		}
		else if (LBound == 1)
		{
			LBound = 0;
			Sleep(150);
		}
	}
	if (GetAsyncKeyState('O') & 0x8000)
	{
		if (RBound == 0)
		{
			RBound = 1;
			Sleep(150);
		}
		else if (RBound == 1)
		{
			RBound = 0;
			Sleep(150);
		}
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		camX = 0;
		camY = 0;
		camera.setPosition(vec3(0.0f, 0.375f, 0.2f));
	}
	camera.mouseUpdate(glm::vec2(camX / ROTATION_SPEED, camY / ROTATION_SPEED));
}

void worldToProjection()
{
	fullTransformMatrix;
	viewToProjectionMatrix = glm::perspective(degToRad(60.0f), ((float)WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 20.0f);
	worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
}

//Shader Initializers
bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC getObjectProperty, PFNGLGETSHADERINFOLOGPROC getInfoLog, GLenum statusType)
{
	GLint status;
	getObjectProperty(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		getObjectProperty(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLog(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout << "File failed to load" << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShader()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];

	std::string temp = readShaderCode("VertexShaderCode.sh");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	temp = readShaderCode("FragmentShaderCode.sh");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) { return; }

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
	{
		return;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glUseProgram(programID);
}

//binding buffers
void bindBuffer(GLuint vertex, GLuint index)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (char*)(sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

//Vector Angle 
vec3 centerVec(vec3 point1, vec3 point2)
{
	vec3 result = point2 - point1;
	return result;
}

vec3 centerDirectionVec(vec3 point1, vec3 point2)
{
	vec3 vector = point2 - point1;
	float mag = glm::length(vector);
	vec3 result = vector / mag;
	return result;
}

vec3 projectToY(vec3 vector)
{
	vec3 result = vec3(vector.x, 0.0f, vector.z);
	float mag = glm::length(result);
	result /= mag;
	return result;
}

vec3 projectToX(vec3 vector)
{
	vec3 result = vec3(0.0f, vector.y, vector.z);
	float mag = glm::length(result);
	result /= mag;
	return result;
}

vec3 intermediateVector(vec3 centerVec, vec3 proj)
{
	vec3 temp = centerVec - proj;
	float mag = glm::length(temp);
	vec3 result = temp / mag;
	return result;
}

float yAngle(vec3 intermediateVec)
{
	float theta;
	if (intermediateVec.z > 0)
	{
		if (intermediateVec.x > 0)
		{
			theta = glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, 1.0f)));
		}
		else
		{
			theta = -glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, 1.0f)));
		}
	}
	else
	{
		if (intermediateVec.x > 0)
		{
			theta = -glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, -1.0f)));
		}
		else
		{
			theta = glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, -1.0f)));
		}
	}
	return theta;
}

float xAngle(vec3 intermediateVec)
{
	float theta;
	if (intermediateVec.z > 0)
	{
		if (intermediateVec.y < 0)
		{
			theta = glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, 1.0f)));
		}
		else
		{
			theta = -glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, 1.0f)));
		}
	}
	else
	{
		if (intermediateVec.y < 0)
		{
			theta = -glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, -1.0f)));
		}
		else
		{
			theta = glm::acos(glm::dot(intermediateVec, vec3(0.0f, 0.0f, -1.0f)));
		}
	}
	return theta;
}

//draws coordinate systems
void drawCoord(float x, float y, float z, float a, float b, float c)
{
	mat4 ModelToWorldMatrix =
		glm::translate(vec3(x, y, z - 0.1f)) *
		glm::rotate(degToRad(c), vec3(0.0f, 0.0f, 1.0f))*
		glm::rotate(degToRad(a), vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(degToRad(b), vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = worldToProjectionMatrix * ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	return;
}

//draws arbitrary shape
void drawShape(float x, float y, float z, float a, float b)
{
	mat4 ModelToWorldMatrix =
		glm::translate(vec3(x, y, z - 0.1f)) *
		glm::scale(vec3(1.0f,1.0f,1.0f)) *
		glm::rotate(degToRad(-90.0f), vec3(1.0f, 0.0f, 0.0f))*
		glm::rotate(degToRad(a), vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(degToRad(-b), vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = worldToProjectionMatrix * ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	return;
}

//Cylinders Draws
void drawCylinderTop(float x, float y, float z, float a, float b)
{
	mat4 ModelToWorldMatrix =
		glm::translate(vec3(x, y, z - 0.1f)) *
		glm::rotate(degToRad(-90.0f), vec3(1.0f, 0.0f, 0.0f))*
		glm::rotate(degToRad(a), vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(degToRad(-b), vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = worldToProjectionMatrix * ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	return;
}

void drawCylinderSide(float x, float y, float z, float a, float b)
{
	mat4 ModelToWorldMatrix =
		glm::translate(vec3(x, y, z - 0.1f)) *
		glm::rotate(degToRad(90.0f), vec3(0.0f, 1.0f, 0.0f))*
		glm::rotate(degToRad(a), vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(degToRad(-b), vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = worldToProjectionMatrix * ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	return;
}

void drawCylinderNormal(float x, float y, float z, float a, float b)
{
	mat4 ModelToWorldMatrix =
		glm::translate(vec3(x, y, z)) *
		glm::rotate(degToRad(a), vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(degToRad(b), vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = worldToProjectionMatrix * ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	return;
}

//Tube Draws
void drawTubeNormal(float x, float y, float z, vec3 point1, vec3 point2)
{
	vec3 centerDirVec = centerDirectionVec(point1, point2);
	vec3 centerVector = centerVec(point1, point2);
	vec3 projY = projectToY(centerDirVec);
	vec3 projX = projectToX(centerDirVec);
	float yTheta = radToDeg(yAngle(projY));
	float xTheta = radToDeg(xAngle(projX));
	drawCylinderNormal(x, y, z, xTheta, yTheta);
}

void drawTubeSide(float x, float y, float z, vec3 point1, vec3 point2)
{
	vec3 centerDirVec = centerDirectionVec(point1, point2);
	vec3 centerVector = centerVec(point1, point2);
	vec3 projY = projectToY(centerDirVec);
	vec3 projX = projectToX(centerDirVec);
	float yTheta = radToDeg(yAngle(projY));
	float xTheta = radToDeg(xAngle(projX));
	drawCylinderSide(z, y, -x, xTheta, -yTheta);
}

void drawTubeTop(float x, float y, float z, vec3 point1, vec3 point2)
{
	vec3 centerDirVec = centerDirectionVec(point1, point2);
	vec3 centerVector = centerVec(point1, point2);
	vec3 projY = projectToY(centerDirVec);
	vec3 projX = projectToX(centerDirVec);
	float yTheta = radToDeg(yAngle(projY));
	float xTheta = radToDeg(xAngle(projX));
	drawCylinderTop(x, -z-0.1, y, xTheta, yTheta);
}

//Trocars draw
void drawTrocar()
{
	bindBuffer(trocarBaseVertexBufferID, trocarBaseIndexBufferID);
	drawTubeNormal(0.0f, 0.0f, toPixel(0.02f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	bindBuffer(trocarHoleVertexBufferID, trocarHoleIndexBufferID);
	drawTubeNormal(-toPixel(2.5f), 0.0f, toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	drawTubeNormal(toPixel(2.5f), 0.0f, toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
}

void drawTrocarSide()
{
	bindBuffer(trocarBaseVertexBufferID, trocarBaseIndexBufferID);
	drawTubeSide(0.0f, 0.0f, toPixel(0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	bindBuffer(trocarHoleVertexBufferID, trocarHoleIndexBufferID);
	drawTubeSide(-toPixel(2.5f), 0.0f, toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	drawTubeSide(toPixel(2.5f), 0.0f, toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
}

void drawTrocarTop()
{
	bindBuffer(trocarBaseVertexBufferID, trocarBaseIndexBufferID);
	drawTubeTop(0.0f, 0.0f, toPixel(30.0f) + toPixel(0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	bindBuffer(trocarHoleVertexBufferID, trocarHoleIndexBufferID);
	drawTubeTop(-toPixel(2.5f), 0.0f, toPixel(30.0f) + toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
	drawTubeTop(toPixel(2.5f), 0.0f, toPixel(30.0f) + toPixel(-0.02), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
}

//Draw Arms
void drawArm(void(*trocarDraw)(), void(*tubeDraw)(float x, float y, float z, vec3 point1, vec3 point2),
	arm *left, arm* right, float qValuesLeft[NUMOFQVALS], float qValuesRight[NUMOFQVALS])
{
	int rightIndex = 0;
	int leftIndex = 0;
	//trocar 				  
	(*trocarDraw)();
	//Outer
	
	bindBuffer(outerTubeVertexBufferID, outerTubeIndexBufferID);
	for (int i = leftIndex; i < left->numDivisions; i++)
	{
		if (i*DIVISIONSIZE < qValuesLeft[2])
		{
			(*tubeDraw)(toPixel(left->x[i]) - toPixel(2.5f), toPixel(left->y[i]), toPixel(-left->z[i]),
				vec3(toPixel(left->x[i]), toPixel(left->y[i]), toPixel(-left->z[i])),
				vec3(toPixel(left->x[i + 1]), toPixel(left->y[i + 1]), toPixel(-left->z[i + 1])));
			leftIndex++;
		}
	}
	for (int i = rightIndex; i < right->numDivisions; i++)
	{
		if (i*DIVISIONSIZE < qValuesRight[2])
		{
			(*tubeDraw)(toPixel(right->x[i]) + toPixel(2.5f), toPixel(right->y[i]), toPixel(-right->z[i]),
				vec3(toPixel(right->x[i]), toPixel(right->y[i]), toPixel(-right->z[i])),
				vec3(toPixel(right->x[i + 1]), toPixel(right->y[i + 1]), toPixel(-right->z[i + 1])));
			rightIndex++;
		}
	}
	//Inner
	bindBuffer(innerTubeVertexBufferID, innerTubeIndexBufferID);
	for (int i = leftIndex; i < left->numDivisions; i++)
	{
		(*tubeDraw)(toPixel(left->x[i]) - toPixel(2.5f), toPixel(left->y[i]), toPixel(-left->z[i]),
			vec3(toPixel(left->x[i]), toPixel(left->y[i]), toPixel(-left->z[i])),
			vec3(toPixel(left->x[i + 1]), toPixel(left->y[i + 1]), toPixel(-left->z[i + 1])));
	}
	for (int i = rightIndex; i < right->numDivisions; i++)
	{
		(*tubeDraw)(toPixel(right->x[i]) + toPixel(2.5f), toPixel(right->y[i]), toPixel(-right->z[i]),
			vec3(toPixel(right->x[i]), toPixel(right->y[i]), toPixel(-right->z[i])),
			vec3(toPixel(right->x[i + 1]), toPixel(right->y[i + 1]), toPixel(-right->z[i + 1])));
	}
}

//boundary Draws
void drawBoundary(void(*drawTube)(float x, float y, float z, vec3 point1, vec3 point2))
{
	for (int i = 0; i < NUMBOUND; i+=1)
	{
		if (LBound == 1)
		{
			bindBuffer(boundaryVertexBufferIDs[i], boundaryIndexBufferIDs[i]);
			drawTube(toPixel(-2.5f), 0.0f, -toPixel((float)i), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
		}
		if (RBound == 1)
		{
			bindBuffer(RboundaryVertexBufferIDs[i], RboundaryIndexBufferIDs[i]);
			drawTube(toPixel(2.5f), 0.0f, -toPixel((float)i), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
		}
	}
}

//UDP Parsing
double charArrayToDouble(char input[])
{
	double output;
	int pos = 0;
	double above[13];
	double below[13];
	int aboveNumLength = 0;
	double aboveNum = 0;
	double belowNum = 0;
	bool isNeg = false;
	bool hasDec = false;


	for (int i = 1; i <= 12; i++)
	{
		if (input[i] != 46)
		{
			above[i] = input[i];
			aboveNumLength++;
		}
		else if (input[i] == 46)
		{
			pos = i + 1;
			hasDec = true;
			break;
		}
	}
	if (hasDec)
	{
		for (int i = pos; i <= 12; i++)
		{
			if (input[i] != 46)
			{
				below[i] = input[i];
			}
		}
	}
	int multiplier = 1;
	for (int i = 0; i < aboveNumLength; i++)
	{
		if (above[aboveNumLength - i] != 45)
		{
			aboveNum = aboveNum + (above[aboveNumLength - i] - 48) * multiplier;
			multiplier = multiplier * 10;
		}
		else if (above[aboveNumLength - i] == 45)
		{
			aboveNum = aboveNum * -1;
			isNeg = true;
			break;
		}
	}
	if (hasDec)
	{
		float divider = 0.1f;
		for (int i = aboveNumLength + 2; i <= 12; i++)
		{
			belowNum = belowNum + float(below[i] - 48) * divider;
			divider = divider / 10.0f;
		}
	}

	if (isNeg)
		aboveNum -= belowNum;
	else
		aboveNum += belowNum;
	output = aboveNum;
	return output;
}

void calibration()
{
	float temp[NUMOFVALS]; //NOTE: All these random numbers come from the Simulink Calibration block, (They are all inverted)
	temp[0] = packet[1] + 1.47; //Front Rot
	temp[1] = (packet[3] - 0.4) * 10; //Front Trans
	temp[2] = packet[0] - 1.4; //Mid Rot
	temp[3] = (packet[4] - 1.9) * 10; //Mid Trans
	temp[4] = packet[2] - 0.06; //Back Rot
	temp[5] = (packet[5] - 0.03) * 10; //Back Trans
	temp[6] = packet[6] - 1.4;//Front Rot
	temp[7] = (packet[9] - 0.4) * 10; //Front Trans
	temp[8] = packet[7] + 1.47;//Mid Rot
	temp[9] = (packet[10] - 1.9) * 10; //Mid Trans
	temp[10] = packet[8] - 0.06; //Back Rot
	temp[11] = (packet[11] - 0.03) * 10; //Back Trans

	for (int i = 0; i < NUMOFVALS; i++)
	{
		packet[i] = temp[i];
	}
}

void readPacket(char input[])
{
	char valHolder[BYTEPERVAL];
	for (int j = 0; j < NUMOFVALS; j++)
	{
		for (int i = 0; i < BYTEPERVAL; i++)
		{
			valHolder[i] = input[j*BYTEPERVAL + i];
		}
		packet[j] = charArrayToDouble(valHolder);
		if (packet[j] == 5450)
			packet[j] = 0;
	}
	calibration();
}


int main(void)
{
	std::ofstream pastQvals; //Opens file to store Qvals
	pastQvals.open("pastQValues.txt");

	//FreeConsole(); //Closes Windows Console

	//Sets up UDP socket
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "ERROR WITH WSA" << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	SOCKET input = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in inputPort;
	InetPton(AF_INET, IPADDRESS, &inputPort.sin_addr.S_un.S_addr);
	inputPort.sin_family = AF_INET;
	inputPort.sin_port = htons(UDPPORT);
	if (bind(input, (sockaddr*)&inputPort, sizeof(inputPort)) == SOCKET_ERROR)
	{
		std::cout << "ERROR BINDING SOCKET" << WSAGetLastError() << std::endl;
		system("pause");
		return 1;
	}
	sockaddr_in client;
	int clientLength = sizeof(client);
	char buf[BUFLEN];

	//Sets up rendering Window
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arm Graphics", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//Sets up GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error Initializing GLEW" << std::endl;
		system("pause");
		return -1;
	}

	//Preps Rendering WIndow
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShader();

	fullTransformationUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	/* Loop until the user closes the window */
	float a = 17.0f;
	float b = 0.0f;
	float dir = 1;
	
	while (!glfwWindowShouldClose(window))
	{
		arm *left = new arm(); //creates arm objects
		arm *right = new arm();
		float qValuesLeft[NUMOFQVALS] = { -3.0f, 5 , 10, 15, 37};
		float qValuesRight[NUMOFQVALS] = { 2, 0 , 3, 1, 30};
	//	float qValuesLeft[NUMOFQVALS];
	//	float qValuesRight[NUMOFQVALS];

		//pastQvals << "Writing this to a file";
		pastQvals << buf << "\n";
		
		//Reads UDP packet and loads coordinates
		/*
		int bytesIn = recvfrom(input, buf, BUFLEN, 0, (sockaddr*)&client, &clientLength); // reads packets over UDP
		readPacket(buf);
		//std::cout << buf << std::endl;
		for (int i = 0; i < NUMOFQVALS; i++)
		{
			qValuesLeft[i] = packet[i];
			//qValuesLeft[i] = 0;
			//std::cout << qValuesLeft[i] << std::endl;
		}
		
		for (int i = 0; i < NUMOFQVALS; i++)
		{
			//qValuesRight[i] = packet[i+6];
			qValuesRight[i] = 0;
		}*/

		//load qvalues and perform forward kinematics
		left->loadQVals(qValuesLeft);
		right->loadQVals(qValuesRight);
		left->Detection();
		right->Detection();

		adjustCamera();
		worldToProjection();

		/* Rendering */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (MODE == 0)
		{
			//Bottom Left (angled view)
			glViewport(0, 0, WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5);
			//Coordinates
			bindBuffer(zVertexBufferID, zIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // z axis
			bindBuffer(yVertexBufferID, yIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 90.0f, 0.0f, 0.0f); // y axis
			bindBuffer(xVertexBufferID, xIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, -90.0f, 0.0f); // x axis
			drawArm(&drawTrocar, &drawTubeNormal, left, right, qValuesLeft, qValuesRight); //draw Arms
			bindBuffer(triangleVertexBufferID, triangleIndexBufferID);
			//drawShape(toPixel(-2.5f), 0.0f, -toPixel(20), 90.0f, 0.0f);
			drawBoundary(&drawTubeNormal); //Work space

			//Bottom Right (side view)
			glViewport(WINDOW_WIDTH*0.5, 0, WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5);
			//Coordinates
			bindBuffer(zVertexBufferID, zIndexBufferID);
			drawCoord(-0.075f, 0.1f, 0.0f, 0.0f, 90.0f, 0.0f); // z axis
			bindBuffer(yVertexBufferID, yIndexBufferID);
			drawCoord(-0.075f, 0.1f, 0.0f, 90.0f, 0.0f, 0.0f); // y axis
			bindBuffer(xVertexBufferID, xIndexBufferID);
			drawCoord(-0.075f, 0.1, 0.0f, 0.0f, 0.0f, 0.0f); // x axis
			drawArm(&drawTrocarSide, &drawTubeSide, left, right, qValuesLeft, qValuesRight); //draw Arms
			bindBuffer(triangleVertexBufferID, triangleIndexBufferID);
			//drawShape(-(0.1f+ toPixel(20)), 0.0f, toPixel(2.5f), 90.0f, 90.0f);
			drawBoundary(&drawTubeSide); //Work space

			//Top Left (top view)
			glViewport(0, WINDOW_HEIGHT*0.5, WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5);
			//Coordinates
			bindBuffer(zVertexBufferID, zIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 90.0f, 0.0f, 0.0f); // z axis
			bindBuffer(yVertexBufferID, yIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, 180.0f, 0.0f); // y axis
			bindBuffer(xVertexBufferID, xIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, -90.0f, 0.0f); // x axis
			drawArm(&drawTrocarTop, &drawTubeTop, left, right, qValuesLeft, qValuesRight); //draw Arms
			bindBuffer(triangleVertexBufferID, triangleIndexBufferID);
			//drawShape(toPixel(-2.5f), toPixel(20), 0.0f, 0.0f, 0.0f);
			drawBoundary(&drawTubeTop); //Work space
		}
		else if (MODE == 1)
		{
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			//Coordinates
			bindBuffer(zVertexBufferID, zIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f); // z axis
			bindBuffer(yVertexBufferID, yIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 90.0f, 0.0f, 0.0f); // y axis
			bindBuffer(xVertexBufferID, xIndexBufferID);
			drawCoord(-0.25f, 0.0f, 0.1f, 0.0f, -90.0f, 0.0f); // x axis
			drawArm(&drawTrocar, &drawTubeNormal, left, right, qValuesLeft, qValuesRight); //draw Arms
			bindBuffer(triangleVertexBufferID, triangleIndexBufferID);
			//drawShape(toPixel(-2.5f), 0.0f, -0.17, 90.0f, 0.0f);
			drawBoundary(&drawTubeNormal); //Work space
		}
		else if (MODE == 2)
		{
			std::ifstream myReadFile;
			myReadFile.open("pastQValues.txt");
			char buffer[BUFLEN];
			if (myReadFile.is_open()) {
				myReadFile.getline(buffer, 22, ' ');
				std::cout << buffer << std::endl;
			}
			myReadFile.close();
		}

		/* Swap front and back buffers */
		left->cleanup();
		right->cleanup();
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();

		if (b > 2.5 || b < 0)
		{
			dir *= -1.0f;
		}
		a += dir * 0.5f;
		b += dir * 0.05f;

	}
	pastQvals.close();
	WSACleanup();
	closesocket(input);
	glfwTerminate();
	return 0;
}

