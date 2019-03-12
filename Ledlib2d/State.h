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

	void Start();
	void Update();

	virtual void OnStart(){}
	virtual void OnEnd(){}

	virtual void OnPause(){}
	virtual void OnResume(){}

	virtual void OnUpdate(){}
	virtual void OnLateUpdate(){}

	virtual void OnRender(){}
	virtual void OnAfterRender(){}

	std::shared_ptr<Scene> GetScene();
	std::shared_ptr<Camera> GetCamera();
	std::shared_ptr<Entity> GetCameraEntity();
	std::shared_ptr<Game> GetGame();

	void SetGame(const std::shared_ptr<Game>& game);

	template<typename T, typename std::enable_if<std::is_base_of<Game, T>::value>::type* = nullptr>
	std::shared_ptr<T> GetGame(){
		return std::static_pointer_cast<T>(_game.lock());
	}

};

}

