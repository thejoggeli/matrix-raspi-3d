#pragma once

#include <memory>

namespace Ledlib {

class State;

class StateManager {


public:
	StateManager();

	template<typename T, typename std::enable_if<std::is_base_of<State, T>::value>::type* = nullptr>
	static std::shared_ptr<T> SetState(){
		std::shared_ptr<T> state = std::make_shared<T>();
		SetState(state);
		return state;
	}
	static std::shared_ptr<State> SetState(std::shared_ptr<State> state);

	template<typename T, typename std::enable_if<std::is_base_of<State, T>::value>::type* = nullptr>
	static std::shared_ptr<T> GetState(){
		return std::static_pointer_cast<T>(GetState());
	}
	static std::shared_ptr<State> GetState();

	static void Swap();

};

}


