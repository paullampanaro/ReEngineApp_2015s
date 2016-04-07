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

	//Exit the program
#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL))
#pragma endregion
}