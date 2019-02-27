#pragma once

#include "../Math/Vector2f.h"
#include "Keys.h"
#include <vector>

namespace Ledlib {

class Event;

class Client {
public:
	int id;
	uint32_t flags = 0;
	bool connected;
	std::vector<Event*> events;
	std::vector<Event*> newEvents;

	Client(int id);
	void Update();
	bool IsKeyDown(KeyCode code);
	bool OnKeyDown(KeyCode code);
	bool OnKeyUp(KeyCode code);
	Vector2f GetJoystickPosition(KeyCode code);

	void AddMessage(const std::string& str);
	void NextGeneration();

	void SetFlag(uint32_t flag);
	void UnsetFlag(uint32_t flag);
	bool IsFlagSet(uint32_t flag);

private:
	bool isDownMap[static_cast<int>(KeyCode::KEY_CODE_MAX)];
	bool onDownMap[static_cast<int>(KeyCode::KEY_CODE_MAX)];
	bool onUpMap[static_cast<int>(KeyCode::KEY_CODE_MAX)];
	Vector2f joystickPositions[2];
};

}


