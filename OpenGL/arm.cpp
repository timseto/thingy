#include "arm.h"
#include <iostream>
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
#define NUMOFQVALS 5
#define DIVISIONSIZE 0.5f

arm::arm()
{
	int config = 0;
	float *qVals = new float[NUMOFQVALS];
	float *divisions = NULL;
	int numDivisions = 0;
 	float* x = NULL;
	float* y = NULL;
	float* z = NULL;
	float sp_1= 0.0f;
	float sp_2= 0.0f;
	float sp_3= 0.0f;
	glm::vec3 ufp_1 = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 ufp_2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ufp_3 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat3 R1 = glm::mat3(0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f);
	glm::mat3 R2 = glm::mat3(0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f);
	glm::mat3 R3 = glm::mat3(0.0f, 0.0f, 0.0f,
						     0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f);
	glm::vec3 ubar_w1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ubar_w2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ubar_w3 = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 T1_p = glm::mat4(0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f);
	glm::mat4 T2_p = glm::mat4(0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f);
	glm::mat4 T3_p = glm::mat4(0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 0.0f, 0.0f);
}

void arm::loadQVals(float qValInput[NUMOFQVALS])
{
	for (int i = 0; i < NUMOFQVALS; i++)
	{
		qVals[i] = qValInput[i];
	}
	numDivisions = (long)(qValInput[4] / DIVISIONSIZE);

	if (numDivisions < 0)
	{
		numDivisions = 1;
	}
	divisions = new float[numDivisions];
	x = new float[numDivisions];
	y = new float[numDivisions];
	z = new float[numDivisions];

	float divisionIndex = 0.0f;
	for (int i = 0; i < numDivisions; i++)
	{
		divisions[i] = divisionIndex;
		divisionIndex += DIVISIONSIZE;
	}
}

void arm::Detection()
{
	for (int i = 0; i < numDivisions; i++)
	{
		glm::vec3 point = forwardKin(qVals, divisions[i]);
		x[i] = point.x;
		y[i] = point.y;
		z[i] = point.z;
	}
} 

/*ubar_w12 = (1 / (outerK + midK))*glm::vec3((float)(-outerK * ubar_f1.y*glm::sin(qVals[0]) - midK * ubar_f2.y*glm::sin(qVals[1])),
		(float)(outerK*ubar_f1.y*glm::cos(qVals[0]) + midK * ubar_f2.y*glm::cos(qVals[1])),
		(float)0.0f);*/
void arm::initializeVelocities(float *qVals)
{
	R3 = glm::mat3(glm::cos(qVals[3]), -glm::sin(qVals[3]), 0.0f,
		glm::sin(qVals[3]), glm::cos(qVals[3]), 0.0f,
		0.0f, 0.0f, 1.0f);
	ubar_w12 = (1 / (outerK + midK))*
		glm::vec3((float)(-outerK * ubar_f1.y*glm::sin(qVals[0]) - midK * ubar_f2.y*glm::sin(qVals[1])),
		(float)(outerK*ubar_f1.y*glm::cos(qVals[0]) + midK * ubar_f2.y*glm::cos(qVals[1])),
		(float)0.0f);
	//ubar_w3 = ubar_f3*R3;
	ubar_w3 = R3*ubar_f3;
}
glm::vec3 arm::forwardKin(float* qVals, float currentDivision)
{
	initializeVelocities(qVals);
	config = configure(qVals);

	if (qVals[4] >= currentDivision)
	{
		if (config == 1)
		{
			sp_1 = currentDivision;
			ufp_1 = ubar_w3;
		}
		else if (config == 2)
		{
			//segment 1
			if (currentDivision < qVals[2])
			{
				sp_1 = currentDivision;
				ufp_1 = ((outerK + midK)*ubar_w12 + innerK * ubar_w3) / (outerK + midK + innerK);
			}
			//segment 2
			else if (currentDivision >= qVals[2] && currentDivision <= qVals[4])
			{
				sp_1 = qVals[2];
				ufp_1 = ((outerK + midK)*ubar_w12 + innerK * ubar_w3) / (outerK + midK + innerK);

				sp_2 = currentDivision - qVals[2];
				ufp_2 = ubar_w3;
			}
		}
		else if (config == 3)
		{
			sp_1 = 0;
			ufp_1 = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else if (config == 4)
		{
			sp_1 = 0;
			ufp_1 = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		sp_1 = 0;
		ufp_1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (glm::length(ufp_1) != 0 && sp_1 != 0)
	{
		T1_p = Trans(ufp_1, sp_1);
	}
	else
	{
		T1_p = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, 1.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f);
	}
	if (glm::length(ufp_2) != 0 && sp_2 != 0)
	{
		T2_p = Trans(ufp_2, sp_2);
	}
	else
	{
		T2_p = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, 1.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f);
	}

	glm::mat4 W_p = T1_p * T2_p;
	glm::vec3 point = glm::vec3(W_p[0].w, W_p[1].w, W_p[2].w);
	return point; 
}

int arm::configure(float* qVals)
{
	int result = 0; 

	if (abs(qVals[2]) > 0 && abs(qVals[4]) > 0 || abs(qVals[4]) > 0)
	{
		if (qVals[2] <= 0.0000000001)
			result = 1;
		else if (qVals[2] <= qVals[4])
			result = 2;
		else if (qVals[1] <= 0.0000000001 && qVals[4] <= 0.0000000001)
			result = 3;
		else
			result = 4;
	}

	return result;
}

glm::mat4 arm::Trans(glm::vec3 u, float s)
{
	float mag_u = glm::length(u);
	float th = s * mag_u;
	glm::mat4 T;
	if (mag_u == 0)
	{
		T = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, s,
					  0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		T = glm::mat4(
			(((u.x*u.x) + (u.y*u.y)*glm::cos(th))/(mag_u*mag_u)), 
			(u.x*u.y)*(1-glm::cos(th))/(mag_u*mag_u),
			(u.y * glm::sin(th) / mag_u),
			((u.y * (1-glm::cos(th))) / (mag_u * mag_u)),
			(u.x * u.y * (1-cos(th))/(mag_u * mag_u)), 
			(((u.y*u.y) + (u.x * u.x)*glm::cos(th))/(mag_u * mag_u)),
			-(u.x*glm::sin(th)/mag_u), 
			-(u.x * (1-glm::cos(th))/(mag_u*mag_u)),
			-(u.y * glm::sin(th)/mag_u),
			(u.x * glm::sin(th)/mag_u), 
			glm::cos(th), 
			(glm::sin(th)/mag_u),
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	return T;
}