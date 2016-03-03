#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_selection = std::pair<int, int>(-1, -1);
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 12.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	// set a new camera position
	m_pCameraMngr->SetPosition(vector3(0.0f, 0.0f, 35.0f));

	// intialize the m_nObjects variable to random numbers
	srand(time(NULL));
	m_nObjects = rand() % 23 + 5;

	// make a couple of start and end vectors
	vector3 v3Start = vector3(-m_nObjects, 0.0f, 0.0f);
	vector3 v3End = vector3(m_nObjects, 0.0f, 0.0f);

	// initialize the m_pShere variable and the m_pMatrix variable
	m_pSphere = new PrimitiveClass[m_nObjects];
	m_pMatrix = new matrix4[m_nObjects]; 

	for (int i = 0; i < m_nObjects; i++)
	{
		// generates a percentage of the lerp
		float fPercent = MapValue(static_cast<float>(i), 0.0f, static_cast<float>(m_nObjects), 0.0f, 1.0f);

		// generate new primitve as a new sphere
		m_pSphere[i].GenerateSphere(1, 5, vector3(fPercent, 0.0f, 0.0f));

		// translate vector of return of the lerp from v3start and v3end as color
		vector3 tVec = glm::lerp(v3Start, v3End, fPercent);

		// update position of each matrix
		m_pMatrix[i] = glm::translate(tVec);
	}

	m_pLightMngr->SetColor(REWHITE, 0);
	m_pLightMngr->SetIntensity(0.1f, 0);
	m_pLightMngr->SetColor(REWHITE, 1);
	m_pLightMngr->SetIntensity(0.5f, 1);
	m_pLightMngr->SetPosition(vector3(0.0f, 1.0f,-1.0f), 1);

	//Load a model onto the Mesh manager
	//m_pMeshMngr->LoadModel("tests\\Cubev.fbx", "Unikitty");
	int nCubes = 10;
	vector3 v3Start(-nCubes/2.0f, 0.0f, -nCubes / 2.0f);
	m_pMeshMngr->LoadModel("Cube.obj", "ElCubo");
	m_pMeshMngr->SetShaderProgramByName("ElCubo", "Phong");
	for (uint n = 0; n < nCubes; n++)
	{
		if (v3Start != vector3(0.0f))
		{
			String sName = "Cube_" + std::to_string(n);
			m_pMeshMngr->LoadModel("Cube.obj", sName, false, glm::translate(v3Start));
			m_pMeshMngr->SetShaderProgramByName(sName, "Phong");
		}
		v3Start += vector3(1.0f, 0.0f, 1.0f);
	}
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3Position) * ToMatrix4(m_qArcBall), 0);
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
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

	// render spheres
	for (int i; i < m_nObjects; i++)
	{
		// m_pSphere[i]->Render()
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

	// release memory
	delete[] m_pSphere;
	delete[] m_pMatrix;
}