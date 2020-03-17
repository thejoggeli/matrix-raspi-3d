#pragma once

#include <memory>

namespace Ledlib {

class App : public std::enable_shared_from_this<App> {
private:
	static int idCounter;
	int _id;
	static std::shared_ptr<App> instance;
	bool hasQueuedState;
public:

	App();
	virtual ~App();

	virtual void OnSetup(){}
	virtual void OnStart(){}
	virtual void OnUpdate(){}
	virtual void OnRender(){}
	virtual void OnExit(){}

	static void Run(App* App);
	static std::shared_ptr<App> GetInstance();

	void Loop();

};

}
