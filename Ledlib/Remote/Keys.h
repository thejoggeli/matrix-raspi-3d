#pragma once

#include <string>

namespace Ledlib {

enum class KeyCode {
        A = 0, B = 1, X = 2, Y = 3,
        Up = 10, Down = 11, Left = 12, Right = 13,
		LeftJoystick = 18, RightJoystick = 19,
        Start = 20, Select = 21,
        KEY_CODE_MAX = 24
};

enum class KeyState {
        Pressed = 0,
        Released = 1,
		JoystickMove = 2,
};

std::string KeyCodeToString(KeyCode keyCode);
std::string KeyStateToString(KeyState keyState);

}
