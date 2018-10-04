#pragma once
#include <Primitives/ShapeData.h>
 
class ShapeGenerator
{
public:
	
	static ShapeData makeTriangle();
	/*
	static ShapeData makeCube();
	static ShapeData makeArrowSquare();
	*/
	static ShapeData makeCircle(float radius);
	
	static ShapeData makeCylinder(float radius, float height, float red, float green, float blue, float alpha);
	static ShapeData makeTrocar(float radius, float height, float red, float green, float blue, float alpha);
	static ShapeData makeArrowCylinder(float red, float green, float blue, float alpha);
	static ShapeData makeBoundary(float radius1, float radius2, float height, float red, float green, float blue, float alpha);
};