#include "Utilities.h"
#include "KeyboardInput.h"
#include "Camera.h"
#include "Terrain.h"
#include "clock.h"
#include "Sphere.h"
#include "Star.h"
#include "TessQuad.h"
#include "ParticleSystem.h"
#include "GPGPUParticleSystem.h"
#include "FrameBuffer.h"
#include "ssAnimatedModel.h"

Terrain* exampleTerrain = new Terrain();
//FrameBuffer* exampleFrameBuffer = new FrameBuffer();
ssAnimatedModel* exampleModel = nullptr;
ParticleSystem* exampleParticleSystem = nullptr;
GPGPUParticleSystem* exampleGPGPUParticleSystem = nullptr;

float cameraDistanceOffset = 0;
float fDeltaTotal = 0.0f;

void Render();
void Update();
void Exit();

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);		
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(ki_SCREENWIDTH, ki_SCREENHEIGHT);
	glutCreateWindow("Week3 Physics");	
	glLineWidth(0.01f);
	glewInit();

	CClock::GetInstance()->Initialise();
	Input::GetInstance()->Initialize();
	Camera::GetInstance()->SetProj(900, 900);
			
	//exampleFrameBuffer->Initialize();
	exampleTerrain->Initialize();	
	exampleModel = new ssAnimatedModel("Resources\\Images\\theDude.DAE", "Resources\\Images\\theDude.png");
	exampleModel->setCurrentAnimation(0, 30); // set idle animation
	exampleModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	exampleModel->setScale(glm::vec3(0.0675f));
	exampleModel->setSpeed(50.0f);
	exampleParticleSystem = new ParticleSystem(glm::vec3(0.0f, 100.0f, 0.0f), "Resources\\Images\\RainDrop.png");
	exampleParticleSystem->Initialize();
	exampleGPGPUParticleSystem = new GPGPUParticleSystem();
	exampleGPGPUParticleSystem->Initialize();

	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutIgnoreKeyRepeat(1);
	glutCloseFunc(Exit);
	glutMainLoop();
	return 0;
}

void Render()
{		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//exampleFrameBuffer->BindFrameBuffer();	
	
	exampleTerrain->Render();	
	exampleModel->render(exampleTerrain);
	exampleParticleSystem->Render(CClock::GetInstance()->GetDeltaTick() / 1000);
	exampleGPGPUParticleSystem->Render();
	
	//exampleFrameBuffer->Render(fDeltaTotal);

	glutSwapBuffers();
}

void Update()
{
	// Process time
	CClock::GetInstance()->Process();
	float fDeltaTick = CClock::GetInstance()->GetDeltaTick() / 1000;
	fDeltaTotal += fDeltaTick;

	// Update stuff goes here				
	cameraDistanceOffset = glm::clamp(cameraDistanceOffset, 0.5f, 100.0f);
	Camera::GetInstance()->UpdatePosition(exampleModel->GetPosition() + glm::vec3(0.0f, cameraDistanceOffset, cameraDistanceOffset));
	Camera::GetInstance()->Update();

	// Update input
	Input::GetInstance()->Update();
	if (Input::GetInstance()->KeyState['a'] == INPUT_HOLD && Input::GetInstance()->KeyState['d'] != INPUT_HOLD)
	{
		//exampleSphere->RelativeTranslation(glm::vec3(-25.0f, 0.0f, 0.0f), fDeltaTick);	
		exampleModel->rotate(90.0f);
		
	}
	else if (Input::GetInstance()->KeyState['d'] == INPUT_HOLD && Input::GetInstance()->KeyState['a'] != INPUT_HOLD)
	{
		//exampleSphere->RelativeTranslation(glm::vec3(25.0f, 0.0f, 0.0f), fDeltaTick);		
		exampleModel->rotate(-90.0f);
	}
	else
	{
		exampleModel->rotate(0.0f);
	}


	if (Input::GetInstance()->KeyState[32] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[32] == INPUT_HOLD)
	{
		exampleModel->move(25.0f);
		if (exampleModel->bMoving == false) {
			exampleModel->bMoving = true;
			exampleModel->setCurrentAnimation(71, 80); // jump animation
		}
	}		
	else if (Input::GetInstance()->KeyState['s'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['s'] == INPUT_HOLD)
	{
		//exampleSphere->RelativeTranslation(glm::vec3(0.0f, 0.0f, 25.0f), fDeltaTick);
		exampleModel->move(-25.0f);
		if (exampleModel->bMoving == false) {
			exampleModel->bMoving = true;
			exampleModel->setCurrentAnimation(51, 70); // run animation
		}
	}
	else if (Input::GetInstance()->KeyState['w'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['w'] == INPUT_HOLD)
	{
		//exampleSphere->RelativeTranslation(glm::vec3(0.0f, 0.0f, -25.0f), fDeltaTick);		
		exampleModel->move(25.0f);
		if (exampleModel->bMoving == false) {
			exampleModel->bMoving = true;
			exampleModel->setCurrentAnimation(31, 50); // run animation
		}
	}
	else
	{
		exampleModel->move(0.0f);
		if (exampleModel->bMoving == true) {
			exampleModel->bMoving = false;
			exampleModel->setCurrentAnimation(0, 30); //idle animation
		}
	}
	

	if (Input::GetInstance()->MouseState[0] == INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[0] == INPUT_HOLD)
	{
		cameraDistanceOffset -= 50.0f * fDeltaTick;
	}
	if (Input::GetInstance()->MouseState[2] == INPUT_FIRST_PRESS || Input::GetInstance()->MouseState[2] == INPUT_HOLD)
	{
		cameraDistanceOffset += 50.0f * fDeltaTick;
	}

	glutPostRedisplay();
}

void Exit()
{
	delete exampleTerrain;
	delete exampleModel;
	//delete exampleFrameBuffer;
	delete exampleParticleSystem;
	delete exampleGPGPUParticleSystem;
}
