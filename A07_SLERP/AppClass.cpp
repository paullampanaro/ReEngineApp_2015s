#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Lampanaro, Paul"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	// percentages for various slerp
	float fPercentEarthRev = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fEarthHalfRevTime), 0.0f, 1.0f);
	float fPercentEarthOrb = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fEarthHalfOrbTime), 0.0f, 1.0f);
	float fPercentMoonOrb = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fMoonHalfOrbTime), 0.0f, 1.0f);

	// convert the control quaternion into a matrix4 for use with other matrices
	glm::mat4 m4Control = glm::mat4_cast(qControl);
	
	// start and end quaternions for all orientations
	glm::quat qStart = glm::quat_cast(m4Control);
	glm::quat qEnd = qStart * glm::angleAxis(180.0f, vector3(0.0f, 1.0f, 0.0f));

	// find the slerp for Earth's orbit
	glm::quat qCurrentEarthOrb = glm::mix(qStart, qEnd, fPercentEarthOrb);
	glm::quat qCurrentEarthRev = glm::mix(qStart, qEnd, fPercentEarthRev);
	glm::quat qCurrentMoonOrb = glm::mix(qStart, qEnd, fPercentMoonOrb);

	// apply orbital orientation, translate, and scaling (in reverse)
	m4Earth = m4Control * glm::mat4_cast(qCurrentEarthOrb) * glm::translate(vector3(11.0f, 0.0f, 0.0f)) * glm::scale(vector3(0.524f, 0.524f, 0.524f));

	// repeat for moon, but also apply Earth's position
	m4Moon = m4Earth * glm::mat4_cast(qCurrentMoonOrb) * glm::translate(vector3(2.0f, 0.0f, 0.0f)) * glm::scale(vector3(0.27f, 0.27f, 0.27f));

	// now apply Earth's revolution orientation, so it doesnt affect the moon's orbit
	m4Earth *= glm::mat4_cast(qCurrentEarthRev);

	// scale the sun
	m4Sun = m4Control * glm::scale(vector3(5.936f, 5.936f, 5.936f));

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	int nEarthOrbits = fRunTime / (fEarthHalfOrbTime * 2);
	int nEarthRevolutions = fRunTime / (fEarthHalfRevTime * 2);
	int nMoonOrbits = fRunTime / (fMoonHalfOrbTime * 2);

	// if statements to increment counters
	
	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

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
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}