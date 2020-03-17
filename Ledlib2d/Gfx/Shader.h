#pragma once

namespace Ledlib {

class Shader {
public:
	bool compiled = false;
	int id = 0;
	int fragId = 0;
	int vertId = 0;
	Shader();
	void Use();
	void Compile(const char* vert, const char* frag);
	virtual ~Shader();
};

namespace Gfx {


}
}

