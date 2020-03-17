#pragma once

#include "Display.h"

namespace Ledlib {

class DummyDisplay : public Display {
public:
	DummyDisplay();
	bool Init();
};

}
