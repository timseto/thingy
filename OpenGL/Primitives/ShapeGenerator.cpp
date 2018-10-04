#include "ShapeGenerator.h"
#include <glm.hpp>
#include <Primitives/Vertex.h>
#include <iostream>
#include <math.h>
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
#define degInterval 10
#define degToRad(x) glm::radians(x)
#define pointPerTri 3
#define bufferOffset 360 / degInterval + 1 
#define numSideTriangles  360 / degInterval * 2 * 3

using glm::vec3;
using glm::vec4;


ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData ret;

	Vertex myTri[] =
	{
		vec3(0,1,0),
		vec4(0,1,0,0.5),
		vec3(-1,-1,0),
		vec4(0,1,0,0.5),
		vec3(1,-1,0),
		vec4(0,1,0,0.5),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = { 0, 1, 2 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

/*
ShapeData ShapeGenerator::makeCube() {
	ShapeData ret;
	Vertex stackVerts[] = {
		vec3(-1.0f, +1.0f, +1.0f), // 0
		vec3(+1.0f, +0.0f, +0.0f), // Color
		vec3(+1.0f, +1.0f, +1.0f), // 1
		vec3(+0.0f, +1.0f, +0.0f), // Color
		vec3(+1.0f, +1.0f, -1.0f), // 2
		vec3(+0.0f, +0.0f, +1.0f), // Color
		vec3(-1.0f, +1.0f, -1.0f), // 3
		vec3(+1.0f, +1.0f, +1.0f), // Color
		
		vec3(-1.0f, +1.0f, -1.0f), // 4
		vec3(+1.0f, +0.0f, +1.0f), // Color
		vec3(+1.0f, +1.0f, -1.0f), // 5
		vec3(+0.0f, +0.5f, +0.2f), // Color
		vec3(+1.0f, -1.0f, -1.0f), // 6
		vec3(+0.8f, +0.6f, +0.4f), // Color
		vec3(-1.0f, -1.0f, -1.0f), // 7
		vec3(+0.3f, +1.0f, +0.5f), // Color
		
		vec3(+1.0f, +1.0f, -1.0f), // 8
		vec3(+0.2f, +0.5f, +0.2f), // Color
		vec3(+1.0f, +1.0f, +1.0f), // 9
		vec3(+0.9f, +0.3f, +0.7f), // Color
		vec3(+1.0f, -1.0f, +1.0f), // 10
		vec3(+0.3f, +0.7f, +0.5f), // Color
		vec3(+1.0f, -1.0f, -1.0f), // 11
		vec3(+0.5f, +0.7f, +0.5f), // Color
		
		vec3(-1.0f, +1.0f, +1.0f), // 12
		vec3(+0.7f, +0.8f, +0.2f), // Color
		vec3(-1.0f, +1.0f, -1.0f), // 13
		vec3(+0.5f, +0.7f, +0.3f), // Color
		vec3(-1.0f, -1.0f, -1.0f), // 14
		vec3(+0.4f, +0.7f, +0.7f), // Color
		vec3(-1.0f, -1.0f, +1.0f), // 15
		vec3(+0.2f, +0.5f, +1.0f), // Color
		
		vec3(+1.0f, +1.0f, +1.0f), // 16
		vec3(+0.6f, +1.0f, +0.7f), // Color
		vec3(-1.0f, +1.0f, +1.0f), // 17
		vec3(+0.6f, +0.4f, +0.8f), // Color
		vec3(-1.0f, -1.0f, +1.0f), // 18
		vec3(+0.2f, +0.8f, +0.7f), // Color
		vec3(+1.0f, -1.0f, +1.0f), // 19
		vec3(+0.2f, +0.7f, +1.0f), // Color
		
		vec3(+1.0f, -1.0f, -1.0f), // 20
		vec3(+0.8f, +0.3f, +0.7f), // Color
		vec3(-1.0f, -1.0f, -1.0f), // 21
		vec3(+0.8f, +0.9f, +0.5f), // Color
		vec3(-1.0f, -1.0f, +1.0f), // 22
		vec3(+0.5f, +0.8f, +0.5f), // Color
		vec3(+1.0f, -1.0f, +1.0f), // 23
		vec3(+0.9f, +1.0f, +0.2f), // Color
		};
	
		ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));
	
		unsigned short stackIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
		};
	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeArrowSquare()
{
	ShapeData ret;
	Vertex stackVerts[] =
		{
				// Top side of arrow head
			vec3(+0.00f, +0.25f, -0.25f),         // 0
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(+0.50f, +0.25f, -0.25f),         // 1
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(+0.00f, +0.25f, -1.00f),         // 2
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(-0.50f, +0.25f, -0.25f),         // 3
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
					// Bottom side of arrow head
			vec3(+0.00f, -0.25f, -0.25f),         // 4
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(+0.50f, -0.25f, -0.25f),         // 5
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(+0.00f, -0.25f, -1.00f),         // 6
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(-0.50f, -0.25f, -0.25f),         // 7
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
				    // Right side of arrow tip
			vec3(+0.50f, +0.25f, -0.25f),         // 8
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.00f, +0.25f, -1.00f),         // 9
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.00f, -0.25f, -1.00f),         // 10
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.50f, -0.25f, -0.25f),         // 11
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
					// Left side of arrow tip
			vec3(+0.00f, +0.25f, -1.00f),         // 12
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(-0.50f, +0.25f, -0.25f),         // 13
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(+0.00f, -0.25f, -1.00f),         // 14
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(-0.50f, -0.25f, -0.25f),         // 15
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			  	    // Back side of arrow tip
			vec3(-0.50f, +0.25f, -0.25f),         // 16
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(+0.50f, +0.25f, -0.25f),         // 17
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(-0.50f, -0.25f, -0.25f),         // 18
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(+0.50f, -0.25f, -0.25f),         // 19
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
					// Top side of back of arrow
			vec3(+0.25f, +0.25f, -0.25f),         // 20
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(+0.25f, +0.25f, +1.00f),         // 21
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(-0.25f, +0.25f, +1.00f),         // 22
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			vec3(-0.25f, +0.25f, -0.25f),         // 23
			vec3(+1.00f, +0.00f, +0.00f),		  // Color
			        // Bottom side of back of arrow
			vec3(+0.25f, -0.25f, -0.25f),         // 24
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(+0.25f, -0.25f, +1.00f),         // 25
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(-0.25f, -0.25f, +1.00f),         // 26
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
			vec3(-0.25f, -0.25f, -0.25f),         // 27
			vec3(+0.00f, +0.00f, +1.00f),		  // Color
					// Right side of back of arrow
			vec3(+0.25f, +0.25f, -0.25f),         // 28
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.25f, -0.25f, -0.25f),         // 29
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.25f, -0.25f, +1.00f),         // 30
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
			vec3(+0.25f, +0.25f, +1.00f),         // 31
			vec3(+0.60f, +1.00f, +0.00f),		  // Color
					// Left side of back of arrow
			vec3(-0.25f, +0.25f, -0.25f),         // 32
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(-0.25f, -0.25f, -0.25f),         // 33
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(-0.25f, -0.25f, +1.00f),         // 34
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
			vec3(-0.25f, +0.25f, +1.00f),         // 35
			vec3(+0.00f, +1.00f, +0.00f),		  // Color
					// Back side of back of arrow
			vec3(-0.25f, +0.25f, +1.00f),         // 36
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(+0.25f, +0.25f, +1.00f),         // 37
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(-0.25f, -0.25f, +1.00f),         // 38
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			vec3(+0.25f, -0.25f, +1.00f),         // 39
			vec3(+0.50f, +0.50f, +0.50f),		  // Color
			};

		GLushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
		};
	
	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));
	
	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}
*/
ShapeData ShapeGenerator::makeCircle(float radius)
{
	ShapeData ret;
	float x[360 / degInterval];
	float y[360 / degInterval];
	Vertex stackVerts[360 / degInterval + 1]; // + 1 because you need to store coords of center
	GLushort stackIndices[360 / degInterval * pointPerTri];

	//Center of base
	stackVerts[360 / degInterval].position = vec3(0.0f,0.0f, 0.0f);
	stackVerts[360 / degInterval].color = vec4(+1.00f, +0.00f, +0.00f,0.5f);

	for (int degrees = 0; degrees < 360/degInterval; degrees++)
	{
		x[degrees] = cos(degToRad((float)degrees*degInterval));
		y[degrees] = sin(degToRad((float)degrees*degInterval));
		stackVerts[degrees].position = vec3(x[degrees] * radius, y[degrees] * radius, 0.0f);
		stackVerts[degrees].color = vec4(+1.00f, +0.00f, +0.00f, 0.5f);
	}

	for (int i = 0; i < 360 / degInterval; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval;
		stackIndices[i*pointPerTri + 1] = i;
		stackIndices[i*pointPerTri + 2] = i + 1;
	}
	stackIndices[360 / degInterval * pointPerTri- 1] = 0;

	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}


ShapeData ShapeGenerator::makeCylinder(float radius, float height, float red, float green, float blue, float alpha)
{
	ShapeData ret;
	float x[360 / degInterval];
	float y[360 / degInterval];
	Vertex stackVerts[(360 / degInterval * 2) + 2]; // + 2 because you need to store coords of center base & center top and * 2 because top and base
	GLushort stackIndices[(360 / degInterval * pointPerTri * 2) + numSideTriangles];

	//Center of base vertex
	stackVerts[360 / degInterval].position = vec3(0.0f, 0.0f, 0);
	stackVerts[360 / degInterval].color = vec4(0.0f, 0.0f, 0.0f,1.0f);
	//Center of top vertex
	stackVerts[360 / degInterval * 2 + 1].position = vec3(0.0f, 0.0f, -height);
	stackVerts[360 / degInterval * 2 + 1].color = vec4(0.0f, 0.0f, 0.0f,1.0f);

	for (int degrees = 0; degrees < 360 / degInterval; degrees++)
	{
		x[degrees] = cos(degToRad((float)degrees*degInterval));
		y[degrees] = sin(degToRad((float)degrees*degInterval));
		stackVerts[degrees].position = vec3(x[degrees] * radius, y[degrees] * radius, 0.0f);
		stackVerts[degrees].color = vec4(red, green, blue, alpha);
		stackVerts[degrees + bufferOffset].position = vec3(x[degrees] * radius, y[degrees] * radius, -height);
		stackVerts[degrees + bufferOffset].color = vec4(red, green, blue, alpha);
	}

	//Base
	for (int i = 0; i < 360 / degInterval; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval;
		stackIndices[i*pointPerTri + 1] = i;
		stackIndices[i*pointPerTri + 2] = i + 1;
	}
	stackIndices[360 / degInterval * pointPerTri - 1] = 0; //has to draw triangle from last vertex to first
	
	//Top
	for (int i = 360 / degInterval; i < 360 / degInterval * 2; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval + bufferOffset;
		stackIndices[i*pointPerTri + 1] = i + 1;
		stackIndices[i*pointPerTri + 2] = i + 2;
	}
	stackIndices[(360 / degInterval * pointPerTri) * 2 - 1] = bufferOffset; //has to draw triangle from last vertex to first

	//Note stackIndices indexs from [360/degInterval * pointPerTri * 2 to (360 / degInterval * pointPerTri * 2) + numSideTriangles - 1]
	int sideTriangleIndex = 360 / degInterval * pointPerTri * 2;
	for (int i = 0; i < 360 / degInterval; i++) 
	{
		if (i == 0)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = 360 / degInterval * 2;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else if (i == 360 / degInterval - 1)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = 0;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
	}
	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeTrocar(float radius, float height, float red, float green, float blue, float alpha)
{
	ShapeData ret;
	float x[360 / degInterval];
	float y[360 / degInterval];
	Vertex stackVerts[(360 / degInterval * 2) + 2]; // + 2 because you need to store coords of center base & center top and * 2 because top and base
	GLushort stackIndices[(360 / degInterval * pointPerTri * 2) + numSideTriangles];

	//Center of base vertex
	stackVerts[360 / degInterval].position = vec3(0.0f, 0.0f, 0.0f);
	stackVerts[360 / degInterval].color = vec4(red, green, blue, alpha);
	//Center of top vertex
	stackVerts[360 / degInterval * 2 + 1].position = vec3(0.0f, 0.0f, height);
	stackVerts[360 / degInterval * 2 + 1].color = vec4(red, green, blue, alpha);

	for (int degrees = 0; degrees < 360 / degInterval; degrees++)
	{
		x[degrees] = cos(degToRad((float)degrees*degInterval));
		y[degrees] = sin(degToRad((float)degrees*degInterval));
		stackVerts[degrees].position = vec3(x[degrees] * radius, y[degrees] * radius, 0.0f);
		stackVerts[degrees].color = vec4(red, green, blue, alpha);
		stackVerts[degrees + bufferOffset].position = vec3(x[degrees] * radius, y[degrees] * radius, height);
		stackVerts[degrees + bufferOffset].color = vec4(red, green, blue, alpha);
	}

	//Base
	for (int i = 0; i < 360 / degInterval; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval;
		stackIndices[i*pointPerTri + 1] = i;
		stackIndices[i*pointPerTri + 2] = i + 1;
	}
	stackIndices[360 / degInterval * pointPerTri - 1] = 0; //has to draw triangle from last vertex to first

														   //Top
	for (int i = 360 / degInterval; i < 360 / degInterval * 2; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval + bufferOffset;
		stackIndices[i*pointPerTri + 1] = i + 1;
		stackIndices[i*pointPerTri + 2] = i + 2;
	}
	stackIndices[(360 / degInterval * pointPerTri) * 2 - 1] = bufferOffset; //has to draw triangle from last vertex to first

																			//Note stackIndices indexs from [360/degInterval * pointPerTri * 2 to (360 / degInterval * pointPerTri * 2) + numSideTriangles - 1]
	int sideTriangleIndex = 360 / degInterval * pointPerTri * 2;
	for (int i = 0; i < 360 / degInterval; i++)
	{
		if (i == 0)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = 360 / degInterval * 2;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else if (i == 360 / degInterval - 1)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = 0;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
	}
	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeArrowCylinder(float red, float green, float blue, float alpha)
{
	ShapeData ret;
	float height = 0.07f;
	float radius = 0.005f;
	float x[360 / degInterval];
	float y[360 / degInterval];
	Vertex stackVerts[(360 / degInterval * 2) + 2]; // + 2 because you need to store coords of center base & center top and * 2 because top and base
	GLushort stackIndices[(360 / degInterval * pointPerTri * 2) + numSideTriangles];

	//Center of base vertex
	stackVerts[360 / degInterval].position = vec3(0.0f, 0.0f, 0.0f);
	stackVerts[360 / degInterval].color = vec4(red, green, blue, alpha);
	//Center of top vertex
	stackVerts[360 / degInterval * 2 + 1].position = vec3(0.0f, 0.0f, -(height + 0.01f));
	stackVerts[360 / degInterval * 2 + 1].color = vec4(red, green, blue, alpha);

	for (int degrees = 0; degrees < 360 / degInterval; degrees++)
	{
		x[degrees] = cos(degToRad((float)degrees*degInterval));
		y[degrees] = sin(degToRad((float)degrees*degInterval));
		stackVerts[degrees].position = vec3(x[degrees] * radius, y[degrees] * radius, 0.0f);
		stackVerts[degrees].color = vec4(red, green, blue, alpha);
		stackVerts[degrees + bufferOffset].position = vec3(x[degrees] * radius, y[degrees] * radius, -height);
		stackVerts[degrees + bufferOffset].color = vec4(red, green, blue, alpha);
	}

	//Base
	for (int i = 0; i < 360 / degInterval; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval;
		stackIndices[i*pointPerTri + 1] = i;
		stackIndices[i*pointPerTri + 2] = i + 1;
	}
	stackIndices[360 / degInterval * pointPerTri - 1] = 0; //has to draw triangle from last vertex to first

														   //Top
	for (int i = 360 / degInterval; i < 360 / degInterval * 2; i++)
	{
		stackIndices[i*pointPerTri] = 360 / degInterval + bufferOffset;
		stackIndices[i*pointPerTri + 1] = i + 1;
		stackIndices[i*pointPerTri + 2] = i + 2;
	}
	stackIndices[(360 / degInterval * pointPerTri) * 2 - 1] = bufferOffset; //has to draw triangle from last vertex to first

																			//Note stackIndices indexs from [360/degInterval * pointPerTri * 2 to (360 / degInterval * pointPerTri * 2) + numSideTriangles - 1]
	int sideTriangleIndex = 360 / degInterval * pointPerTri * 2;
	for (int i = 0; i < 360 / degInterval; i++)
	{
		if (i == 0)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = 360 / degInterval * 2;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else if (i == 360 / degInterval - 1)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = 0;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
	}
	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeBoundary(float radius1, float radius2, float height,float red, float green, float blue, float alpha)
{
	
	ShapeData ret;
	float x[360 / degInterval];
	float y[360 / degInterval];
	Vertex stackVerts[(360 / degInterval * 2) + 2]; // + 2 because you need to store coords of center base & center top and * 2 because top and base
	GLushort stackIndices[(360 / degInterval * pointPerTri * 2) + numSideTriangles];

	//Center of base vertex
	stackVerts[360 / degInterval].position = vec3(0.0f, 0.0f, 0.0f);
	stackVerts[360 / degInterval].color = vec4(red, green, blue, alpha);
	//Center of top vertex
	stackVerts[360 / degInterval * 2 + 1].position = vec3(0.0f, 0.0f, -(height + 0.01f));
	stackVerts[360 / degInterval * 2 + 1].color = vec4(red, green, blue, alpha);

	for (int degrees = 0; degrees < 360 / degInterval; degrees++)
	{
		x[degrees] = cos(degToRad((float)degrees*degInterval));
		y[degrees] = sin(degToRad((float)degrees*degInterval));
		stackVerts[degrees].position = vec3(x[degrees] * radius2, y[degrees] * radius2, 0.0f);
		stackVerts[degrees].color = vec4(red, green, blue, alpha);
		stackVerts[degrees + bufferOffset].position = vec3(x[degrees] * radius1, y[degrees] * radius1, -height);
		stackVerts[degrees + bufferOffset].color = vec4(red, green, blue, alpha);
	}

	//Base
	for (int i = 0; i < 360 / degInterval; i++)
	{
		stackIndices[i*pointPerTri] = 0;
		stackIndices[i*pointPerTri + 1] = 0;
		stackIndices[i*pointPerTri + 2] = 0;
	}
	stackIndices[360 / degInterval * pointPerTri - 1] = 0; //has to draw triangle from last vertex to first

														   //Top
	for (int i = 360 / degInterval; i < 360 / degInterval * 2; i++)
	{
		stackIndices[i*pointPerTri] = 0;
		stackIndices[i*pointPerTri + 1] = 0;
		stackIndices[i*pointPerTri + 2] = 0;
	}
	stackIndices[(360 / degInterval * pointPerTri) * 2 - 1] = 0;

	//Note stackIndices indexs from [360/degInterval * pointPerTri * 2 to (360 / degInterval * pointPerTri * 2) + numSideTriangles - 1]
	int sideTriangleIndex = 360 / degInterval * pointPerTri * 2;
	for (int i = 0; i < 360 / degInterval; i++)
	{
		if (i == 0)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = 360 / degInterval * 2;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else if (i == 360 / degInterval - 1)
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = 0;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
		else
		{
			stackIndices[sideTriangleIndex + (i * 6)] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 1] = i + 360 / degInterval;
			stackIndices[sideTriangleIndex + (i * 6) + 2] = i + 360 / degInterval + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 3] = i;
			stackIndices[sideTriangleIndex + (i * 6) + 4] = i + 1;
			stackIndices[sideTriangleIndex + (i * 6) + 5] = i + 360 / degInterval + 1;
		}
	}
	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}