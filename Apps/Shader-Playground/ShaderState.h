#pragma once

#include "Ledlib2d/State.h"
#include <memory>

using namespace Ledlib;

namespace Ledlib {
class ShaderBox;
}

class ShaderState : public State {
public:	
	std::unique_ptr<ShaderBox> shader;
	ShaderState();
	void OnRender();
};

