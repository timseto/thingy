#define GLM_ENABLE_EXPERIMENTAL
#define MOVESPEED 0.01f
#include "Camera.h"
#include <gtx/transform.hpp>

Camera::Camera() :
	viewDirection(0.0f, -1.0f, -1.0f),
	//viewDirection(0.0f, 0.0f, 1.0f),
	UP(0.0f, 1.0f, 0.0f),
	//position(0.0f, 0.0f, -0.35f)
	position(0.0f,0.375f,0.2f)
{

}
void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	viewDirection = glm::mat3(glm::rotate(mouseDelta.x, UP))* viewDirection;

	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	viewDirection = glm::mat3(glm::rotate(mouseDelta.y, toRotateAround))* viewDirection;
	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}
void Camera::setPosition(glm::vec3 input)
{
	position = input;
}
void Camera::moveForward()
{
	position += MOVESPEED * viewDirection;
}
void Camera::moveBack()
{
	position -= MOVESPEED * viewDirection;
}
void Camera::moveUp()
{
	position += MOVESPEED * UP;
}
void Camera::moveDown()
{
	position -= MOVESPEED * UP;
}
void Camera::strafeLeft()
{
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	position -= MOVESPEED * toRotateAround;

}
void Camera::strafeRight()
{
	glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
	position += MOVESPEED * toRotateAround;
}