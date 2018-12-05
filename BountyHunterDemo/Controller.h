///-------------------------------------------------------------------------------------------------
/// File:	Controller.h.
///
/// Summary:	Declares the controller class.
///-------------------------------------------------------------------------------------------------

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "GameTypes.h"
#include "IController.h"

class Controller
{
private:

	IController*	m_ControllerImpl;

	PlayerAction*	m_FrameAction;

public:

	Controller(IController* controllerImpl);

	~Controller();

	void Release();

	void SetController(IController* controllerImpl);

	bool SwapController(IController* controllerImpl);

	bool Possess(const GameObjectId gameObjectId);

	void Unpossess();

	GameObjectId GetPossessed();

	inline void SetFrameAction(PlayerAction* const action) { this->m_FrameAction = action; }

	void Update();

}; // class Controller

#endif // __CONTROLLER_H__
