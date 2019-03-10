#pragma once

#include <memory>

namespace Ledlib {

class Game;
class Scene;
class Entity;
class Camera;

class State {
private:
	static int idCounter;
	int _id;
	std::shared_ptr<Scene> _scene;
	std::weak_ptr<Entity> _cameraEntity;
	std::weak_ptr<Game> _game;
public:
	State();
	virtual ~State();
	template<typename T>
	static std::shared_ptr<State> Create(const std::shared_ptr<Game>& game){
		return std::static_pointer_cast<T>(Init(std::make_shared<State>(), game));
	}
	static std::shared_ptr<State> Init(const std::shared_ptr<State>& state, const std::shared_ptr<Game>& game);

	void Start();
	void Update();

	virtual void OnStart(){}
	virtual void OnEnd(){}

	virtual void OnPause(){}
	virtual void OnResume(){}

	virtual void OnUpdate(){}
	virtual void OnLateUpdate(){}

	virtual void OnBeforeRender(){}
	virtual void OnRender(){}
	virtual void OnAfterRender(){}

	std::shared_ptr<Scene> GetScene();
	std::shared_ptr<Camera> GetCamera();
	std::shared_ptr<Game> GetGame();

};

}

