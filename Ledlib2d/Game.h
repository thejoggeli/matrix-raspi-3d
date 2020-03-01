#pragma once

#include <memory>

namespace Ledlib {

class State;
class Scene;
class Camera;
class Entity;

class Game : public std::enable_shared_from_this<Game> {
private:
	static int idCounter;
	int _id;
	static std::shared_ptr<Game> instance;
	std::shared_ptr<State> activeState;
	std::shared_ptr<State> queuedState;
	bool hasQueuedState;
public:

	Game();
	virtual ~Game();

	virtual void OnSetup(){}
	virtual void OnStart(){}
	virtual void OnExit(){}

	static void Run(Game* game);
	static std::shared_ptr<Game> GetInstance();

	void Loop();

	static std::shared_ptr<Camera> GetCameraFromState();
	static std::shared_ptr<Entity> GetCameraEntityFromState();
	static std::shared_ptr<Scene> GetSceneFromState();

	std::shared_ptr<State> SetState(const std::shared_ptr<State>& state);
	template<typename T, typename std::enable_if<std::is_base_of<State, T>::value>::type* = nullptr>
	std::shared_ptr<T> SetState(){
		std::shared_ptr<T> state = std::make_shared<T>();
		SetState(state);
		return state;
	}

	std::shared_ptr<State> GetState();
	template<typename T, typename std::enable_if<std::is_base_of<State, T>::value>::type* = nullptr>
	std::shared_ptr<T> GetState(){
		return std::static_pointer_cast<T>(GetState());
	}

};

}


