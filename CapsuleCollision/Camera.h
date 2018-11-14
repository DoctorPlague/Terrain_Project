#include "Utilities.h"

#pragma once
class Camera
{
#pragma region Singleton
public:
	static std::shared_ptr<Camera> GetInstance();
	static void DestroyInstance();
	~Camera();
	/*!!!!!!!!! YOU SHOULD NOT BE CALLING THIS DIRECTLY !!!!!!!*/
	Camera();
private:
	static std::shared_ptr<Camera> s_pCamera;
	Camera(Camera const&);
	void operator=(Camera const&);
	
#pragma endregion

public:	
	//Getters
	glm::mat4 GetView();
	glm::mat4 GetProj();
	glm::vec3 GetPos();
	glm::vec3 GetFront();
	glm::vec3 GetUp();
	
	//Setters	
	void SetProj(int _screenWidth, int _screenHeight);
	//Other functions
	void Update();
	void UpdatePosition(glm::vec3 _target);
	void UpdateHeight(float _height);

	// Camera Movement
	void MoveLeft(float _deltaTick);
	void MoveRight(float _deltaTick);
	void MoveForward(float _deltaTick);
	void MoveBackward(float _deltaTick);
	
private:
	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_upVec;
	float m_fCameraSpeed;
};

