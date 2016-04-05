#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E10_Projections"); // Window Name
}
void AppClass::InitVariables(void)
{
	//Generate the Cone
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);

	//Calculate the first projections
	m_m4Projection = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	m_m4View = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),  glm::vec3(0.0f, 0.0f, 14.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	/*
	//Calculate the first projections
	m_m4Projection = glm::perspective(
		45.0f, // angle of view
		1080.0f / 768.0f, // ratio
		0.01f, // near clip plane (relative to camera)
		1000.0f // far clip plane (relative to camera)
		); 

	m_m4Projection = glm::ortho(
		-20.0f, // left
		20.0f, // right
		-10.0f, // bottom
		10.0f, // top
		0.01f, // near
		1000.0f // far
		);

	m_m4View = glm::lookAt(
		glm::vec3(0.0f, 3.0f, 15.0f),  // position
		glm::vec3(0.0f, 0.0f, 0.0f), // what I'm looking at
		glm::vec3(0.0f, 1.0f, 0.0f)); // up

	*/
}

void AppClass::Display(void)
{
	//Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the grid
	m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY);

	//Render the cone
	m_pCone->Render(m_m4Projection, m_m4View, IDENTITY_M4);

	//Render the cylinder
	m_pCylinder->Render(m_m4Projection, m_m4View, glm::translate(IDENTITY_M4, REAXISZ * -3.0f));

	//Render the rest of the meshes
	m_pMeshMngr->Render();

	//Swaps the OpenGL buffers
	m_pGLSystem->GLSwapBuffers();
}

void AppClass::Release(void)
{
	//Release the memory of the member fields
	SafeDelete(m_pCone);
	SafeDelete(m_pCylinder);

	//Release the memory of the inherited fields
	super::Release(); 
}