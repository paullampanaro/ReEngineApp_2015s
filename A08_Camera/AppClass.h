/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/09
----------------------------------------------*/
#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>
//#include <chrono>

using namespace ReEng; //Using ReEng namespace to use all the classes in the dll

// Camera Singleton - follows basic design pattern
// using glm::conjugate based on some internet research
class Camera
{
private:
	// necessary vectors
	vector3 position = vector3(0.0f, 0.0f, 30.0f);
	vector3 target = vector3(0.0f, 0.0f, 0.0f);
	vector3 forward = vector3(0.0f, 0.0f, -1.0f);
	vector3 up = vector3(0.0f, 1.0f, 0.0f);
	vector3 right = vector3(1.0f, 0.0f, 0.0f);

	// quat for doing orientation
	glm::quat orient = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	// matrices
	matrix4 view = IDENTITY_M4;
	matrix4 proj = IDENTITY_M4;

public: 
	static Camera* instance;
	
	static Camera* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new Camera();
		}
		return instance;
	};

	static void ReleaseInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	};

	// gets View Matrix from singleton
	matrix4 GetView(void)
	{
		view = glm::toMat4(orient);
		view = glm::translate(view, -position);
		return view;
	};

	// get Projection Matrix from singleton, bool decides between Perspective or Orthographic
	matrix4 GetProjection(bool bOrthographic)
	{
		if (bOrthographic)
			proj = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
		else
			proj = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.01f, 1000.0f);

		return proj;
	};

	// sets the position of the camera
	void SetPosition(vector3 v3Position)
	{
		position = v3Position;
	};

	// sets the target of the camera
	void SetUp(vector3 v3Up)
	{
		up = v3Up;
	};

	// move the camera forward (or backwards) depending on the "view vector"
	void MoveForward(float fIncrement)
	{
		position += (glm::conjugate(orient) * vector3(0.0f, 0.0f, -1.0f)) * fIncrement;
	};

	// move the camera based on the "right" vector depending on the "view" vector
	void MoveSideways(float fIncrement)
	{
		position += (glm::conjugate(orient) * vector3(1.0f, 0.0f, 0.0f)) * fIncrement;
	};

	// move the camera up or down depending on the "up vector"
	void MoveVertical(float fIncrement)
	{
		position += (glm::conjugate(orient) * vector3(0.0f, 1.0f, 0.0f)) * fIncrement;
	};

	// orient your camera (meaning forward and up vectors) changing its angle in x
	void ChangePitch(float fIncrement)
	{
		orient = orient * glm::angleAxis(fIncrement, vector3(1.0f, 0.0f, 0.0f));
	};

	// orient your camera (meaning forward and up vectors) changing its angle in z
	void ChangeRoll(float fIncrement)
	{
		orient = orient * glm::angleAxis(fIncrement, vector3(0.0f, 0.0f, 1.0f));
	};

	// orient your camera (meaning forward and up vectors) changing its angle in y
	void ChangeYaw(float fIncrement)
	{
		orient = orient * glm::angleAxis(fIncrement, vector3(0.0f, 1.0f, 0.0f));
	};
};

class AppClass : public ReEngAppClass
{
	PrimitiveClass* m_pCone = nullptr;
	PrimitiveClass* m_pCylinder = nullptr;

	float m_fTemp = 0.0f;

	Camera* camera;

public:
	typedef ReEngAppClass super;

	/* Constructor */
	AppClass(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow) : super(hInstance, lpCmdLine, nCmdShow) {}

	/*
	InitWindow
	Initialize ReEng variables necessary to create the window
	*/
	virtual void InitWindow(String a_sWindowName);

	/*
	InitVariables
	Initializes user specific variables, this is executed right after InitApplicationVariables,
	the purpose of this member function is to initialize member variables specific for this lesson
	*/
	virtual void InitVariables(void);

	/*
	Update
	Updates the scene
	*/
	virtual void Update(void);

	/*
	Display
	Displays the scene
	*/
	virtual void Display(void);

	/*
	ProcessKeyboard
	Manage the response of key presses
	*/
	virtual void ProcessKeyboard(void);

	/*
	Release
	Releases the application
	IF INHERITED AND OVERRIDEN MAKE SURE TO RELEASE BASE POINTERS (OR CALL BASED CLASS RELEASE)
	*/
	virtual void Release(void);

	/*
	USAGE: Reads the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void ReadConfig(void) final {}
	/*
	USAGE: Writes the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	virtual void WriteConfig(void) final {}
};

#endif //__APPLICATION_H_
