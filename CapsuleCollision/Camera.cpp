#include "Camera.h"

std::shared_ptr<Camera> Camera::s_pCamera;

std::shared_ptr<Camera> Camera::GetInstance()
{
	if (!s_pCamera)
	{
		s_pCamera = std::shared_ptr<Camera>(new Camera());
	}
	return s_pCamera;
}

void Camera::DestroyInstance()
{
	s_pCamera.reset();
	//s_pSceneManager = nullptr;
}


Camera::Camera()
{
	// Default camera position
	//m_cameraPos = glm::vec3(0.0f, 200.0f, 200.0f);
	//m_cameraFront = glm::vec3(0.0f, -2.0f, -1.0f);
	//m_upVec = glm::vec3(0.0, 1.0f, 0.0f);
	//m_fCameraSpeed = 0.25f;

	m_cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
	m_cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);
	m_upVec = glm::vec3(0.0, 1.0f, 0.0f);
	m_fCameraSpeed = 7.5f;
}


Camera::~Camera()
{
}

glm::mat4 Camera::GetView()
{
	return m_view;
}

glm::mat4 Camera::GetProj()
{
	return m_proj;
}

glm::vec3 Camera::GetPos()
{
	return m_cameraPos;
}

void Camera::SetProj(int _screenWidth, int _screenHeight)
{
	m_proj = glm::perspective(45.0f, (GLfloat)ki_SCREENWIDTH / (GLfloat)ki_SCREENHEIGHT, 0.1f, 2000.0f);
}

void Camera::Update()
{
	m_view = glm::lookAt(m_cameraPos,
						 m_cameraPos + m_cameraFront,
						 m_upVec);
}

void Camera::UpdatePosition(glm::vec3 _target)
{
	m_cameraPos = _target + glm::vec3(0.0f, 10.0f, 10.0f);
}

void Camera::UpdateHeight(float _height)
{
	m_cameraPos.y = _height + 1.0f;
}

void Camera::MoveLeft(float _deltaTick)
{
	m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_upVec)) * m_fCameraSpeed * _deltaTick;
}

void Camera::MoveRight(float _deltaTick)
{
	m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_upVec)) * m_fCameraSpeed * _deltaTick;
}

void Camera::MoveForward(float _deltaTick)
{
	m_cameraPos += glm::normalize(m_cameraFront) * m_fCameraSpeed * _deltaTick;
}

void Camera::MoveBackward(float _deltaTick)
{
	m_cameraPos -= glm::normalize(m_cameraFront) * m_fCameraSpeed * _deltaTick;
}

