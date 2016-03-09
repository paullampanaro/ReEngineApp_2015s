#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	// load vector 3 translation points into std::vector container
	tPoints.push_back(vector3(-4.0f, -2.0f, 5.0f));
	tPoints.push_back(vector3(1.0f, -2.0f, 5.0f));
	tPoints.push_back(vector3(-3.0f, -1.0f, 3.0f));
	tPoints.push_back(vector3(2.0f, -1.0f, 3.0f));
	tPoints.push_back(vector3(-2.0f, 0.0f, 0.0f));
	tPoints.push_back(vector3(3.0f, 0.0f, 0.0f));
	tPoints.push_back(vector3(-1.0f, 1.0f, -3.0f));
	tPoints.push_back(vector3(4.0f, 1.0f, -3.0f));
	tPoints.push_back(vector3(0.0f, 2.0f, -5.0f));
	tPoints.push_back(vector3(5.0f, 2.0f, -5.0f));
	tPoints.push_back(vector3(1.0f, 3.0f, -5.0f));

	// take care of circle generation
	m_pSphere = new PrimitiveClass();
	m_pSphere->GenerateSphere(0.1f, 5, RERED);
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");

	// start at 0 - switched this to static variable cause it will look nicer?
	static int aCycle = 0;

	// if the runtime is greater than duration, done with set of first points
	if (fRunTime > fDuration)
	{
		// reset run time
		fRunTime -= fDuration;

		// switch to next set of points
		aCycle++;
		int bCycle = aCycle + 1;

		// loop back to beginning of translation points
		if (aCycle == tPoints.size())
		{
			aCycle = 0;
		}
	}

	// int for accessing next vector in container
	int bCycle = aCycle + 1;

	if (bCycle == tPoints.size())
	{
		bCycle = 0;
	}

	// generates a percentage of the lerp
	float fPercent = MapValue(static_cast<float>(fRunTime), 0.0f, fDuration, 0.0f, 1.0f);

	// translate vector of return of the lerp from first point and second 
	vector3 tVec = glm::lerp(tPoints[aCycle], tPoints[bCycle], fPercent);

	// make into a matrix
	matrix4 tMatrix = glm::translate(tVec);

	m_pMeshMngr->SetModelMatrix(tMatrix, "WallEye");

#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	// render the circles
	matrix4 mProj = m_pCameraMngr->GetProjectionMatrix();
	matrix4 mView = m_pCameraMngr->GetViewMatrix();

	for (int i = 0; i < tPoints.size(); i++)
	{
		m_pSphere->Render(mProj, mView, glm::translate(tPoints[i]));
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	// remember to delete
	SafeDelete(m_pSphere);
	super::Release(); //release the memory of the inherited fields
}