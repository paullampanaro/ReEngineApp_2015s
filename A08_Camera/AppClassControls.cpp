#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	//Flag for the modifier
	bool bModifier = false;

	//ON PRESS/RELEASE
#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

	// Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		camera->SetPosition(vector3(0.0f, 0.0f, -5.0f));
		camera->SetTarget(vector3(0.0f, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		camera->MoveForward(0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		camera->MoveSideways(-0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		camera->MoveForward(-0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		camera->MoveSideways(0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		camera->MoveVertical(0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		camera->MoveVertical(-0.1f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		toggleView = !toggleView;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		camera->ChangeRoll(1.0f);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		camera->ChangeRoll(-1.0f);
	}

	// mouse control from ReEngAppClass.h
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	float a_fSpeed = 0.005;

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (MouseX < CenterX)
	{
		fDeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += fDeltaMouse * a_fSpeed;
	}
	else if (MouseX > CenterX)
	{
		fDeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= fDeltaMouse * a_fSpeed;
	}

	if (MouseY < CenterY)
	{
		fDeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= fDeltaMouse * a_fSpeed;
	}
	else if (MouseY > CenterY)
	{
		fDeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += fDeltaMouse * a_fSpeed;
	}
	//Change the Yaw and the Pitch of the camera
	camera->ChangeYaw(fAngleY * 3.0f);
	camera->ChangePitch(-fAngleX * 3.0f);
	SetCursorPos(CenterX, CenterY);//Position the mouse in the center

	//Exit the program
#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL))
#pragma endregion
}