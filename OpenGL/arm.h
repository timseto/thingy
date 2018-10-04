#pragma once
#include <glm.hpp>
#include <gtc/constants.hpp>
#define pie 3.14159265359
#define NUMOFQVALS 5

class arm
{
	//constants 
	//tube Dimensions
	const float outerOD = 2.3114f;
	const float outerID = 2.1082f;
	const float midOD = 1.8542f;
	const float midID = 1.7018f;
	const float innerOD = 1.4478f;
	const float innerID = 1.3208f;
	//elastic modulous (E)
	const float outerE = 56073000000.0f / 1000000.0f; 
	const float midE = 57965000000.0f / 1000000.0f;
	const float innerE = 61293000000.0f / 1000000.0f;

	//moments of Inertia (I)
	const float outerI = (float)((pie / 64.0f) * (glm::pow(outerOD, 4.0f) - glm::pow(outerID, 4.0f)));
	const float midI = (float) ((pie / 64.0f) * (glm::pow(midOD, 4.0f) - glm::pow(midID, 4.0f)));
	const float innerI = (float) ((pie / 64.0f) * (glm::pow(innerOD, 4.0f) - glm::pow(innerID, 4.0f)));

	//stiffness Tensor (K)
	const float outerK = (float)outerE * outerI;
	const float midK = (float)midE * midI;
	const float innerK = (float)innerE * innerI;

	//radius of curvature (rho)
	const float outerRho = 77.046f;
	const float midRho = 77.046f;
	const float innerRho = 48.26f;

	//angular rate of change
	glm::vec3 ubar_f1 = glm::vec3(0.0f, 1.0f / outerRho, 0.0f);
	glm::vec3 ubar_f2 = glm::vec3(0.0f, 1.0f / midRho, 0.0f);
	glm::vec3 ubar_f3 = glm::vec3(0.0f, 1.0f / innerRho, 0.0f);


	//Fields 
	int config; 
	float qVals[NUMOFQVALS];
	float *divisions;
	float sp_1;
	float sp_2;
	float sp_3;
	glm::vec3 ufp_1;
	glm::vec3 ufp_2;
	glm::mat3 R3;
	glm::vec3 ubar_w12;
	glm::vec3 ubar_w3;
	glm::mat4 T1_p;
	glm::mat4 T2_p;

public:
	int numDivisions;
	float* x;
	float* y;
	float* z;
	arm();
	void Detection();
	glm::vec3 forwardKin(float* qVals, float currentDivisions);
	void loadQVals(float qValInput[NUMOFQVALS]);
	void initializeVelocities(float* qVals);
	int configure(float* qVals);
	glm::mat4 Trans(glm::vec3 u, float s);

	void cleanup()
	{
		delete []divisions;
		delete []x;
		delete []y;
		delete []z;		
	}
};

