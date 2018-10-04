#pragma once
#include <glm.hpp>

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection; 
	const glm::vec3 UP;
	glm::vec2 oldMousePosition = glm::vec2(0.0f,0.0f); 

public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	void moveForward();
	void moveBack();
	void moveUp();
	void moveDown();
	void strafeLeft();
	void strafeRight();
	void setPosition(glm::vec3 a);
	glm::mat4 getWorldToViewMatrix() const; 
};

