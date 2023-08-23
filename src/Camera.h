#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Input.h"

enum Camera_Movement
{
	FORWAD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.025f;
const float ZOOM = 45.0f;

class Camera
{
	private:
		// Camera Attribute
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		// Euler Angles
		float m_Yaw;
		float m_Pitch;

		// Camera Options
		float m_MovementSpeed;
		float m_MouseSensitivity;
		float m_Zoom;

		float prevX;
		float prevY;

		// Calculate the front vector from the Camera's (updated) Euler Angles
		void UpdateCameraVector()
		{
			// Calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			front.y = sin(glm::radians(m_Pitch));
			front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front = glm::normalize(front);

			// Re-Calculate the right and the up vector
			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		}

	public:
		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
			: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
		{
			m_Position = position;
			m_WorldUp = up;
			m_Yaw = yaw;
			m_Pitch = pitch;
			UpdateCameraVector();
		}
		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
			: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
		{
			m_Position = glm::vec3(posX, posY, posZ);
			m_WorldUp = glm::vec3(upX, upY, upZ);
			m_Yaw = yaw;
			m_Pitch = pitch;
			UpdateCameraVector();
		}
		
		// Process input received from a mouse input system. Expects the offset value in both x and y direction
		void ProcessMousesMovement(GLboolean constrainPitch = true)
		{
			float xOffset = Input::mouseX - prevX;
			float yOffset = prevY - Input::mouseY;

			xOffset *= m_MouseSensitivity;
			yOffset *= m_MouseSensitivity;

			m_Yaw += xOffset;
			m_Pitch += yOffset;

			// Pitch out of bound detection 
			if (constrainPitch)
			{
				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;
			}

			// Update Front, Right and Up Vectors using the updated Euler angles
			UpdateCameraVector();

			prevX = Input::mouseX;
			prevY = Input::mouseY;
		}

		// Process input received from any keyboard-like input system.
		void Input(const float& deltaTime)
		{
			float velocity = m_MovementSpeed * deltaTime;
			// Front/Back
			if (Input::isKeyPressed(GLFW_KEY_W))
				m_Position += m_Front * velocity;
			if (Input::isKeyPressed(GLFW_KEY_S))
				m_Position -= m_Front * velocity;

			// Left/Right
			if (Input::isKeyPressed(GLFW_KEY_D))
				m_Position += m_Right * velocity;
			if (Input::isKeyPressed(GLFW_KEY_A))
				m_Position -= m_Right * velocity;
			
			// Up/Down
			if (Input::isKeyPressed(GLFW_KEY_E))
				m_Position.y += velocity;
			if (Input::isKeyPressed(GLFW_KEY_Q))
				m_Position.y -= velocity;

		}

		// Getter
		glm::mat4 GetViewMatrix() const
		{
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}
		glm::vec3 GetPosition() const { return m_Position; }

		void ResetMousePosition()
		{
			prevX = Input::mouseX;
			prevY = Input::mouseY;
		}

};