#include "StateManager.h"
#include "State.h"

namespace Ledlib {

static std::shared_ptr<State> activeState;
static std::shared_ptr<State> queuedState;

StateManager::StateManager(){}

std::shared_ptr<State> StateManager::SetState(std::shared_ptr<State> state){
	queuedState = state;
	return state;
}
std::shared_ptr<State> StateManager::GetState(){
	return activeState;
}

void StateManager::Swap(){
	if(queuedState){
		activeState = queuedState;
		queuedState = nullptr;
	}
}

}

