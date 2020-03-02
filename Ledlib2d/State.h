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
	std::weak_ptr<Camera> _camera;
	Game* _game;
public:
	State();
	virtual ~State();

	void Start();
	void Update();

	virtual void OnStart(){}
	virtual void OnExit(){}

	virtual void OnPause(){}
	virtual void OnResume(){}

	virtual void OnUpdate(){}
	virtual void OnLateUpdate(){}

	virtual void OnRender(){}
	virtual void OnAfterRender(){}

	Scene* GetScene();
	Camera* GetCamera();
	Entity* GetCameraEntity();
	Game* GetGame();

	void SetGame(Game* game);

};

}

